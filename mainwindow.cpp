#include "mainwindow.h"
#include "./ui_mainwindow.h"

// Janela principal que gerencia todo o código
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)   // Inicializa o parent
    , ui(new Ui::MainWindow) // Inicializa o ponteiro para a interface
    , sensorWindow(nullptr)  // Inicializa o ponteiro como nullptr
    , sensorData{"Modelo não selecionado",0.0,360.0,"CW"}   // Inicializa o objeto sensorData
    , actual_servo_value(0x00) // Inicializa a variável com o valor inicial do servo
    , myServo(nullptr)        // ponteiro para a classe que controla o servo
    , logServoWindow(nullptr) // ponteiro para a janela de log dentro da mainWindow
    , servoUP(false)          // bool que comunica o estado do servo (se está apto a ser usado)
    , myUser{"Comum","Produção"} // Inicializa o objeto myUser com o nome e permissão do usuário
{

    ui->setupUi(this); // configurar e iniciar os elementos definidos em UI
    setWindowTitle("Ma'at"); // Título da janela


    // Configuração da barra de menu para Linux
    ui->menubar->setNativeMenuBar(false);       

    // Criação da classe que cuida dos usuários
    usersHandler = new UsersHandler(this); // Criação da classe que cuida dos usuários

    // Configurando o dial
    myDial = ui->dial_elements->findChild<QDial*>("plan_dial"); // usando o dial criado no UI
    myInsertAbsolute = ui->absoluteRevolution;  
    myAnimate_progress_bar = ui->animate_progress_bar;
    myAnimate_progress_bar->setMinimum(this->sensorData.start_angle);
    myAnimate_progress_bar->setMaximum(this->sensorData.arrive_angle);
    myAnimate_progress_bar->setValue(0);
    
    if(myDial){
        configDial(myDial); // Configurando o dial
    }

    //  Engenharia tab
    EngParameters *engParameters = new EngParameters; // Criação da classe que cuida das engenharias
    configTag_engenharia();                             // Configurando a tab de engenharia

    // tab de comunuicação com o micronas
    myMicronas = new SerialMicronas(this); // Criação da classe que cuida da comunuicação com o micronas

    // Criação das classes que cuidam dos logs
    logHandler = new LogHandler(ui->general_log_screen, this);
    logServoWindow = new LogServoWindow(ui->servo_log_screen, ui->filter_servo_log,
                                        ui->clean_log_button, this);
    
    //  Thread que controla o servo e sua comunicação principal e lança as operações
    myServoThread = new QThread(this); // inicializa uma thread
    myServo = new ServoMinas("enp2s0", engParameters); // enp2s0 para linux. eth0 para windows
    myServo->moveToThread(myServoThread); // move a classe que controla o servo para a thread
    

    // botão "Selecionar sensor"
    connect(ui->sensorSelected, &QPushButton::clicked, this, &MainWindow::by_sensorSelected_action);
    // botão "Animar Dial" com simulação de operação
    connect(ui->animate_dial_button, &QPushButton::clicked, this, &MainWindow::by_animate_dial_button_action);


    // lineEdit que recebe o Valor aboluto em ângulo desejado
    connect(myInsertAbsolute, &QLineEdit::returnPressed, this, &MainWindow::insertedAngleToAchieve);
    connect(ui->readServoPosition_button,&QPushButton::clicked, this, &MainWindow::readInputsUpdate);
    // botões de limpeza e de filtro da aba de logs gerais

    originalGeneralLogContent = ui->general_log_screen->toPlainText();
    originalServoLogContent = ui->servo_log_screen->toPlainText();

    // Conectar o texto da tela de log geral com a função que atualiza o conteúdo original
    connect(ui->general_log_screen, &QTextEdit::textChanged, this, &MainWindow::updateGeneralLogContent);
    // Conectar o texto da tela de log do servo com a função que atualiza o conteúdo original
    connect(ui->servo_log_screen, &QTextEdit::textChanged, this, &MainWindow::updateServoLogContent);

    ui->filter_general_log->setPlaceholderText("Buscar no log geral...");
    // Conectar botão de limpar para apagar todo o log geral
    connect(ui->clean_all_output, &QPushButton::clicked, ui->general_log_screen, &QTextEdit::clear);
    // Filtrar conteúdo do log geral
    connect(ui->filter_general_log, &QLineEdit::textChanged, this, &MainWindow::filterGeneralLog);

    ui->filter_servo_log->setPlaceholderText("Buscar no log do servo...");
    // Conectar botão de limpar para apagar todo o log do servo
    connect(ui->clean_all_output, &QPushButton::clicked, ui->general_log_screen, &QTextEdit::clear);
    // Filtrar conteúdo do log do servo
    connect(ui->filter_servo_log, &QLineEdit::textChanged, this, &MainWindow::filterServoLog);

    // configuração de comunicação com o servo
    ui->disable_servo_button->setEnabled(false);
    ui->disable_servo_button_2->setEnabled(false);

    // Connect checkbox toggle to the MainWindow signal
    connect(ui->enable_servo_communication, &QPushButton::toggled, this, &MainWindow::servoCommunicationBox_stateChanged);

    // Cconectar as operações de log do servo com a tela de log
    connect(myServo, &ServoMinas::logMessage, logServoWindow, &LogServoWindow::appendLog);
    connect(myServo, &ServoMinas::initializationFinished, this, &MainWindow::servoState);

    // conexões do timer de ligar e desligar o servo:
    connect(ui->init_servo_button, &QPushButton::clicked, this, &MainWindow::initializeServo);
    connect(ui->disable_servo_button, &QPushButton::clicked, this, &MainWindow::stopOperation);
    connect(ui->disable_servo_button_2, &QPushButton::clicked, this, &MainWindow::stopOperation);

    // Operações no servo
    connect(ui->homing_button, &QPushButton::clicked, this, &MainWindow::startHoming);
    connect(ui->clear_errors, &QPushButton::clicked, this, &MainWindow::clearServoErrors);

    // connects que viabilizam o envio dos sinais gerados pelas funções acima
    connect (this, &MainWindow::initSignal,myServo,&ServoMinas::initialize);
    connect (this, &MainWindow::stopOperationSignal,myServo,&ServoMinas::disableServo);
    connect (this, &MainWindow::moveServoToPositionSignal,myServo,&ServoMinas::moveAbsoluteTo);
    connect (this, &MainWindow::startHomingSignal,myServo,&ServoMinas::moveToHome);
    connect (this, &MainWindow::resetErrorsSignal,myServo,&ServoMinas::resetErrors);
    connect (this, &MainWindow::readInputNowSignal,myServo,&ServoMinas::readInput);
    //jogging signals
    connect (this, &MainWindow::jogSignal,myServo,&ServoMinas::moveOffset);

    // responsividade após resultado de ligar ou não o servo
    connect (myServo, &ServoMinas::initializationFinished,this,[this](bool situation){
        if(situation){
            qDebug()<< "Servo Habilitado";
            ui->init_servo_button->setEnabled(false);
            ui->disable_servo_button->setEnabled(true);
            ui->disable_servo_button_2->setEnabled(true);
            ui->enable_servo_communication->setEnabled(true);

        }
        else{
            qDebug()<< "Servo não habilitado";
            QMessageBox::information(nullptr, "Falha", "Falha ao inicializar o EtherCAT:\n");
        }

    });
    //  Atualização dos dados da tela em função da modificação do input or output do servo vindo do PDO
    connect(myServo, &ServoMinas::inputChangedSignal,this, &MainWindow::updateActualServoData);
    //  Atualização dos botẽs da tela em função do estado da operação
    connect(myServo, &ServoMinas::operationStatusSignal,this, &MainWindow::operationOnOFFBehavior);


    // Inicializar thread do servo
    myServoThread->start();

}

