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
#include "usershandler.h"


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr); // construtor da janela principal
    ~MainWindow();

    // retornar os atributos do sensor
    QString getModelName();
    double getStartAngle();
    double getArriveAngle();
    QString getTurnDirection();


signals:
    // Sinal enviado pela main window para avisar que a comunicação com o servo foi iniciada
    void servoCheckBoxChanged(bool checked);    // Enviar o sinal geral que a caixa de habilitação de comunicação com o servo foi mudada de estado

private slots:
    //Atualizar as dependências na mainWindow em função do sensor selecionado
    void updateSensorDependencies(SensorData *_sensorData);

    // Abrir a janela de seleção do sensor pela janela principal
    void by_sensorSelected_action();

    // CRUD de sensores
    void on_actionVer_Sensores_triggered();
    void on_actionAdicionar_triggered();
    void on_actionAtualizar_triggered();
    void on_actionRemover_triggered();

    // Login e CRUD de usuários
    void on_login_menuBar_triggered();
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
    void setServoAngularPosition(double angle, double velocity);    //  envia comando para o servo de posicionamento

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



    void on_actionLogout_triggered();

private:
    // ponteiros para as estruturas a baixo da janela principal
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

    UserType myUser;  // Guarda o nome do usuário e sua permissão de acesso na página
    double actual_servo_value; // variável que guarda a posição atual do servo em hexa
    bool servoUP;       // variável para ver se o servo está habilitado

    void configDial(QDial *_myDial);

};

#endif // MAINWINDOW_H
