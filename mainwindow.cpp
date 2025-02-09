#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , sensorWindow(nullptr)  // Inicializa o ponteiro como nullptr
    , sensorData{"Modelo não selecionado",0.0,360.0,"CW"}
    , actual_servo_value(0.0)
    , myServo(nullptr)        // ponteiro para a classe que controla o servo
    , logServoWindow(nullptr) // ponteiro para a janela de log dentro da mainWindow
    , servoUP(false)          // bool que comunica o estado do servo (se está apto a ser usado)
    , myUser{"Usuário comum","Produção"}
{

    ui->setupUi(this); // configurar e iniciar os elementos definidos em UI
    setWindowTitle("Ma'at"); // Título da janela


    // Configuração da barra de menu para Linux
    ui->menubar->setNativeMenuBar(false);

    // Criação da classe que cuida dos usuários
    usersHandler = new UsersHandler(this);

    myDial = ui->dial_elements->findChild<QDial*>("plan_dial"); // usando o dial criado no UI
    myInsertAbsolute = ui->absoluteRevolution;
    myAnimate_progress_bar = ui->animate_progress_bar;
    myAnimate_progress_bar->setMinimum(this->sensorData.start_angle);
    myAnimate_progress_bar->setMaximum(this->sensorData.arrive_angle);
    myAnimate_progress_bar->setValue(0);

    if(myDial){
        configDial(myDial);
    }

    logHandler = new LogHandler(ui->general_log_screen, this);
    logServoWindow = new LogServoWindow(ui->servo_log_screen, ui->filter_servo_log,
                                        ui->clean_log_button, this);
    //  ServoMinas deploy
    myServoThread = new QThread(this); // inicializa uma thread
    myServo = new ServoMinas("enp2s0"); // enp2s0 para linux. eth0 para windows
    myServo->moveToThread(myServoThread);
    // seção das conexões
    // Selecionar sensor
    connect(ui->sensorSelected, &QPushButton::clicked, this, &MainWindow::by_sensorSelected_action);
    // Animar Dial com simulação de operação
    connect(ui->animate_dial_button, &QPushButton::clicked, this, &MainWindow::by_animate_dial_button_action);


    // Valor aboluto em ângulo desejado
    connect(myInsertAbsolute, &QLineEdit::returnPressed, this, &MainWindow::insertedAngleToAchieve);
    connect(ui->readServoPosition_button,&QPushButton::clicked, this, &MainWindow::readInputsUpdate);
    {// botões de limpeza e de filtro da aba de logs gerais

        originalGeneralLogContent = ui->general_log_screen->toPlainText();
        originalServoLogContent = ui->servo_log_screen->toPlainText();

        connect(ui->general_log_screen, &QTextEdit::textChanged, this, &MainWindow::updateGeneralLogContent);
        connect(ui->servo_log_screen, &QTextEdit::textChanged, this, &MainWindow::updateServoLogContent);

        ui->filter_general_log->setPlaceholderText("Buscar no log geral...");
        // Conectar botão de limpar para apagar todo o log geral
        connect(ui->clean_all_output, &QPushButton::clicked, ui->general_log_screen, &QTextEdit::clear);
        // filtrar conteúdo do log geral
        connect(ui->filter_general_log, &QLineEdit::textChanged, this, &MainWindow::filterGeneralLog);

        ui->filter_servo_log->setPlaceholderText("Buscar no log do servo...");
        // Conectar botão de limpar para apagar todo o log do servo
        connect(ui->clean_all_output, &QPushButton::clicked, ui->general_log_screen, &QTextEdit::clear);
        // filtrar conteúdo do log do servo
        connect(ui->filter_servo_log, &QLineEdit::textChanged, this, &MainWindow::filterServoLog);
    }

    // connect(myDial, &QDial::sliderReleased, this, [this]() {
    //     double min = sensorData.start_angle ;
    //     double max = sensorData.arrive_angle ;
    //     double value = myDial->value();
    //     qDebug() << "valor atual é:" << value;
    //     if(value < min)
    //         myDial->setValue(min);
    //     else if( value> max)
    //         myDial->setValue(max);

    // });


    // servo communication setup
    ui->disable_servo_button->setEnabled(false);
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


    //  Atualização dos dados da tela em função da modificação do input or output do servo vindo do PDO
    connect(myServo, &ServoMinas::inputChangedSignal,this, &MainWindow::updateActualServoData);
    //  Atualização dos botẽs da tela em função do estado da operação
    connect(myServo, &ServoMinas::operationStatusSignal,this, &MainWindow::operationOnOFFBehavior);


    myServoThread->start(); // Inicializar thread do servo

}