MainWindow::~MainWindow() // destructor
{
    if (myServoThread) { // caso a thread exista ao fechar a mainwindow
        myServoThread->quit(); // Solicita que a thread pare
        myServoThread->wait(); // Aguarda a finalização da thread
        delete myServoThread;  // Libera a memória da thread
    }
    if(myMicronas)
        myMicronas->closePort(); // Fecha a comunicação com o micronas caso exista
    delete ui;  // Libera a memória da interface gráfica
}


void MainWindow::by_animate_dial_button_action()
{

    //  Inversão feita somente na animação pois o padrão é invertido somente aqui
    if(sensorData.turn_direction == "CW")
        setDialDirection("CCW");
    else if (sensorData.turn_direction == "CCW")
        setDialDirection("CW");
    else
        QMessageBox::critical(this, "Erro", "Direção de giro do modelo inserido incorreta\nDeve ser:   'CW' ou 'CCW'");

    double min = sensorData.start_angle; // minimos e maximos locais 
    double max = sensorData.arrive_angle;

    double real_angle;      
    qDebug() << "Simulação de operação iniciada";
    for(int i=10*min;i<=10*max;i++){ // loop de 10x dos minimos e maximos

        real_angle=double(i)/10; // calculo do angulo real
        qDebug() << "|" << min << "| -> |"  << max << "|     Posição do dial é:" << real_angle ;
        myAnimate_progress_bar->setValue(real_angle);
        myDial->setValue(real_angle);
        QEventLoop loop;
        QTimer::singleShot(1, &loop, &QEventLoop::quit); // Pausa por 100ms
        loop.exec();
    }

}

// Funcionalidades gerais da janela pricipal em manipular os sensores
void MainWindow::by_sensorSelected_action()
{
    // Cria a janela de seleção se ainda não existir
    sensorWindow = new SensorSelectionWindow(this);  // Define o MainWindow como pai
    // assim que selecionar o sensor, chama a função updateSensorDependencies para dar update nos dados da tela principal
    connect(sensorWindow, &SensorSelectionWindow::sensorSelected, this, &MainWindow::updateSensorDependencies);
    sensorWindow->show();  // Mostra a janela de seleção

}

