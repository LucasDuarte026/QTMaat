#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "sensorselectionwindow.h"
#include "addsensordialog.h"
#include "removesensordialog.h"
#include "viewsensorsdialog.h"




MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , sensorWindow(nullptr)  // Inicializa o ponteiro como nullptr
    , sensorData{"Modelo não selecionado",0.0,360.0,"Sem sentido"}
    , actual_servo_value(0.0)
    , myServo(nullptr)        // ponteiro para a classe que controla o servo
    , logServoWindow(nullptr) // ponteiro para a janela de log dentro da mainWindow
    , timerState(true)
    , servoUP(false)          // bool que comunica o estado do servo (se está apto a ser usado)
    , initTimer(new QTimer(this)) // timer para controle dos botões

{
// config section
    ui->setupUi(this); // configurar e iniciar os elementos definidos em UI
    setWindowTitle("Ma'at");

    /* Verificar se o menu voltou a funcionar
        // Configuração da barra de menu para Linux
        ui->menubar->setVisible(true);
        ui->menubar->setEnabled(true);
        ui->menubar->setNativeMenuBar(false);

        // Garante que os menus estão habilitados e configurados
        ui->menuFile->setEnabled(true);
        ui->menuSensor->setEnabled(true);

        // Configura o comportamento dos menus para Linux
        menuBar()->setDefaultUp(true); // Força os menus a abrirem para cima
        ui->menuFile->setSeparatorsCollapsible(false);
        ui->menuSensor->setSeparatorsCollapsible(false);

        // Conecta os eventos de hover dos menus
        ui->menuFile->installEventFilter(this);
        ui->menuSensor->installEventFilter(this);
    */

    myDial = ui->dial_elements->findChild<QDial*>("plan_dial"); // usando o dial criado no UI
    myInsertDegree = ui->degreeInsertDial;
    myAnimate_progress_bar = ui->animate_progress_bar;
    myAnimate_progress_bar->setMinimum(this->sensorData.start_angle);
    myAnimate_progress_bar->setMaximum(this->sensorData.arrive_angle);
    myAnimate_progress_bar->setValue(0);
    if(myDial){
        configDial(myDial);
    }

    logHandler = new LogHandler(ui->general_log_screen, this);
    logServoWindow = new LogServoWindow(ui->log_screen, ui->search_line_log,
                                        ui->clean_log_button, ui->search_log_button, this);
    myServo = new ServoMinas("enp2s0"); // enp2s0 para linux. eth0 para windows

// seção das conexões
    //  Quando o sensor é selecionado, deve se mudar as variaveis privadas de mainWindow

    // Example: Connecting a signal from the QDial

    // inicializar a imagem

    // QPixmap pixmap2(":/images/resources/green_cicle.png");  // // trocar pela imagem do green circle
    // ui->label_greenCircle->setPixmap(pixmap2);

    connect(ui->sensorSelected, &QPushButton::clicked, this, &MainWindow::by_sensorSelected_action);
    connect(ui->animate_dial_button, &QPushButton::clicked, this, &MainWindow::by_animate_dial_button_action);

    connect(myDial, &QDial::valueChanged, this, [this](){
        // ui->actual_dial_degree->setText(QString::number(myDial->value(),'f',4)+"º");
        ui->actual_dial_degree->setText(QString::number(this->actual_servo_value,'f',4)+"º");

    });
    connect(myInsertDegree, &QLineEdit::returnPressed, this, [this](){
        bool ok;
        double insertedValue = myInsertDegree->text().toDouble(&ok);
        qDebug() << "Valor alterado para:" << insertedValue;
        if(ok && insertedValue<= sensorData.arrive_angle && insertedValue>=sensorData.start_angle){
        myDial->setValue(insertedValue);
        }
        else{
            QMessageBox::critical(this, "Erro", "Insira o dado flutuante corretamente entre os limites do modelo");
        }

    });


// botões de limpeza e de filtro da aba de logs gerais

    // Conectar botão de buscar ao filtro da aba general_log_screen
    connect(ui->general_log_search_button, &QPushButton::clicked, this, [this]() {
        QString searchText = ui->general_log_text_edit->text().trimmed();

        if (!searchText.isEmpty()) {
            QString logContent = ui->general_log_screen->toPlainText();
            QStringList lines = logContent.split("\n");

            QStringList filteredLines;
            for (const QString &line : lines) {
                if (line.contains(searchText, Qt::CaseInsensitive)) {
                    filteredLines.append(line);
                }
            }

            ui->general_log_screen->clear();
            for (const QString &line : filteredLines) {
                ui->general_log_screen->append(line);
            }
        }
    });

    // Conectar botão de limpar para apagar todo o log geral
    connect(ui->clean_all_output, &QPushButton::clicked, ui->general_log_screen, &QTextEdit::clear);

    connect(myDial, &QDial::sliderReleased, this, [this]() {
        double min = sensorData.start_angle ;
        double max = sensorData.arrive_angle ;
        double value = myDial->value();
        qDebug() << "valor atual é:" << value;
        if(value < min)
            myDial->setValue(min);
        else if( value> max)
            myDial->setValue(max);

    });


    // servo communication setup
    ui->init_servo_button->setEnabled(true);
    ui->disable_servo_button->setEnabled(false);
    // Connect checkbox toggle to the MainWindow signal
    connect(ui->enable_servo_communication, &QCheckBox::toggled, this, &MainWindow::servoCommunicationBox_stateChanged);

    // Cconectar as operações de log do servo com a tela de log
    connect(myServo, &ServoMinas::logMessage, logServoWindow, &LogServoWindow::appendLog);
    connect(myServo, &ServoMinas::state, this, &MainWindow::servoState);

    // conexões do timer de ligar e desligar o servo:
    connect(ui->init_servo_button, &QPushButton::clicked, this, &MainWindow::startInitTimer);
    connect(ui->disable_servo_button, &QPushButton::clicked, this, &MainWindow::startInitTimer);
    connect(initTimer, &QTimer::timeout, this, &MainWindow::initTimeout);

    // Operações no servo
    connect(ui->homing_button, &QPushButton::clicked, this, &MainWindow::startHoming);
    connect(ui->clear_errors, &QPushButton::clicked, this, &MainWindow::clearServoErrors);


}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::by_sensorSelected_action()
{
    // Cria a janela de seleção se ainda não existir
    sensorWindow = new SensorSelectionWindow(this);  // Define o MainWindow como pai
    connect(sensorWindow, &SensorSelectionWindow::sensorSelected, this, &MainWindow::updateSensorDependencies);
    // connect(sensorWindow, &QObject::destroyed, this, [this]() {
    //     sensorWindow = nullptr;
    //     });


    sensorWindow->show();  // Mostra a janela de seleção

}
void MainWindow::by_animate_dial_button_action()
{
    double min = sensorData.start_angle;
    double max= sensorData.arrive_angle;
    qDebug() << "tela entrou";
    for(double i=min;i<=max;i=i+0.1){

        qDebug() << "|" << min << "| -> |"  << max << "|     Posição do dial é:" << i ;

        this->setServoAbsolutePosition(i);
        myAnimate_progress_bar->setValue(i);
        myDial->setValue(i);
        QEventLoop loop;
        QTimer::singleShot(1, &loop, &QEventLoop::quit); // Pausa por 100ms
        loop.exec();
    }

}
void MainWindow::on_actionAdicionar_triggered()
{
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

void MainWindow::on_actionRemover_triggered()
{
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

void MainWindow::on_actionVer_sensores_triggered()
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
    myDial->setValue(this->sensorData.start_angle);
    myAnimate_progress_bar->setMinimum(this->sensorData.start_angle);
    myAnimate_progress_bar->setMaximum(this->sensorData.arrive_angle);
// Atualizar os dados do dial




    qDebug() << "\nSistema configurado e atualizado ao modelo:";
    qDebug() << "Model:" << sensorData.model_name;
    qDebug() << "Start Angle:" << sensorData.start_angle;
    qDebug() << "Arrive Angle:" << sensorData.arrive_angle;
    qDebug() << "Turn Direction:" << sensorData.turn_direction;
}

void MainWindow::configDial(QDial *_myDial){
    _myDial->setMinimum(sensorData.start_angle);
    _myDial->setMaximum(sensorData.arrive_angle);
    _myDial->setNotchesVisible(true);
    _myDial->setNotchTarget(3.7);
    _myDial->setWrapping(true); // Definir comportamento de wrapping
    _myDial->setSingleStep(1);
    _myDial->setValue(this->sensorData.start_angle);


}

void MainWindow::servoState(bool servoSituation){

    servoUP=servoSituation;
    if(servoUP){
        qDebug() << " -> Servo conectado e pronto para inicializar operações\n";

    }
    else
    {
        qDebug() << " -> Servo desconectado\n";
    }
}


void MainWindow::servoCommunicationBox_stateChanged(bool toogled){

    if(servoUP)
    {
        myServo->updateCommunicationState(toogled);
    }
    else{
        QMessageBox::critical(this, "Erro", "Servo não conectado");
        ui->enable_servo_communication->blockSignals(true);
        ui->enable_servo_communication->setChecked(false);
        ui->enable_servo_communication->blockSignals(false);
    }
}

void MainWindow::startInitTimer()
{

    if(timerState){
        ui->init_servo_button->setEnabled(false);
        ui->disable_servo_button->setEnabled(false);
        myServo->initialize(); // Inicializar o servo
        qDebug()<< "Servo Habilitado";
    }
    else{
        ui->init_servo_button->setEnabled(false);
        ui->disable_servo_button->setEnabled(false);
        myServo->disableServo(); // Desabilitar o servo
        qDebug()<< "Servo desabilitado";
    }
    initTimer->start(10000); //timer de 10 segundos
}

void MainWindow::initTimeout(){
    initTimer->stop();

    if(timerState)
    {
        ui->disable_servo_button->setEnabled(true);
        timerState = false;
    }
    else
    {
        ui->init_servo_button->setEnabled(true);
        timerState = true;
    }
}



void MainWindow::startHoming(){

    qDebug() << "inicializar o homing";
    myServo->moveToHome();
}



void MainWindow::clearServoErrors(){

    myServo->resetErrors();
}

//  -------------- setters e getters para os dados da main --------------

void MainWindow::setServoAbsolutePosition(double _value){

    // definir o envio do pacote, ver se foi um sucesso e possivelmente enviar um não void para verificação
}

void MainWindow::setSensorData(SensorData _data)
{
    this->sensorData = _data;
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

bool MainWindow::eventFilter(QObject *obj, QEvent *event)
{
    if (obj == ui->menuFile || obj == ui->menuSensor) {
        if (event->type() == QEvent::Enter) {
            QMenu *menu = qobject_cast<QMenu*>(obj);
            if (menu && !menu->isVisible()) {
                menu->popup(mapToGlobal(menuBar()->actionGeometry(menu->menuAction()).bottomLeft()));
            }
            return true;
        }
    }
    return QMainWindow::eventFilter(obj, event);
}
