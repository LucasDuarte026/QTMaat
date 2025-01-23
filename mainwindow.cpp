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
    , sensorData{"Modelo não selecionado",0.0,0.0,"Sem sentido"}

{
// config section
    ui->setupUi(this); // configurar e iniciar os elementos definidos em UI

    myDial = ui->dial_elements->findChild<QDial*>("plan_dial"); // usando o dial criado no UI



// seção das conexões
    //  Quando o sensor é selecionado, deve se mudar as variaveis privadas de mainWindow

    // Example: Connecting a signal from the QDial


    // QPixmap pixmap1(":/images/resources/main_circle.png");  // trocar pela imagem do black cicle
    // ui->label_mainCircle->setPixmap(pixmap1);
    QPixmap pixmap2(":/images/resources/green_cicle.png");  // // trocar pela imagem do green circle
    ui->label_greenCircle->setPixmap(pixmap2);
    // // Conexões

    // // inicializar as imagens
    // ui->label_mainCircle->move(main_circle_x, main_circle_y);
    connect(ui->sensorSelected, &QPushButton::clicked, this, &MainWindow::on_sensorSelected_clicked);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_sensorSelected_clicked()
{
    // Cria a janela de seleção se ainda não existir
    if (!sensorWindow) {
        sensorWindow = new SensorSelectionWindow(this);  // Define o MainWindow como pai

        connect(sensorWindow, &SensorSelectionWindow::sensorSelected, this, &MainWindow::updateSensorDependencies);
        // connect(sensorWindow, &SensorSelectionWindow::sensorSelected, this, &MainWindow::updateDial);

    }
    sensorWindow->show();  // Mostra a janela de seleção
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

void MainWindow::updateSensorDependencies(SensorData &_sensorData) {
    // myDial->updateDial(_sensorData)

        // Atualizar os dados do dial

    // Atualizar as configurações do dial de forma segura
    myDial->blockSignals(true); // Evita emitir sinais durante a atualização
    myDial->setMinimum(_sensorData.start_angle);
    myDial->setMaximum(_sensorData.arrive_angle);
    myDial->setWrapping(true); // Definir comportamento de wrapping (não deve causar crash)
    myDial->setValue(_sensorData.start_angle);
    myDial->blockSignals(false); // Reativa os sinais

    qDebug() << "Dial updated with new sensor data:";
    qDebug() << "Model:" << _sensorData.model_name
             << "Start Angle:" << _sensorData.start_angle
             << "Arrive Angle:" << _sensorData.arrive_angle
             << "Turn Direction:" << _sensorData.turn_direction;
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