void MainWindow::on_actionAdicionar_triggered()
{
    if(myUser.type != "Administrador" && myUser.type != "Engenharia"){
        QMessageBox::information(this,"Permissão negada", "Este tipo de operação só pode ser executada pelo Administrador ou pela Engenharia");
        return;
    }
    // Caminho do arquivo CSV no diretório raiz do projeto
    QString filePath = QCoreApplication::applicationDirPath() + "/sensors.csv";

    // Garantir que o arquivo existe (criar se necessário)
    QFile file(filePath);
    if (!file.exists()) {
        if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QTextStream out(&file);
            out << "Model,start_angle,arrive_angle,turn_direction\n";  // Cabeçalho inicial
            file.close();
            qDebug() << "Arquivo sensors.csv criado com cabeçalho.";
        } else {
            QMessageBox::critical(this, "Erro", "Não foi possível criar o arquivo sensors.csv.");
            return;
        }
    }

    // Vetor para armazenar as linhas do arquivo
    QVector<QStringList> csvData;

    // Carregar o conteúdo existente do CSV (pulando o cabeçalho)
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);

        // Ignorar cabeçalho
        bool isFirstLine = true;
        while (!in.atEnd()) {
            QString line = in.readLine();
            if (isFirstLine) {
                isFirstLine = false;
                continue;  // Pule o cabeçalho
            }

            QStringList parts = line.split(",");
            if (parts.size() >= 4) {  // Verifica se a linha tem as colunas esperadas
                csvData.append(parts);
            }
        }

        file.close();
    }

    // Exibir o diálogo para adicionar sensor
    AddSensorDialog dialog(this);
    SensorData add;
    if (dialog.exec() == QDialog::Accepted) {
        add.model_name = dialog.getModelName();
        add.start_angle = dialog.getStartAngle();
        add.arrive_angle= dialog.getEndAngle();
        add.turn_direction= dialog.getRotationDirection();

        // Verificar se o modelo já existe no CSV
        for (const QStringList &row : csvData) {
            if (row[0] == add.model_name) {
                QMessageBox::critical(this, "Erro", "O modelo a ser inserido já está incluído. Remova-o para inserir novamente.");
                return;
            }
        }

        // Adicionar a nova linha ao vetor
        QStringList newRow = { add.model_name, QString::number(add.start_angle), QString::number(add.arrive_angle), add.turn_direction };
        csvData.append(newRow);

        // Ordenar as linhas em ordem alfabética pelo nome do modelo (primeira coluna)
        std::sort(csvData.begin(), csvData.end(), [](const QStringList &a, const QStringList &b) {
            return a[0].toLower() < b[0].toLower();  // Ordenação case-insensitive
        });

        // Reescrever o conteúdo ordenado no arquivo CSV
        if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QTextStream out(&file);

            // Escrever cabeçalho novamente
            out << "Model,start_angle,arrive_angle,turn_direction\n";

            // Escrever cada linha
            for (const QStringList &row : csvData) {
                out << row.join(",") << "\n";
            }

            file.close();
            QMessageBox::information(this, "Sucesso", "Modelo adicionado em ordem alfabética.");
        } else {
            QMessageBox::critical(this, "Erro", "Não foi possível abrir o arquivo CSV para escrita.");
        }
    }
}

void MainWindow::on_actionAtualizar_triggered()
{
    if(myUser.type != "Administrador" && myUser.type != "Engenharia"){
        QMessageBox::information(this,"Permiss o negada", "Este tipo de opera o s  pode ser executada pelo Administrador ou pela Engenharia");
        return;
    }
    // Criar e mostrar janela de sele o de sensores
    SensorSelectionWindow *selectionWindow = new SensorSelectionWindow(this);
    connect(selectionWindow, &SensorSelectionWindow::sensorSelected, this, [this](SensorData *selectedSensor) {
        if (selectedSensor) {
            // Criar e mostrar di logo de atualiza o com dados do sensor selecionado
            UpdateSensorDialog updateDialog(*selectedSensor, this);
            if (updateDialog.exec() == QDialog::Accepted) {
                // Ler o arquivo CSV
                QFile file("sensors.csv");
                if (!file.open(QIODevice::ReadWrite | QIODevice::Text)) {
                    QMessageBox::critical(this, "Erro", "N o foi poss vel abrir o arquivo sensors.csv");
                    return;
                }

                // Ler todas as linhas
                QStringList lines;
                QTextStream in(&file);
                QString header = in.readLine(); // Salvar cabe alho
                lines.append(header);

                while (!in.atEnd()) {
                    QString line = in.readLine();
                    QStringList fields = line.split(',');
                    if (fields.size() >= 4 && fields[0] == selectedSensor->model_name) {
                        // Substituir essa linha com dados atualizados
                        line = QString("%1,%2,%3,%4")
                                   .arg(updateDialog.getModelName())
                                   .arg(updateDialog.getStartAngle())
                                   .arg(updateDialog.getEndAngle())
                                   .arg(updateDialog.getRotationDirection());
                    }
                    lines.append(line);
                }

                // Escrever de volta no arquivo
                file.resize(0);
                QTextStream out(&file);
                for (const QString &line : lines) {
                    out << line << "\n";
                }

                file.close();
                QMessageBox::information(this, "Sucesso", "Sensor atualizado com sucesso!");
            }
        }
    });
    selectionWindow->show();
}

void MainWindow::on_actionRemover_triggered()
{
    if(myUser.type != "Administrador"){
        QMessageBox::information(this,"Permissão negada", "Este tipo de operação só pode ser executada pelo Administrador");
        return;
    }
    // Caminho do arquivo CSV no diretório raiz do projeto
    QString filePath = QCoreApplication::applicationDirPath() + "/sensors.csv";

    QFile file(filePath);
    if (!file.exists()) {
        QMessageBox::critical(this, "Erro", "O arquivo sensors.csv não existe.");
        return;
    }

    // Vetor para armazenar as linhas do arquivo
    QVector<QStringList> csvData;
    QStringList sensorNames;

    // Carregar o conteúdo existente do CSV (pulando o cabeçalho)
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);

        bool isFirstLine = true;
        while (!in.atEnd()) {
            QString line = in.readLine();
            if (isFirstLine) {
                isFirstLine = false;
                continue;  // Pule o cabeçalho
            }

            QStringList parts = line.split(",");
            if (parts.size() >= 4) {  // Verifica se a linha tem as colunas esperadas
                csvData.append(parts);
                sensorNames.append(parts[0]);  // Adiciona o nome do modelo
            }
        }

        file.close();
    }

    if (sensorNames.isEmpty()) {
        QMessageBox::information(this, "Informação", "Não há sensores para remover.");
        return;
    }

    // Abrir o diálogo para remover sensores
    RemoveSensorDialog dialog(this);
    dialog.setSensorList(sensorNames);

    if (dialog.exec() == QDialog::Accepted) {
        QString selectedSensor = dialog.getSelectedSensor();

        // Remover o sensor selecionado
        auto it = std::remove_if(csvData.begin(), csvData.end(), [&](const QStringList &row) {
            return row[0] == selectedSensor;
        });

        if (it != csvData.end()) {
            csvData.erase(it, csvData.end());
        }

        // Reescrever o conteúdo no arquivo CSV
        if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QTextStream out(&file);

            // Escrever cabeçalho novamente
            out << "Modelo,Ângulo Partida,Ângulo Chegada,Sentido de Giro\n";

            // Escrever cada linha restante
            for (const QStringList &row : csvData) {
                out << row.join(",") << "\n";
            }

            file.close();
            QMessageBox::information(this, "Sucesso", "Sensor removido com sucesso.");
        } else {
            QMessageBox::critical(this, "Erro", "Não foi possível abrir o arquivo CSV para escrita.");
        }
    }
}

