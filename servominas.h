#ifndef SERVOMINAS_H
#define SERVOMINAS_H

#include <QObject>
#include <QTimer>
#include <QDateTime>
#include <QThread>
#include <QElapsedTimer>
#include <QCoreApplication>
#include <QException>
#include "ethercat_manager.h"
#include "minas_client.h"
#include "worker.h"


class ServoMinas : public QObject
{
    Q_OBJECT

public:
    ServoMinas(QString interface);
    ~ServoMinas();

    bool initialize(); // Inicializa o cliente EtherCAT
    void moveAbsoluteTo(double position, double velocity = 100); // Move para uma posição absoluta
    void moveOffset(double offset, double velocity, double step );   //  Move com offset da posição atual
    void moveAngularTo(double angle, double velocity = 100); // Move para uma posição angular
    void enableServo(int mode); // Habilita o servo para operação
    void disableServo(); // Desabilita o servo
    void resetErrors(); // Reseta os erros no servo
    void moveToHome(); // Move to home position
    void setActualPosition(uint32_t value);
    uint32_t getActualPosition();


public slots:
    void updateCommunicationState(bool checked);


signals:
    void logMessage(QString message); // enviar os logs para o logwindow
    void inputChangedSignal(minas_control::MinasInput input);
    // void outputChangedSignal(minas_control::MinasInput output); // não será usado

    //  Sinais de controle
    void initializationFinished(bool situation);
    void operationStatusSignal(bool status); // sinal de que a worker entrou ou saiu em operação, não pode iniciar mais de uma operação ao mesmo tempo

private:
    void threadMoveToHome();
    void threadMoveAbsoluteTo(double position, double velocity );
    void newLogReceived(QString message);

    QThread *workerThread;
    Worker *worker;

    QString interfaceName;
    ethercat::EtherCatManager *manager; //  cliente que gerencia a comunicação em baixo nível EtherCAT
    minas_control::MinasClient *client; //  client qque gerencia a comunicação em auto nivel com o drive

    struct timespec tick;// tick de tempo
    double period;      //  periodo de comunicação com o equipamento
    int iterationCount;

    uint32_t actual_position;
    bool isCommunicationEnabled;
    void configureSafetyLimits(); // Configura os limites de segurança do servo
    minas_control::MinasOutput  readOutput();
    minas_control::MinasInput   readInput();

};

#endif // SERVOMINAS_H
