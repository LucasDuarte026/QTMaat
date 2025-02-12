#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QDial>
#include <QProgressBar>
#include <QTimer>
#include <QDoubleSpinBox>
#include <QThread>
#include <QTreeView>
#include <QStandardItemModel>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonArray>
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
#include "serialmicronas.h"

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


    // sinais para controle da thread servo
    void initSignal();
    void stopOperationSignal();
    void moveServoToPositionSignal(double position, double velocity);
    void startHomingSignal();
    void resetErrorsSignal();
    //jogging signals
    void jogSignal(double amount, double velocity,double step);

    //requisitar leitura da posição atual do encoder
    void readInputNowSignal();

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
    void on_actionLogout_triggered();
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
    // Controle jogging
    void on_left_jog_released();
    void on_right_jog_released();

    //  Filtrar as telas de log em função da aba
    void filterGeneralLog(const QString &text);
    void filterServoLog(const QString &text);
    //  Atualizar as telas de log
    void updateGeneralLogContent();
    void updateServoLogContent();

    //  Atualizar os dados na main page em função do estado atual do servo
    void updateActualServoData(minas_control::MinasInput input);
    void readInputsUpdate();

    //  Atualizar a interface a depender do estado de operação do servo
    void operationOnOFFBehavior(bool status);

    //  Setter de dados do tipo SensorData
    void setSensorData(SensorData _data);

    // Configurar a tela da engenharia
    void configTag_engenharia();
    void toggleTreeEditability(bool checked);

    // Aba de operação
    void on_prepare_operation_released();   // botão de preparação do servo antes da operação
    void on_start_operation_released();     // botão de iniciar operação do servo

private:
    // ponteiros para as estruturas a baixo da janela principal
    Ui::MainWindow *ui;
    QDial *myDial;
    QProgressBar *myAnimate_progress_bar;
    QDoubleSpinBox *mySpinBox;
    QLineEdit *myInsertAbsolute;
    SensorSelectionWindow *sensorWindow;
    SensorData sensorData;
    LogServoWindow *logServoWindow;

    //  Página de configurações da Engenharia
    QTreeView *myParameters_TreeView ;
    QStandardItemModel *model_tree ;

    //  Classe que cuida dos logs
    LogHandler * logHandler;
    //  Subtela de logs
    QString originalGeneralLogContent;
    QString originalServoLogContent;

    //  Classe dos logins e CRUD de usuários
    UsersHandler *usersHandler;

    QThread *myServoThread; //  Thread para deploy da classe myServo
    ServoMinas *myServo;    //  Classe ServoMinas para executar as operações e comunicações com o servo
    UserType myUser;  // Guarda o nome do usuário e sua permissão de acesso na página
    uint32 actual_servo_value;  // variável que guarda a posição atual do servo em hexa
    double actual_servo_angle;    // variável que guarda o ângulo atual do servo em graus
    bool servoUP;       // variável para ver se o servo está habilitado

    //cofigurar o dial
    void configDial(QDial *_myDial);
    void setDialDirection(QString direction_set);

    SerialMicronas *myMicronas;
};

#endif // MAINWINDOW_H