void MainWindow::on_actionVer_Sensores_triggered()
{
    // Caminho do arquivo CSV
    QString filePath = QCoreApplication::applicationDirPath() + "/sensors.csv";

    QFile file(filePath);
    if (!file.exists()) {
        QMessageBox::critical(this, "Erro", "O arquivo sensors.csv não existe.");
        return;
    }

    // Abrir a janela de visualização
    ViewSensorsDialog dialog(filePath, this);
    dialog.exec();  // Exibir como diálogo modal
}

// configurar o sentido do dial com base no valor de direction_set
void MainWindow::setDialDirection(QString direction_set){ 
    if (direction_set == "CW"){
        myDial->setInvertedAppearance(true);
    }
    else if (direction_set == "CCW"){
        myDial->setInvertedAppearance(false);
    }
    else
        QMessageBox::critical(this, "Erro", "Direção de giro do modelo inserido incorreta\nDeve ser:   'CW' ou 'CCW'");

}

//dar update nos widgets da mainWindow que dependem dos dados do sensor
void MainWindow::updateSensorDependencies(SensorData *_sensorData) {

    // Atualizar dados do sensor mostrado
    setSensorData(*_sensorData);
    ui->sensorSelected->setText(this->sensorData.model_name);
    ui->label_angle_start->setText(QString::number(this->sensorData.start_angle));
    ui->label_angle_end->setText(QString::number(this->sensorData.arrive_angle));
    ui->label_turn_direction->setText(this->sensorData.turn_direction);
    myAnimate_progress_bar->setMinimum(this->sensorData.start_angle);
    myAnimate_progress_bar->setMaximum(this->sensorData.arrive_angle);

    qDebug() << "\n\nSistema configurado e atualizado ao modelo:";
    qDebug() << "Model:" << sensorData.model_name;
    qDebug() << "Start Angle:" << sensorData.start_angle;
    qDebug() << "Arrive Angle:" << sensorData.arrive_angle;
    qDebug() << "Turn Direction:" << sensorData.turn_direction;

    // Aba de operação habilitada
    ui->prepare_operation->setEnabled(true);
}

// update do log geral
void MainWindow::updateGeneralLogContent()
{
    originalGeneralLogContent = ui->general_log_screen->toPlainText();
}

// atualizar o log servo
void MainWindow::updateServoLogContent()
{
    originalServoLogContent = ui->servo_log_screen->toPlainText();
}

// Filtrar o log geral
void MainWindow::filterGeneralLog(const QString &text)
{
    QString originalText = ui->general_log_screen->toPlainText(); // Armazena o texto ORIGINAL
    QStringList linhas = originalText.split("\n"); // Separa o texto ORIGINAL em linhas
    QStringList linhasFiltradas;

    if (text.isEmpty()) { // Se o texto de filtro estiver vazio, mostrar todas as linhas
        ui->general_log_screen->clear();
        ui->general_log_screen->append(originalText); // Restaura o conteúdo ORIGINAL
        return; // Sai da fun o cedo
    }

    for (const QString &linha : linhas) {
        if (linha.contains(text, Qt::CaseInsensitive)) { // Usa o texto de filtro recebido
            linhasFiltradas.append(linha); // Mantém a linha se ela contiver o texto de filtro
        }
    }

    ui->general_log_screen->clear(); // Limpa a área de texto
    ui->general_log_screen->append(linhasFiltradas.join("\n")); // Mostra as linhas filtradas
}

// Filtrar o log servo
void MainWindow::filterServoLog(const QString &text)
{
    QString originalText = ui->servo_log_screen->toPlainText(); // Armazena o texto ORIGINAL
    QStringList linhas = originalText.split("\n"); // Separa o texto ORIGINAL em linhas
    QStringList linhasFiltradas;

    if (text.isEmpty()) { // Se o texto de filtro estiver vazio, mostrar todas as linhas
        ui->servo_log_screen->clear();
        ui->servo_log_screen->append(originalText); // Restaura o conteúdo ORIGINAL
        return; // Sai da fun o cedo
    }

    for (const QString &linha : linhas) {
        if (linha.contains(text, Qt::CaseInsensitive)) { // Usa o texto de filtro recebido
            linhasFiltradas.append(linha); // Mantém a linha se ela contiver o texto de filtro
        }
    }

    ui->servo_log_screen->clear(); // Limpa a área de texto
    ui->servo_log_screen->append(linhasFiltradas.join("\n")); // Mostra as linhas filtradas
}

// Configura es do comportamento do dial reativo

void MainWindow::configDial(QDial *_myDial){
    _myDial->setInvertedAppearance(false);
    _myDial->setMinimum(sensorData.start_angle);
    _myDial->setMaximum(sensorData.arrive_angle);
    _myDial->setNotchesVisible(true);
    _myDial->setNotchTarget(3.7);
    _myDial->setWrapping(true);
    _myDial->setSingleStep(1);
    _myDial->setValue(this->sensorData.start_angle);
    _myDial->setStyleSheet(
        "QDial {"
        "background-color: gray;"
        "border: 2px solid gray;"
        "border-radius: 20px;"
        "color: blue;"
        "font: bold 12px;"
        "subcontrol-origin: red"
        "}"
        );

}

