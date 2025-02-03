#include "updatesensordialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QComboBox>
#include <QPushButton>
#include <QMessageBox>

UpdateSensorDialog::UpdateSensorDialog(const SensorData& currentData, QWidget *parent)
    : QDialog(parent)
    , modelNameEdit(new QLineEdit(this))
    , startAngleEdit(new QLineEdit(this))
    , endAngleEdit(new QLineEdit(this))
    , rotationDirection(new QComboBox(this))
    , okButton(new QPushButton("OK", this))
    , cancelButton(new QPushButton("Cancel", this))
{
    setWindowTitle("Atualizar Sensor");

    // Pre-fill with current data
    modelNameEdit->setText(currentData.model_name);
    startAngleEdit->setText(QString::number(currentData.start_angle));
    endAngleEdit->setText(QString::number(currentData.arrive_angle));

    // Setup rotation direction
    rotationDirection->addItems({"CW", "CCW"});
    rotationDirection->setCurrentText(currentData.turn_direction);

    // Layout setup
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    // Model name
    QHBoxLayout *modelLayout = new QHBoxLayout();
    modelLayout->addWidget(new QLabel("Nome do Modelo:"));
    modelLayout->addWidget(modelNameEdit);
    mainLayout->addLayout(modelLayout);

    // Start angle
    QHBoxLayout *startAngleLayout = new QHBoxLayout();
    startAngleLayout->addWidget(new QLabel("Ângulo de partida:"));
    startAngleLayout->addWidget(startAngleEdit);
    mainLayout->addLayout(startAngleLayout);

    // End angle
    QHBoxLayout *endAngleLayout = new QHBoxLayout();
    endAngleLayout->addWidget(new QLabel("Ângulo de chegada:"));
    endAngleLayout->addWidget(endAngleEdit);
    mainLayout->addLayout(endAngleLayout);

    // Rotation direction
    QHBoxLayout *rotationLayout = new QHBoxLayout();
    rotationLayout->addWidget(new QLabel("Sentido de rotação:"));
    rotationLayout->addWidget(rotationDirection);
    mainLayout->addLayout(rotationLayout);

    // Buttons
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget(okButton);
    buttonLayout->addWidget(cancelButton);
    mainLayout->addLayout(buttonLayout);

    // Connect signals
    connect(okButton, &QPushButton::clicked, this, &UpdateSensorDialog::validateInputs);
    connect(cancelButton, &QPushButton::clicked, this, &QDialog::reject);
}

UpdateSensorDialog::~UpdateSensorDialog()
{
}

QString UpdateSensorDialog::getModelName() const
{
    return modelNameEdit->text();
}

float UpdateSensorDialog::getStartAngle() const
{
    return startAngleEdit->text().toFloat();
}

float UpdateSensorDialog::getEndAngle() const
{
    return endAngleEdit->text().toFloat();
}

QString UpdateSensorDialog::getRotationDirection() const
{
    return rotationDirection->currentText();
}

void UpdateSensorDialog::validateInputs()
{
    if (modelNameEdit->text().isEmpty() ||
        startAngleEdit->text().isEmpty() ||
        endAngleEdit->text().isEmpty()) {
        QMessageBox::warning(this, "Entrada inválida", "Todas as entradas devem ser preenchidas");
        return;
    }

    bool startAngleOk = false;
    bool endAngleOk = false;
    double start = startAngleEdit->text().toFloat(&startAngleOk);
    double end = endAngleEdit->text().toFloat(&endAngleOk);

    if (!startAngleOk || !endAngleOk) {
        QMessageBox::warning(this, "Entrada inválida", "Os ângulos devem ser números válidos");
        return;
    }

    if (start < 0.0 || start > 360.0 || end < 0.0 || end > 360.0) {
        QMessageBox::warning(this, "Entrada inválida", "Os ângulos devem estar entre 0.0 e 360.0 graus");
        return;
    }

    accept();
}
