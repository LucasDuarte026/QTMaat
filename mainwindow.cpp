#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "sensorselectionwindow.h"
#include "addsensordialog.h"
#include "removesensordialog.h"
#include "viewsensorsdialog.h"

int main_circle_x = 300, main_circle_y = 300;
int green_circle_x = 330, green_circle_y = 330;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , sensorWindow(nullptr)  // Inicializa o ponteiro como nullptr

{
    ui->setupUi(this); // configurar e iniciar os elementos definidos em UI

    QPixmap pixmap1(":/images/resources/main_circle.png");  // trocar pela imagem do black cicle
    ui->label_mainCircle->setPixmap(pixmap1);
    QPixmap pixmap2(":/images/resources/green_cicle.png");  // // trocar pela imagem do green circle
    ui->label_greenCircle->setPixmap(pixmap2);
    // Conexões

    // inicializar as imagens
    ui->label_mainCircle->move(main_circle_x, main_circle_y);
    ui->label_greenCircle->move(green_circle_x, green_circle_y);
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

        connect(sensorWindow, &SensorSelectionWindow::sensorSelected, this, &MainWindow::updateSensorLabel);

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

    if (dialog.exec() == QDialog::Accepted) {
        QString modelName = dialog.getModelName();
        float startAngle = dialog.getStartAngle();
        float endAngle = dialog.getEndAngle();
        QString rotation = dialog.getRotationDirection();

        // Verificar se o modelo já existe no CSV
        for (const QStringList &row : csvData) {
            if (row[0] == modelName) {
                QMessageBox::critical(this, "Erro", "O modelo a ser inserido já está incluído. Remova-o para inserir novamente.");
                return;
            }
        }

        // Adicionar a nova linha ao vetor
        QStringList newRow = { modelName, QString::number(startAngle), QString::number(endAngle), rotation };
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

void MainWindow::updateSensorLabel(const QString &sensorName)
{
    QString filePath = QCoreApplication::applicationDirPath() + "/sensors.csv";
    QFile file(filePath);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::critical(this, "Error", "Falha em abrir sensors.csv em modo leitura.");
        return;
    }

    QTextStream in(&file);
    bool sensorFound = false;

    // Skip the header line
    in.readLine();

    // Read each line to find the matching sensor
    while (!in.atEnd()) {
        QString line = in.readLine();
        QStringList parts = line.split(",");

        // Ensure the row has at least 4 columns
        if (parts.size() >= 4 && parts[0] == sensorName) {
            sensorFound = true;

            // Update the labels with the corresponding values
            ui->sensorSelected->setText(sensorName);
            ui->label_angle_start->setText(parts[1]);
            ui->label_angle_end->setText(parts[2]);
            ui->label_turn_direction->setText(parts[3]);

            model_name     = sensorName;
            start_angle    = parts[1].toDouble();;
            arrive_angle   = parts[2].toDouble();;
            turn_direction = parts[3];
            qDebug() << "Após a alteração, o modelo escolhido é:";
            qDebug() << "Model Name:" << model_name;
            qDebug() << "Start Angle:" << start_angle;
            qDebug() << "Arrive Angle:" << arrive_angle;
            qDebug() << "Turn Direction:" << turn_direction;
            break;
        }
    }

    file.close();

    if (!sensorFound) {
        QMessageBox::warning(this, "Sensor Not Found", "The selected sensor was not found in the CSV file.");
    }
}

void MainWindow::on_actionUpdatePositions_triggered()
{
    // Update global position variables
    main_circle_x += 10; // Move 10px to the right
    main_circle_y += 5;  // Move 5px down

    green_circle_x -= 10; // Move 10px to the left
    green_circle_y -= 5;  // Move 5px up

    // Update positions of labels
    ui->label_mainCircle->move(main_circle_x, main_circle_y);
    ui->label_greenCircle->move(green_circle_x, green_circle_y);
}