// Login button ativo -> credenciar o usuário
void MainWindow::on_login_menuBar_triggered()
{
    myUser = usersHandler->loginAccess();
    ui->permission_label->setText(myUser.type);
    ui->user_label->setText(myUser.username);
}
// Logout button ativo -> sair do usuário
void MainWindow::on_actionLogout_triggered()
{
    myUser.username = "Comum";
    myUser.type     = "Produção";
    ui->permission_label->setText(myUser.type);
    ui->user_label->setText(myUser.username);
}


// CRUD de usuários:
void MainWindow::on_actionVer_usu_rios_triggered()
{
    usersHandler->showViewUsersDialog(myUser);
}

void MainWindow::on_actionAdicionar_2_triggered()
{
    if(myUser.type == "Administrador")
        usersHandler->showAddUserDialog(myUser);
    else
        QMessageBox::information(this,"Permissão negada", "Este tipo de operação só pode ser executada pelo Administrador");
}

void MainWindow::on_actionRemover_2_triggered()
{
    if(myUser.type == "Administrador")
        usersHandler->showRemoveUserDialog();
    else
        QMessageBox::information(this,"Permissão negada", "Este tipo de operação só pode ser executada pelo Administrador");

}

void MainWindow::on_actionAtualizar_2_triggered()
{
    if(myUser.username == "Comum"){
        QMessageBox::information(this,"Permissão negada", "Para atualizar, faça login!");

    }
    else{
        usersHandler->showUpdateUserDialog(myUser);
    }
}


//  Funções de comunicação ao servo
void MainWindow::servoState(bool servoSituation)
{
    servoUP=servoSituation; // se o servo foi conectado e o bind do socket completo
    if(servoUP)
        qDebug() << " -> Servo conectado e pronto para inicializar operações\n";
    else
        qDebug() << " -> Servo não está conectado\n";
}

// Função para o controle dos widgets da mainWindow em função do estad do botão de comunicaçãos
void MainWindow::servoCommunicationBox_stateChanged(bool toogled)
{

    ui->animate_dial_button->setEnabled(!toogled);
    ui->readServoPosition_button->setEnabled(toogled);
    if(servoUP)
    {
        myServo->updateCommunicationState(toogled);
    }
    else{
        QMessageBox::critical(this, "Erro", "Servo não conectado");
        ui->enable_servo_communication->setEnabled(false);
        ui->animate_dial_button->setEnabled(toogled);
    }
    if (toogled)
        ui->actualOperationMode_label->setText("OPERAÇÃO");
    else{
        ui->actualOperationMode_label->setText("SIMULAÇÃO");
        ui->servo_angle_position->setText("Ângulo");
        ui->servo_hex_position->setText("Posição em hexa");
    }
}

//inicializar o servo e enviar sinal para iniciar a thread do servo
void MainWindow::initializeServo() { emit initSignal(); }

//parar a operação e enviar sinal para parar a thread do servo
void MainWindow::stopOperation()
{

    emit stopOperationSignal(); // Desabilitar o
    // ui->clear_errors->setEnabled(true);
    qDebug()<< "Operação parada";
}

//inicializar o homing e enviar sinal para iniciar a a sub thread de operação do servo
void MainWindow::startHoming()
{
    emit startHomingSignal();// inicializar homing
    qDebug() << "inicializar o homing";
}

// função de movimentação do servo absoluta para a posição desejada em graus
void MainWindow::insertedAngleToAchieve(){
    setDialDirection("CCW"); // direção para movimento absoluto
    bool ok, ok_velocity=1;
    double insertedValue = myInsertAbsolute->text().toDouble(&ok);
    qDebug() << "Valor alterado para:" << insertedValue;
    int velocity = 50; // valor padrão
    int velocity_virtualLimit = 551; // parametro que deve mudar segundo a configuração da engenharia ISSO AQUI DEVE SER ALTERADO
    if(!ui->servo_velocity_setup->text().isEmpty()) // se ouver algo dentro da celular de velocidade, irá usar o valor de la
    {
        velocity = ui->servo_velocity_setup->text().toDouble(&ok_velocity);
        if(velocity <= 0 || velocity > velocity_virtualLimit)
        {
            velocity = 50;
            QMessageBox::critical(this, "Erro", QString("Insira o dado de velocidade até %1 rad/s").arg(velocity_virtualLimit));
        }


    }
    if(!ok_velocity){
        QMessageBox::critical(this, "Erro", "Insira o dado de velocidade corretamente: valor flutuante ");

    }
    else if(ok){
        emit moveServoToPositionSignal(insertedValue,velocity);
    }
    else{
        QMessageBox::critical(this, "Erro", "Insira o dado flutuante corretamente entre os limites do modelo");
    }

}

// Posicionar o servo a posição inicial dependendo do sensor
void MainWindow::on_prepare_operation_released()
{

    int velocity = 50; // valor padrão
    bool ok=1,ok_velocity=1;
    int velocity_virtualLimit = 551; // parametro que deve mudar segundo a configuração da engenharia
    if(!ui->servo_velocity_setup->text().isEmpty()) // se ouver algo dentro da celular de velocidade, irá usar o valor de la
    {
        velocity = ui->servo_velocity_setup->text().toDouble(&ok_velocity);
        if(velocity<=0 || velocity >velocity_virtualLimit)
        {       velocity=50;

            QMessageBox::critical(this, "Erro", QString("Insira o dado de velocidade até %1 rad/s").arg(velocity_virtualLimit));
    }
    }
    if(!ok_velocity){
        QMessageBox::critical(this, "Erro", "Insira o dado de velocidade corretamente: valor flutuante ");

    }
    if(ok){
        if(sensorData.turn_direction== "CW"){
            setDialDirection("CCW");
            emit moveServoToPositionSignal(-sensorData.start_angle,velocity);

        }
        else if(sensorData.turn_direction== "CCW"){
            setDialDirection("CCW");
            emit moveServoToPositionSignal(sensorData.start_angle,velocity);
        }
        else{
            QMessageBox::critical(this, "Erro", "Direção de giro do modelo inserido incorreta\nDeve ser:   'CW' ou 'CCW'");
            return;
        }
        qDebug() << "\n -> Posicionando o servo para o ângulo X, início do modelo Y\n";
    }
    else{
        QMessageBox::critical(this, "Erro", "Insira o dado flutuante corretamente para a velocidade (0 - 550 rad/s)");
    }
    qDebug() << "\n -> Operação iniciada do com o modelo "<< sensorData.model_name<<", ângulo "<< sensorData.start_angle<<" ao ângulo "<< sensorData.arrive_angle<<" no sentido "<< sensorData.turn_direction<<"\n";

}

