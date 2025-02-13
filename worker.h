#ifndef WORKER_H
#define WORKER_H

#include <QObject>
#include <QMutex>
#include <QTimer>
#include "ethercat_manager.h"
#include "minas_client.h"
#include "datatypes.h"

class Worker : public QObject {
    Q_OBJECT

public:
    explicit Worker(minas_control::MinasClient *_client,EngParameters *engParameters,QObject* parent = nullptr);

public slots:
    void threadMoveToHome();
    void threadMoveAbsoluteTo(double amount, double velocity);
    void threadMoveOffset(double amount,double velocity,double step);

signals:
    void sendLog(QString message);
    void finished();// sinal para indicar que a thread parou e terminou sua operação
    void inputChangedSignalThread(minas_control::MinasInput input);

private:

    minas_control::MinasInput threadReadInput();
    minas_control::MinasOutput threadReadOutput();
    minas_control::MinasClient *_client;
    void threadDisableServo();
    void threadEnableServo(int mode);
    struct timespec tick;// tick de tempo
    int period;
    minas_control::MinasClient *client;
    EngParameters *engParameters;
};

#endif // WORKER_H
