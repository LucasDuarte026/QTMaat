#ifndef SERVOMINAS_H
#define SERVOMINAS_H

#include <QObject>
#include <QElapsedTimer>
#include <QCoreApplication>
#include <QException>
#include "ethercat_manager.h"
#include "minas_client.h"

class ServoMinas : public QObject
{
    Q_OBJECT

public:
    ServoMinas(QString interface);
    ~ServoMinas();

    void initialize(); // Inicializa o cliente EtherCAT
    void moveAbsoluteTo(double position, double velocity = 500); // Move para uma posição absoluta
    void enableServo(int mode); // Habilita o servo para operação
    void disableServo(); // Desabilita o servo
    void resetErrors(); // Reseta os erros no servo
    void moveToHome(); // Move to home position

public slots:
    void updateCommunicationState(bool checked);


signals:
    void logMessage(QString message); // enviar os logs para o logwindow
    void state(bool servo_situation);
private:

    QString interfaceName;
    ethercat::EtherCatManager *manager;
    minas_control::MinasClient *client;
    minas_control::MinasOutput output;
    minas_control::MinasInput input;

    bool isCommunicationEnabled;

    void configureSafetyLimits(); // Configura os limites de segurança do servo
};

#endif // SERVOMINAS_H