// Posicionar o servo a posição final dependendo do sensor
void MainWindow::on_start_operation_released()
{
    int velocity = 50; // valor padrão
    bool ok=1,ok_velocity=1;
    int velocity_virtualLimit = 551; // parametro que deve mudar segundo a configuração da engenharia
    if(!ui->servo_velocity_setup->text().isEmpty()) // se ouver algo dentro da celular de velocidade, irá usar o valor de la
    {
        velocity = ui->servo_velocity_setup->text().toDouble(&ok_velocity);
        if(velocity<=0 || velocity >velocity_virtualLimit)
        {   velocity=50;
            QMessageBox::critical(this, "Erro", QString("Insira o dado de velocidade até %1 rad/s").arg(velocity_virtualLimit));
        }
    }
    if(!ok_velocity){
        QMessageBox::critical(this, "Erro", "Insira o dado de velocidade corretamente: valor flutuante ");

    }
    if(ok){
        if(sensorData.turn_direction== "CW"){
            setDialDirection("CCW");
            emit moveServoToPositionSignal(-sensorData.arrive_angle,velocity);

        }
        else if(sensorData.turn_direction== "CCW"){
            setDialDirection("CCW");
            emit moveServoToPositionSignal(sensorData.arrive_angle,velocity);
        }
        else{
            QMessageBox::critical(this, "Erro", "Direção de giro do modelo inserido incorreta\nDeve ser:   'CW' ou 'CCW'");
            return;
        }
        qDebug() << "\n -> Posicionando o servo para o ângulo X, início do modelo Y\n";
    }
    else{
        QMessageBox::critical(this, "Erro", "Insira o dado flutuante corretamente para a velocidade (0 - 550 rad/s)");
    }
}

// limpar os erros do servo
void MainWindow::clearServoErrors()
{
    emit resetErrorsSignal();
}

// Operação Jog - Anti Horário CCW
void MainWindow::on_left_jog_released()
{
    setDialDirection("CCW"); // direção para movimento absoluto
    bool ok_amount,ok_step,ok_velocity=1;
    double amount= ui->amountJog_lineEdit->text().toDouble(&ok_amount);
    double step = ui->step_lineEdit->text().toDouble(&ok_step);
    int velocity = 50; // valor padrão
    int velocity_virtualLimit = 551; // parametro que deve mudar segundo a configuração da engenharia
    if(!ui->servo_velocity_setup->text().isEmpty()) // se ouver algo dentro da celular de velocidade, irá usar o valor de la
    {
        velocity = ui->servo_velocity_setup->text().toDouble(&ok_velocity);
        if(velocity<=0 || velocity >velocity_virtualLimit)
        {
            velocity=50;
            QMessageBox::critical(this, "Erro", QString("Insira o dado de velocidade até %1 rad/s").arg(velocity_virtualLimit));
        }

    }
    if(!ok_velocity){
        QMessageBox::critical(this, "Erro", "Insira o dado de velocidade corretamente: valor flutuante ");

    }
    else if(ok_amount && ok_step){
        qDebug() << "Jog de offset CCW em:" << amount*step <<"º";
        emit jogSignal(amount,velocity,step);
    }
    else{
        QMessageBox::critical(this, "Erro", "Insira o dado flutuante em step, amount");
        return;
    }

}

// Operação Jog - Horário CW
void MainWindow::on_right_jog_released()
{
    setDialDirection("CCW"); // direção para movimento absoluto
    bool ok_amount,ok_step,ok_velocity=1;
    double amount= ui->amountJog_lineEdit->text().toDouble(&ok_amount);
    double step = ui->step_lineEdit->text().toDouble(&ok_step);
    int velocity = 50; // valor padrão
    int velocity_virtualLimit = 551; // parametro que deve mudar segundo a configuração da engenharia
    if(!ui->servo_velocity_setup->text().isEmpty()) // se ouver algo dentro da celular de velocidade, irá usar o valor de la
    {
        velocity = ui->servo_velocity_setup->text().toDouble(&ok_velocity);
        if(velocity<=0 || velocity >velocity_virtualLimit)
        {
            velocity=50;
            QMessageBox::critical(this, "Erro", QString("Insira o dado de velocidade até %1 rad/s").arg(velocity_virtualLimit));
        }
    }
    if(!ok_velocity){
        QMessageBox::critical(this, "Erro", "Insira o dado de velocidade corretamente: valor flutuante ");

    }
    else if(ok_amount && ok_step && ok_velocity){
        qDebug() << "Jog de offset CW em:" << amount*step << "º";
        emit jogSignal(-amount,velocity,step);
    }
    else{
        QMessageBox::critical(this, "Erro", "Insira o dado flutuante em step e amount");
        return;
    }
}


void MainWindow::setSensorData(SensorData _data)
{
    this->sensorData = _data;
}

