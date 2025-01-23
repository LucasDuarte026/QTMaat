#include "sensorselectionwindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QCoreApplication>
#include <QTextStream>
#include <QDebug>
#include <QMessageBox>

SensorSelectionWindow::SensorSelectionWindow(QWidget *parent)
    : QWidget(parent)
    , filterEdit(new QLineEdit(this))
    , sensorList(new QListWidget(this))
    , okButton(new QPushButton("OK", this))
    , cancelButton(new QPushButton("Cancel", this))
{
    setWindowTitle("Select a Sensor");
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

void SensorSelectionWindow::loadSensors()
{
    QFile file("sensors.csv");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "Failed to open sensor database!";
        return;
    }

    QTextStream in(&file);

    // Skip the header line
    if (!in.atEnd()) {
        in.readLine();  // Read and discard the first line
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

void SensorSelectionWindow::filterSensors(const QString &text)
{
    for (int i = 0; i < sensorList->count(); ++i) {
        QListWidgetItem *item = sensorList->item(i);
        item->setHidden(!item->text().contains(text, Qt::CaseInsensitive));  // Mostra/esconde com base no filtro
    }
}

void SensorSelectionWindow::confirmSelection()
{
    QListWidgetItem *selectedItem = sensorList->currentItem(); // Obtém o item selecionado

    if (selectedItem) {
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

        emit sensorSelected(sensor_selected); // Emite o sinal com o nome do sensor e suas informações

        close(); // Fecha a janela
    } else {
        QMessageBox::warning(this, "No Selection", "Selecione um sensor.");
    }
}

void SensorSelectionWindow::cancelSelection()
{
    close();  // Fecha a janela sem fazer nada
}


