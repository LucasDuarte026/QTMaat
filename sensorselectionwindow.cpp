#include "sensorselectionwindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QCoreApplication>
#include <QTextStream>
#include <QDebug>
#include <QMessageBox>

SensorSelectionWindow::SensorSelectionWindow(QWidget *parent)
    : QWidget(parent)
    , filterEdit(new QLineEdit(this))  // Barra de filtro
    , sensorList(new QListWidget(this))  // Lista de sensores
    , okButton(new QPushButton("OK", this))  // Botão "OK"
    , cancelButton(new QPushButton("Cancel", this))  // Botão "Cancel"
{
    // janela de seleção do sensor
    setWindowTitle("Selecione um sensor"); 
    resize(500, 400);

    // Configurar layout principal
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(filterEdit);  // Barra de filtro
    mainLayout->addWidget(sensorList);  // Lista de sensores

    // Layout para os botões
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget(okButton);      // Botão "OK"
    buttonLayout->addWidget(cancelButton); // Botão "Cancel"
    mainLayout->addLayout(buttonLayout);

    // Configurar placeholders
    filterEdit->setPlaceholderText("Procure por um modelo...");

    // Carregar sensores
    loadSensors();

    // Conectar o filtro à lista
    connect(filterEdit, &QLineEdit::textChanged, this, &SensorSelectionWindow::filterSensors);

    // Conectar o botão "OK"
    connect(okButton, &QPushButton::clicked, this, &SensorSelectionWindow::confirmSelection);

    // Conectar o botão "Cancel"
    connect(cancelButton, &QPushButton::clicked, this, &SensorSelectionWindow::cancelSelection);

}

SensorSelectionWindow::~SensorSelectionWindow()
{
}

// Carrega os sensores do CSV para dentro da lista sensorList
void SensorSelectionWindow::loadSensors()
{
    QFile file("sensors.csv");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "Falha em abrir o DataBase!";
        return;
    }
    QTextStream in(&file);

    // Pula o cabeçalho do CSV
    if (!in.atEnd()) {
        in.readLine();  // lê e descarta a primeira linhas
    }
    while (!in.atEnd()) {
        QString line = in.readLine();
        QStringList parts = line.split(",");  // Divide a linha por vírgulas

        if (!parts.isEmpty()) {
            sensorList->addItem(parts[0]);  // Adiciona apenas o nome do modelo (primeira coluna)
        }
    }

    file.close();
}

// Filtra os sensores da janela
void SensorSelectionWindow::filterSensors(const QString &text)
{ 
    for (int i = 0; i < sensorList->count(); ++i) {
        QListWidgetItem *item = sensorList->item(i);
        item->setHidden(!item->text().contains(text, Qt::CaseInsensitive));  // Mostra/esconde com base no filtro
    }
}

// Confirma a seleção de um sensor com o botão "OK"
void SensorSelectionWindow::confirmSelection()
{
    QListWidgetItem *selectedItem = sensorList->currentItem(); // Obtém o item selecionado

    if (selectedItem) { // busca o resto dos valores o item selecionado
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

        QString sensorName = selectedItem->text();

        SensorData sensor_selected;

        // ler linha por linha até encontrar o modelo
        while (!in.atEnd()) {
            QString line = in.readLine();
            QStringList parts = line.split(",");

            // Check for at least 4 columns before accessing elements
            if (parts.size() < 4) {
                qWarning() << "Invalid line in sensor data file: " << line;
                continue; // Skip to the next line
            }

            if (parts[0] == sensorName) {
                sensorFound = true;

                sensor_selected.model_name = sensorName;
                // Check parts.size() before accessing elements
                if (parts.size() >= 2) {
                    sensor_selected.start_angle = parts[1].toDouble();
                }
                if (parts.size() >= 3) {
                    sensor_selected.arrive_angle = parts[2].toDouble();
                }
                if (parts.size() >= 4) {
                    sensor_selected.turn_direction = parts[3];
                }
                break;
            }
        }

        file.close();

        if (!sensorFound) {
            QMessageBox::warning(this, "Sensor não encontrado", "O sensor selecionado não foi encontrado nos CSV.");
        }

        SensorData *sensor_selected_ptr = new SensorData(sensor_selected); // pega o ponteiro alocado do objeto para passar para frente
        emit sensorSelected(sensor_selected_ptr); // Emite o sinal com o nome do sensor e suas informações

        close(); // Fecha a janela
    } else {
        QMessageBox::warning(this, "Sem selecão", "Selecione um sensor.");
    }
}

void SensorSelectionWindow::cancelSelection()
{
    close();  // Fecha a janela sem fazer nada
}