//update mainwindow com informações vindas do servo
double castoTo360(double value){
    // Use modulo operator for efficiency
    double result = fmod(fabs(value), 360.0);
    return (value >= 0)? result: -result;
}
// atualizar a tela com os dados do servo atual (input)
void MainWindow::updateActualServoData(minas_control::MinasInput input){
    uint32_t raw_servo_value = input.position_actual_value; // valor absoluto unsigned int 32
    int32_t signed_angle = static_cast<int32_t>(raw_servo_value); // valor com sinal de 32 bits

    // corrigir o valor negativo com tratamento em hexa uint
    double absolute_angle = (signed_angle >= 0)?
                                (360.0 * static_cast<double>(signed_angle) / 0x800000):
                                (-360.0 * static_cast<double>(-signed_angle) / 0x800000);

    double mydial_value = castoTo360(absolute_angle); // corrigir voltar > |+-360|
    myDial->setValue(-mydial_value); // dar update no dial
    qDebug() << "| Absolute angle:" << absolute_angle; // mostrar a evolução do angulo ao ser lido
    //  Atualizar a tela
    ui->servo_hex_position->setText( QString::number(signed_angle,16).toUpper()+"h");
    ui->servo_angle_position->setText(QString::number(absolute_angle,'f',4)+"º");
    //  Atualizar a barra de progresso
    if(sensorData.turn_direction=="CW")
        myAnimate_progress_bar->setValue(-absolute_angle);
    if(sensorData.turn_direction=="CCW")
        myAnimate_progress_bar->setValue(absolute_angle);
}
// função para chamar as leituras dos inputs do servo
void MainWindow::readInputsUpdate(){emit readInputNowSignal();}

// Função para bloquear ou desbloquear widgets em função do botão de operação
void MainWindow::operationOnOFFBehavior(bool status){
    // status true: em operação
    // status false: não está em operação

    //A ideia é bloquear o usuário de dar overload de requisições ao drive
    ui->homing_button->setEnabled(!status);
    myInsertAbsolute->setEnabled(!status);
    ui->right_jog->setEnabled(!status);
    ui->left_jog->setEnabled(!status);
    ui->prepare_operation->setEnabled(!status);
    ui->start_operation->setEnabled(!status);
    ui->clear_errors->setEnabled(!status);

}


//  -------------- setters e getters para os dados da main --------------


QString MainWindow::getModelName()
{
    return this->sensorData.model_name ;
}

double MainWindow::getStartAngle()
{
    return this->sensorData.start_angle;
}

double MainWindow::getArriveAngle()
{
    return this->sensorData.arrive_angle;
}

QString MainWindow::getTurnDirection()
{
    return this->sensorData.turn_direction ;
}


// -- -- -- -- área da engenharia -- -- -- -- 

//função auxiliar para incrementar os itens recursivamente
void addJsonToTree(const QJsonObject &jsonObj, QStandardItem *parentItem) {
    for (const QString &key : jsonObj.keys()) {
        QJsonValue value = jsonObj.value(key);
        QStandardItem *categoryItem = new QStandardItem(key);
        categoryItem->setFlags(Qt::ItemIsEnabled);
        parentItem->appendRow(categoryItem);

        if (value.isObject()) {
            addJsonToTree(value.toObject(), categoryItem);
        } else {
            QStandardItem *valueItem = new QStandardItem(value.toVariant().toString());
            valueItem->setFlags(Qt::ItemIsEditable); // Permitir edição
            categoryItem->appendRow(valueItem);
        }
    }
}

void MainWindow::configTag_engenharia() {
    // JSON com as configurações do ServoMinas
    QString jsonString = R"(
    {
      "ServoMinas": {
        "LimitesDeSeguranca": {
          "TorqueDeParadaEmergencia": 100,
          "NivelDeOverload": 70,
          "NivelDeOverspeed": 600,
          "LimiteDeTrabalhoDoMotor": 0.1,
          "PeriodoDeInterpolacaoDeTempo": 4000,
          "Velocidade": "0x16000000",
          "Aceleracao": "0x80000000",
          "Desaceleracao": "0x80000000"
        },
        "Homing": {
          "ProbeFunction": 8000000,
          "SetSwitchSpeed": 8000000,
          "HomingAcceleration": 33554432,
          "HomingTorqueLimit": 500,
          "HomingDetectionTime": 2048,
          "HomingDetectionVelocity": 33554432,
          "CommunicationFunctionExtendedSetup": 40,
          "HomingReturnSpeedLimit": 20000,
          "HomingMode": 34,
          "TouchProbe": 7
        },
        "Posicionamento": {
          "TorqueMaximo": 500,
          "TargetTorque": 500
        }
      }
    })";

    // Converter a string JSON para um objeto JSON
    QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonString.toUtf8());
    if (jsonDoc.isNull() || !jsonDoc.isObject()) {
        qDebug() << "Erro ao carregar JSON!";
        return;
    }

    QJsonObject rootObj = jsonDoc.object();

    // Criar o modelo de árvore
    model_tree = new QStandardItemModel(this);
    QStandardItem *rootItem = new QStandardItem("Servo Minas");
    rootItem->setFlags(Qt::ItemIsEnabled);
    model_tree->appendRow(rootItem);

    // Adicionar os dados do JSON na árvore usando a função auxiliar
    addJsonToTree(rootObj["ServoMinas"].toObject(), rootItem);

    // Configurar a QTreeView
    myParameters_TreeView = ui->treeView_engenharia;
    myParameters_TreeView->setModel(model_tree);
    myParameters_TreeView->setEditTriggers(QAbstractItemView::NoEditTriggers); // Inicialmente desativado


    // Conectar a checkbox ao evento para habilitar/desabilitar edição
    connect(ui->engineeringEdit_checkBox, &QCheckBox::toggled, this, &MainWindow::toggleTreeEditability);
}

