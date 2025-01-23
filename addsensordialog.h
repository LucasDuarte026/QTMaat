#ifndef ADDSENSORDIALOG_H
#define ADDSENSORDIALOG_H

#include <QDialog>

class QLineEdit;
class QComboBox;
class QPushButton;

class AddSensorDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddSensorDialog(QWidget *parent = nullptr);
    ~AddSensorDialog();

    // Função para obter os valores inseridos
    QString getModelName() const;
    float getStartAngle() const;
    float getEndAngle() const;
    QString getRotationDirection() const;

private:
    QLineEdit *modelNameEdit;       // Campo para o nome do modelo
    QLineEdit *startAngleEdit;      // Campo para o ângulo de partida
    QLineEdit *endAngleEdit;        // Campo para o ângulo de chegada
    QComboBox *rotationDirection;  // Campo para o sentido de giro

    QPushButton *okButton;         // Botão OK
    QPushButton *cancelButton;     // Botão Cancelar

private slots:
    void validateInputs();         // Valida os dados de entrada
};

#endif // ADDSENSORDIALOG_H
