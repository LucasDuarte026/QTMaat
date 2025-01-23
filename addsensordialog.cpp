#include "addsensordialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QComboBox>
#include <QPushButton>
#include <QMessageBox>

AddSensorDialog::AddSensorDialog(QWidget *parent)
    : QDialog(parent)
    , modelNameEdit(new QLineEdit(this))
    , startAngleEdit(new QLineEdit(this))
    , endAngleEdit(new QLineEdit(this))
    , rotationDirection(new QComboBox(this))
    , okButton(new QPushButton("OK", this))
    , cancelButton(new QPushButton("Cancel", this))
{
    setWindowTitle("Adicionar um novo modelo");

    // Configurar campos de entrada
    rotationDirection->addItems({"CW", "CCW"});

    // Layout para o diálogo
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    // Nome do modelo
    QHBoxLayout *modelLayout = new QHBoxLayout();
    modelLayout->addWidget(new QLabel("Nome do Modelo:"));
    modelLayout->addWidget(modelNameEdit);
    mainLayout->addLayout(modelLayout);

    // Ângulo de partida
    QHBoxLayout *startAngleLayout = new QHBoxLayout();
    startAngleLayout->addWidget(new QLabel("Ângulo de partida:"));
    startAngleLayout->addWidget(startAngleEdit);
    mainLayout->addLayout(startAngleLayout);

    // Ângulo de chegada
    QHBoxLayout *endAngleLayout = new QHBoxLayout();
    endAngleLayout->addWidget(new QLabel("Ângulo de chegada:"));
    endAngleLayout->addWidget(endAngleEdit);
    mainLayout->addLayout(endAngleLayout);

    // Sentido de giro
    QHBoxLayout *rotationLayout = new QHBoxLayout();
    rotationLayout->addWidget(new QLabel("Sentido de rotação"));
    rotationLayout->addWidget(rotationDirection);
    mainLayout->addLayout(rotationLayout);

    // Botões OK e Cancelar
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget(okButton);
    buttonLayout->addWidget(cancelButton);
    mainLayout->addLayout(buttonLayout);

    // Conectar os botões
    connect(okButton, &QPushButton::clicked, this, &AddSensorDialog::validateInputs);
    connect(cancelButton, &QPushButton::clicked, this, &QDialog::reject);
}

AddSensorDialog::~AddSensorDialog()
{
}

QString AddSensorDialog::getModelName() const
{
    return modelNameEdit->text();
}

float AddSensorDialog::getStartAngle() const
{
    return startAngleEdit->text().toFloat();
}

float AddSensorDialog::getEndAngle() const
{
    return endAngleEdit->text().toFloat();
}

QString AddSensorDialog::getRotationDirection() const
{
    return rotationDirection->currentText();
}

void AddSensorDialog::validateInputs()
{
    if (modelNameEdit->text().isEmpty() ||
        startAngleEdit->text().isEmpty() ||
        endAngleEdit->text().isEmpty()) {
        QMessageBox::warning(this, "Entrada inválida", "Todas as entradas devem ser preenchidas");
        return;
    }
    // Verifica se os ângulos são valores de ponto flutuante
    bool startAngleOk = false;
    bool endAngleOk = false;

    startAngleEdit->text().toFloat(&startAngleOk);  // Tenta converter para float
    endAngleEdit->text().toFloat(&endAngleOk);      // Tenta converter para float

    if (!startAngleOk || !endAngleOk) {
        QMessageBox::warning(this, "Formato inválido", "Insira os valores com ponto flutuante");
        return;
    }
    accept();  // Fecha o diálogo e sinaliza sucesso
}
