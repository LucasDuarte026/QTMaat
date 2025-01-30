#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QDial>
#include <QProgressBar>
#include <QTimer>
#include <QDoubleSpinBox>
#include <QTimer>
#include "sensorselectionwindow.h"
#include "datatypes.h"
#include "servominas.h"
#include "logservowindow.h"
#include "loghandler.h"


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

protected:
    bool eventFilter(QObject *obj, QEvent *event) override;

signals:
    void servoCheckBoxChanged(bool checked);    // Enviar o sinal geral que a caixa de habilitação de comunicação com o servo foi mudada de estado

private slots:
    void updateSensorDependencies(SensorData *_sensorData);
    void by_sensorSelected_action();
    void on_actionAdicionar_triggered();
    void on_actionRemover_triggered();
    void on_actionVer_sensores_triggered();
    void by_animate_dial_button_action();
    void setSensorData(SensorData _data);
    void servoCommunicationBox_stateChanged(bool checked);
    void servoState(bool servoSituation);
    void startInitTimer();
    void initTimeout();
    void startHoming ();
    void clearServoErrors();
private:
    Ui::MainWindow *ui;
    QDial *myDial;
    QProgressBar *myAnimate_progress_bar;
    QDoubleSpinBox *mySpinBox;
    QLineEdit *myInsertDegree;
    SensorSelectionWindow *sensorWindow;
    SensorData sensorData;
    LogServoWindow *logServoWindow;
    LogHandler * logHandler;
    QTimer *initTimer;


    ServoMinas *myServo;

    double actual_servo_value;
    bool timerState;    //  Variavel para controle do estado do servo para controle de ligar e desligá-lo
    bool servoUP;
    void setServoAbsolutePosition(double _value);
    void configDial(QDial *_myDial);

};

#endif // MAINWINDOW_H
