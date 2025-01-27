#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QDial>
#include <QProgressBar>
#include <QTimer>
#include <QDoubleSpinBox>
#include "sensorselectionwindow.h"
#include "datatypes.h"



namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    QString getModelName();
    double getStartAngle();
    double getArriveAngle();
    QString getTurnDirection();


private slots:
    void updateSensorDependencies(SensorData *_sensorData);
    void by_sensorSelected_action();
    void on_actionAdicionar_triggered();
    void on_actionRemover_triggered();
    void on_actionVer_sensores_triggered();
    void by_animate_dial_button_action();
    void setSensorData(SensorData _data);




private:
    Ui::MainWindow *ui;
    QDial *myDial;
    QProgressBar *myAnimate_progress_bar;
    QDoubleSpinBox *mySpinBox;
    QLineEdit *myInsertDegree;
    SensorSelectionWindow *sensorWindow;
    SensorData sensorData;
    double actual_servo_value;

    void setServoAbsolutePosition(double _value);
    void configDial(QDial *_myDial);

};

#endif // MAINWINDOW_H
