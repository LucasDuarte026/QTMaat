#ifndef UPDATESENSORDIALOG_H
#define UPDATESENSORDIALOG_H

#include <QDialog>
#include "datatypes.h"

class QLineEdit;
class QComboBox;
class QPushButton;

class UpdateSensorDialog : public QDialog
{
    Q_OBJECT

public:
    explicit UpdateSensorDialog(const SensorData& currentData, QWidget *parent = nullptr);
    ~UpdateSensorDialog();

    QString getModelName() const;
    float getStartAngle() const;
    float getEndAngle() const;
    QString getRotationDirection() const;

private:
    QLineEdit *modelNameEdit;
    QLineEdit *startAngleEdit;
    QLineEdit *endAngleEdit;
    QComboBox *rotationDirection;
    QPushButton *okButton;
    QPushButton *cancelButton;

private slots:
    void validateInputs();
};

#endif // UPDATESENSORDIALOG_H
