#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QDial>
#include <QProgressBar>
#include <QTimer>
#include <QDoubleSpinBox>
#include "sensorselectionwindow.h"
#include "addsensordialog.h"
#include "removesensordialog.h"
#include "viewsensorsdialog.h"
#include "updatesensordialog.h"
#include "datatypes.h"
#include "servominas.h"
#include "logservowindow.h"
#include "loghandler.h"
#include "userlogin.h"
#include "usershandler.h"


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


signals:
    void servoCheckBoxChanged(bool checked);    // Enviar o sinal geral que a caixa de habilitação de comunicação com o servo foi mudada de estado

private slots:
    //Atualizar as dependências na mainWindow em função do sensor selecionado
    void updateSensorDependencies(SensorData *_sensorData);
    void by_sensorSelected_action();

    // CRUD de sensores
    void on_actionVer_Sensores_triggered();
    void on_actionAdicionar_triggered();
    void on_actionAtualizar_triggered();
    void on_actionRemover_triggered();

    // Login e CRUD de usuários
    void on_actionlogin_triggered();
    void on_actionVer_usu_rios_triggered();
    void on_actionAdicionar_2_triggered();
    void on_actionAtualizar_2_triggered();
    void on_actionRemover_2_triggered();

    // Simulação de operação do dial
    void by_animate_dial_button_action();

    //  Estados do servo dentro do contexto da janela principal
    void servoCommunicationBox_stateChanged(bool checked);
    void servoState(bool servoSituation);

    //  Funções de controle EtherCAT
    void stopOperation();
    void initializeServo();
    void startHoming ();
    void clearServoErrors();
    void insertedAngleToAchieve();

    //  Filtrar as telas de log em função da aba
    void filterGeneralLog(const QString &text);
    void filterServoLog(const QString &text);
    //  Atualizar as telas de log
    void updateGeneralLogContent();
    void updateServoLogContent();
    //  Atualizar
    void updateActualServoData();

    //  Setter de dados do tipo SensorData
    void setSensorData(SensorData _data);


private:
    Ui::MainWindow *ui;
    QDial *myDial;
    QProgressBar *myAnimate_progress_bar;
    QDoubleSpinBox *mySpinBox;
    QLineEdit *myInsertDegree;
    SensorSelectionWindow *sensorWindow;
    SensorData sensorData;
    LogServoWindow *logServoWindow;

    //  Classe que cuida dos logs
    LogHandler * logHandler;
    //  Subtela de logs
    QString originalGeneralLogContent;
    QString originalServoLogContent;

    //  Classe dos logins e CRUD de usuários
    UsersHandler *usersHandler;


    ServoMinas *myServo;

    double actual_servo_value;
    bool timerState;    //  Variavel para controle do estado do servo para controle de ligar e desligá-lo
    bool servoUP;
    void setServoAngularPosition(double angle, double velocity);
    void configDial(QDial *_myDial);

};

#endif // MAINWINDOW_H
