#include "removesensordialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QMessageBox>

RemoveSensorDialog::RemoveSensorDialog(QWidget *parent)
    : QDialog(parent)
    , sensorComboBox(new QComboBox(this))
    , removeButton(new QPushButton("Remover", this))
    , cancelButton(new QPushButton("Cancelar", this))
{
    setWindowTitle("Remover Sensor");
    resize(300, 150);

    // Layout principal
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    // Label e combo box
    QHBoxLayout *selectionLayout = new QHBoxLayout();
    selectionLayout->addWidget(new QLabel("Selecione o sensor:"));
    selectionLayout->addWidget(sensorComboBox);
    mainLayout->addLayout(selectionLayout);

    // Botões
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget(removeButton);
    buttonLayout->addWidget(cancelButton);
    mainLayout->addLayout(buttonLayout);

    // Conexões
    connect(removeButton, &QPushButton::clicked, this, &RemoveSensorDialog::handleRemoveButton);
    connect(cancelButton, &QPushButton::clicked, this, &QDialog::reject);
}

RemoveSensorDialog::~RemoveSensorDialog() {}

QString RemoveSensorDialog::getSelectedSensor() const
{
    return sensorComboBox->currentText();
}

void RemoveSensorDialog::setSensorList(const QStringList &sensors)
{
    sensorComboBox->clear();
    sensorComboBox->addItems(sensors);  // Adiciona os sensores disponíveis ao menu
}

void RemoveSensorDialog::handleRemoveButton()
{
    if (sensorComboBox->currentText().isEmpty()) {
        QMessageBox::warning(this, "Erro", "Nenhum sensor selecionado.");
        return;
    }

    accept();  // Fecha o diálogo e sinaliza sucesso
}