MainWindow::~MainWindow()
{
    if (myServoThread) { // caso a thread exista ao fechar a mainwindow
        myServoThread->quit(); // Solicita que a thread pare
        myServoThread->wait(); // Aguarda a finalização da thread
        delete myServoThread;  // Libera a memória da thread
    }
    delete ui;
}


void MainWindow::by_animate_dial_button_action()
{
    double min = sensorData.start_angle;
    double max= sensorData.arrive_angle;

    double real_angle;
    qDebug() << "Simulação de operação iniciada";
    for(int i=10*min;i<=10*max;i++){

        real_angle=double(i)/10;
        qDebug() << "|" << min << "| -> |"  << max << "|     Posição do dial é:" << real_angle ;
        this->setServoAngularPosition(real_angle,500);
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
        QMessageBox::information(this,"Permissão negada", "Este tipo de operação só pode ser executada pelo Administrador ou pela Engenharia");
        return;
    }
    // Create and show sensor selection window
    SensorSelectionWindow *selectionWindow = new SensorSelectionWindow(this);
    connect(selectionWindow, &SensorSelectionWindow::sensorSelected, this, [this](SensorData *selectedSensor) {
        if (selectedSensor) {
            // Create and show update dialog with current sensor data
            UpdateSensorDialog updateDialog(*selectedSensor, this);
            if (updateDialog.exec() == QDialog::Accepted) {
                // Read the CSV file
                QFile file("sensors.csv");
                if (!file.open(QIODevice::ReadWrite | QIODevice::Text)) {
                    QMessageBox::critical(this, "Erro", "Não foi possível abrir o arquivo sensors.csv");
                    return;
                }

                // Read all lines
                QStringList lines;
                QTextStream in(&file);
                QString header = in.readLine(); // Save header
                lines.append(header);

                while (!in.atEnd()) {
                    QString line = in.readLine();
                    QStringList fields = line.split(',');
                    if (fields.size() >= 4 && fields[0] == selectedSensor->model_name) {
                        // Replace this line with updated data
                        line = QString("%1,%2,%3,%4")
                                   .arg(updateDialog.getModelName())
                                   .arg(updateDialog.getStartAngle())
                                   .arg(updateDialog.getEndAngle())
                                   .arg(updateDialog.getRotationDirection());
                    }
                    lines.append(line);
                }

                // Write back to file
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

void MainWindow::updateSensorDependencies(SensorData *_sensorData) {

    // Atualizar dados do sensor mostrado
    setSensorData(*_sensorData);
    ui->sensorSelected->setText(this->sensorData.model_name);
    ui->label_angle_start->setText(QString::number(this->sensorData.start_angle));
    ui->label_angle_end->setText(QString::number(this->sensorData.arrive_angle));
    ui->label_turn_direction->setText(this->sensorData.turn_direction);
    myAnimate_progress_bar->setMinimum(this->sensorData.start_angle);
    myAnimate_progress_bar->setMaximum(this->sensorData.arrive_angle);
    if (this->sensorData.turn_direction == "CW"){
        myDial->setInvertedAppearance(false);
    }
    else if (this->sensorData.turn_direction == "CCW"){
        myDial->setInvertedAppearance(true);
    }
    else
        QMessageBox::critical(this, "Erro", "Direção de giro do modelo inserido incorreta\nDeve ser:   'CW' ou 'CCW'");
    myDial->setValue(this->sensorData.start_angle);
    // Atualizar os dados do dial

    qDebug() << "\n\nSistema configurado e atualizado ao modelo:";
    qDebug() << "Model:" << sensorData.model_name;
    qDebug() << "Start Angle:" << sensorData.start_angle;
    qDebug() << "Arrive Angle:" << sensorData.arrive_angle;
    qDebug() << "Turn Direction:" << sensorData.turn_direction;
}

// Filtrar logs
void MainWindow::updateGeneralLogContent()
{
    originalGeneralLogContent = ui->general_log_screen->toPlainText();
}

void MainWindow::updateServoLogContent()
{
    originalServoLogContent = ui->servo_log_screen->toPlainText();
}

void MainWindow::filterGeneralLog(const QString &text)
{
    QString originalText = ui->general_log_screen->toPlainText(); // Store the ORIGINAL text
    QStringList linhas = originalText.split("\n"); // Split the ORIGINAL text
    QStringList linhasFiltradas;

    if (text.isEmpty()) { // If the filter text is empty, show all lines
        ui->general_log_screen->clear();
        ui->general_log_screen->append(originalText); // Restore the ORIGINAL content
        return; // Exit the function early
    }

    for (const QString &linha : linhas) {
        if (linha.contains(text, Qt::CaseInsensitive)) { // Use the received 'text'
            linhasFiltradas.append(linha); // Keep the line if it contains the filter text
        }
    }

    ui->general_log_screen->clear(); // Clear the text edit
    ui->general_log_screen->append(linhasFiltradas.join("\n")); // Display the filtered lines
}

void MainWindow::filterServoLog(const QString &text)
{
    QString originalText = ui->servo_log_screen->toPlainText(); // Store the ORIGINAL text
    QStringList linhas = originalText.split("\n"); // Split the ORIGINAL text
    QStringList linhasFiltradas;

    if (text.isEmpty()) { // If the filter text is empty, show all lines
        ui->servo_log_screen->clear();
        ui->servo_log_screen->append(originalText); // Restore the ORIGINAL content
        return; // Exit the function early
    }

    for (const QString &linha : linhas) {
        if (linha.contains(text, Qt::CaseInsensitive)) { // Use the received 'text'
            linhasFiltradas.append(linha); // Keep the line if it contains the filter text
        }
    }

    ui->servo_log_screen->clear(); // Clear the text edit
    ui->servo_log_screen->append(linhasFiltradas.join("\n")); // Display the filtered lines
}

// Configurações do comportamento do dial reativo

void MainWindow::configDial(QDial *_myDial){
    _myDial->setMinimum(sensorData.start_angle);
    _myDial->setMaximum(sensorData.arrive_angle);
    _myDial->setNotchesVisible(true);
    _myDial->setNotchTarget(3.7);
    _myDial->setWrapping(true); // Definir comportamento de wrapping
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
    myUser.username = "Usuário comum";
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
        usersHandler->showRemoveUserDialog(myUser);
    else
        QMessageBox::information(this,"Permissão negada", "Este tipo de operação só pode ser executada pelo Administrador");

}

void MainWindow::on_actionAtualizar_2_triggered()
{
    if(myUser.username == "Usuário comum"){
        QMessageBox::information(this,"Permissão negada", "Para atualizar, faça login!");

    }
    else{
        usersHandler->showUpdateUserDialog(myUser);
    }
}



// Funcionalidades relacionada ao micronas


//  Funções de comunicação ao servo
void MainWindow::servoState(bool servoSituation)
{
    servoUP=servoSituation;
    if(servoUP)
        qDebug() << " -> Servo conectado e pronto para inicializar operações\n";
    else
        qDebug() << " -> Servo não está conectado\n";
}

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
}

void MainWindow::initializeServo()
{

    emit initSignal();

    connect (myServo, &ServoMinas::initializationFinished,this,[this](bool situation){
        if(situation){
            qDebug()<< "Servo Habilitado";
            ui->init_servo_button->setEnabled(false);
            ui->disable_servo_button->setEnabled(true);
            ui->disable_servo_button_2->setEnabled(true);
            ui->enable_servo_communication->setEnabled(true);

        }
        else
            qDebug()<< "Servo não habilitado";
    });


}

void MainWindow::stopOperation()
{

    emit stopOperationSignal(); // Desabilitar o servo
    qDebug()<< "Operação parada";
}

void MainWindow::startHoming()
{
    emit startHomingSignal();// inicializar homing
    qDebug() << "inicializar o homing";
}

void MainWindow::insertedAngleToAchieve(){
    bool ok;
    double insertedValue = myInsertAbsolute->text().toDouble(&ok);
    qDebug() << "Valor alterado para:" << insertedValue;
    int velocity = 50; // valor padrão
    if(!ui->servo_velocity_setup->text().isEmpty()) // se ouver algo dentro da celular de velocidade, irá usar o valor de la
    {
        velocity = ui->servo_velocity_setup->text().toDouble();
    }
    if(ok){
        // myServo->moveAbsoluteTo(insertedValue,velocity);
        emit moveServoToPositionSignal(insertedValue,velocity);
    }
    else{
        QMessageBox::critical(this, "Erro", "Insira o dado flutuante corretamente entre os limites do modelo");
    }

}

void MainWindow::clearServoErrors()
{
    emit resetErrorsSignal();
}

// Operação Jog - Anti Horário CCW
void MainWindow::on_left_jog_released()
{
    bool ok_amount,ok_step;
    double amount= ui->amountJog_lineEdit->text().toDouble(&ok_amount);
    double step = ui->step_lineEdit->text().toDouble(&ok_step);
    int velocity = 50; // valor padrão
    if(!ui->servo_velocity_setup->text().isEmpty()) // se ouver algo dentro da celular de velocidade, irá usar o valor de la
    {
        velocity = ui->servo_velocity_setup->text().toDouble();
    }
    if(ok_amount && ok_step){
        qDebug() << "Jog de offset CCW em:" << amount*step <<"º";
        emit jogSignal(amount,velocity,step);
    }
    else{
        QMessageBox::critical(this, "Erro", "Insira o dado flutuante em step e amount");
        return;
    }

}

// Operação Jog - Horário CW
void MainWindow::on_right_jog_released()
{
    bool ok_amount,ok_step;

    double amount= ui->amountJog_lineEdit->text().toDouble(&ok_amount);
    double step = ui->step_lineEdit->text().toDouble(&ok_step);
    int velocity = 50; // valor padrão
    if(!ui->servo_velocity_setup->text().isEmpty()) // se ouver algo dentro da celular de velocidade, irá usar o valor de la
    {
        velocity = ui->servo_velocity_setup->text().toDouble();
    }
    if(ok_amount && ok_step){
        qDebug() << "Jog de offset CW em:" << amount*step << "º";
        emit jogSignal(-amount,velocity,step);
    }
    else{
        QMessageBox::critical(this, "Erro", "Insira o dado flutuante em step e amount");
        return;
    }
}


void MainWindow::setServoAngularPosition(double angle, double velocity){

        // definir o envio do pacote, ver se foi um sucesso e possivelmente enviar um não void para verificação
}


//  -------------- setters e getters para os dados da main --------------


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
void MainWindow::updateActualServoData(minas_control::MinasInput input){
    uint32_t raw_servo_value = input.position_actual_value; // valor absoluto unsigned int 32
    int32_t signed_angle = static_cast<int32_t>(raw_servo_value); // valor com sinal de 32 bits

    // Explicitly handle the sign
    double absolute_angle = (signed_angle >= 0)?
                                (360.0 * static_cast<double>(signed_angle) / 0x800000):
                                (-360.0 * static_cast<double>(-signed_angle) / 0x800000);

    double mydial_value = castoTo360(absolute_angle);
    myDial->setValue(-mydial_value);
    qDebug() << "| Absolute angle:" << absolute_angle;
    ui->servo_hex_position->setText( QString::number(signed_angle,16).toUpper()+"h");
    ui->servo_angle_position->setText(QString::number(absolute_angle,'f',4)+"º");
}

void MainWindow::readInputsUpdate(){
    emit readInputNowSignal();
}


void MainWindow::operationOnOFFBehavior(bool status){
    // status true: em operação
    // status false: não está em operação

    ui->homing_button->setEnabled(!status);
    myInsertAbsolute->setEnabled(!status);
    ui->right_jog->setEnabled(!status);
    ui->left_jog->setEnabled(!status);

}

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