void MainWindow::toggleTreeEditability(bool checked) {
    qDebug() << "Checkbox ativada: " << checked;

    for (int i = 0; i < model_tree->rowCount(); ++i) {
        QStandardItem *categoryItem = model_tree->item(i); // Ex: "Limites de Segurança", "Homing", etc.
        if (!categoryItem) continue;

        for (int j = 0; j < categoryItem->rowCount(); ++j) {
            QStandardItem *subCategoryItem = categoryItem->child(j); // Ex: "Torque de parada de emergência"
            if (!subCategoryItem) continue;

            // O terceiro nível é onde os valores numéricos ficam
            for (int k = 0; k < subCategoryItem->rowCount(); ++k) {
                QStandardItem *valueItem = subCategoryItem->child(k); // Ex: "100", "500", etc.
                if (valueItem && valueItem->rowCount() == 0) { // Apenas nós folha
                    valueItem->setFlags(checked
                                            ? (Qt::ItemIsEditable | Qt::ItemIsEnabled | Qt::ItemIsSelectable)
                                            : (Qt::ItemIsEnabled | Qt::ItemIsSelectable));
                }
            }
        }
    }

    // Atualizar a QTreeView para refletir a edição
    QModelIndex topLeft = model_tree->index(0, 0);
    QModelIndex bottomRight = model_tree->index(model_tree->rowCount() - 1, 0);
    emit model_tree->dataChanged(topLeft, bottomRight);

    // Certificar-se de que os valores sejam editáveis com duplo clique
    myParameters_TreeView->setEditTriggers(QAbstractItemView::DoubleClicked);
    myParameters_TreeView->update();
}

// apaga
// crc function original da documentação. foi traga aqui só pra teste local sem antes msm ligar a USB
int calculateIntCrc(int data, int size)
{
    const unsigned char CRC_POLY = 0x13; // x^4 + x + 1
    unsigned char crc = 0x0;
    for (int i=0; i<size; i++)
    {
        if ((crc<<3 & 0x1) != (data >> (size - 1 - i) & 1))
        {
            crc = crc<<1;
            crc ^= CRC_POLY;
            crc &= 0xF;
        }
        else
            crc <<= 1;
    }
    return crc&0xF;
}

// inicializar comunicação com o micronas
void MainWindow::on_micronas_connect_released()
{

    // const unsigned char CRC_POLY = 0x13;
    // unsigned char crc = 0;
    // int totalBits = 10 * 8; // total number of bits -> 10 caracteres * 8
    // QString baseCommand = "xxsb000001";    // 78 78 73 62 30 30 30 30 30 31

    // QByteArray full_command = baseCommand.toUtf8();
    // // Process each bit (MSB first for each byte)
    // for (int i = 0; i < totalBits; i++) {
    //     // Calculate the bit position: i/8 gives the byte index,
    //     // (7 - (i % 8)) gives the bit index within that byte (MSB first)
    //     bool bit = (full_command.at(i / 8) >> (7 - (i % 8))) & 0x1;
    //     // Compare the MSB (bit 3) of crc with the current data bit.
    //     if (((crc >> 3) & 0x1) != bit) {
    //         crc = (crc << 1) ^ CRC_POLY;
    //     } else {
    //         crc <<= 1;
    //     }
    //     crc &= 0xF; // keep crc to 4 bits
    // }
    // qDebug()<< "Para " <<baseCommand <<"| crc: " << crc;

    // uint32_t value = 0;
    // int command_size = full_command.size();
    // for (int i = 0; i < command_size; ++i) {
    //     value |= full_command.at(i) << (i * 8);
    // }
    // int crc2 = calculateIntCrc(value,command_size);
    // qDebug() <<"int value : "<< value << QString::number(value,16).toUpper();
    // qDebug() <<"(CRC2):     "<< crc2  << QString::number(crc2 ,16).toUpper();

    // return;  // parada para teste aqui


    myMicronas = new SerialMicronas(this);
    connect(myMicronas,&SerialMicronas::errorOccurred,this,&MainWindow::errorFromMicronas);
    connect(myMicronas,&SerialMicronas::messageMicronas_signal,this,&MainWindow::messageFromMicronas);

    QString portName = ui->micronasUSBPort_lineEdit->text().trimmed();

    // Verificar se o campo foi preenchido
    if (!portName.isEmpty()) {
        // Expressão regular para validar "ttyUSBX" (onde X é um número)
        QRegularExpression regex("^ttyUSB\\d+$");

        if (!regex.match(portName).hasMatch()) {
            qCritical() << "Erro: O nome da porta deve estar no formato 'ttyUSBX', onde X é um número.";
            return;
        }
    } else {
        // Se o campo estiver vazio, usa "ttyUSB0" como padrão
        portName = "ttyUSB0";
    }
    if (!myMicronas->openPort("/dev/" + portName)) {
        qCritical() << "Falha ao abrir a porta serial.";
        myMicronas->closePort();
        return;
    }

    // versão do firmware
    QString firmwareVersion = myMicronas->getFirmwareVersion();
    qDebug() << "Versão do firmware é: "<< firmwareVersion;

    // configurar a base:
    if(!myMicronas->setBoardMode('C')){
        qCritical() << "Falha ao abrir ao configurar modo de placa.";
        myMicronas->closePort();
        return;
    }

    uint8_t baseAddress = 0x00;
    uint16_t dataBase = 0x0001;
    myMicronas->setBaseAddress(baseAddress, dataBase);
    qDebug() << "baseAddress: "<< baseAddress;

    uint8_t registerAddress = 0x01;
    QString readData = myMicronas->readAddress(registerAddress);
    qDebug() << "registador: "<<registerAddress<<" -> tem tal informação: "<<readData;
}

void MainWindow::errorFromMicronas(const QString &errorMessage)
{
    qCritical() << "Micronas error: " + errorMessage;
}
void MainWindow::messageFromMicronas(const QString &message)
{
    qDebug() << "Micronas: " + message;
}

