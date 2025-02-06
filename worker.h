#ifndef WORKER_H
#define WORKER_H

#include <QObject>
#include <QMutex>
#include <QTimer>
#include "ethercat_manager.h"
#include "minas_client.h"

class Worker : public QObject {
    Q_OBJECT

public:
    explicit Worker(minas_control::MinasClient *_client,QObject* parent = nullptr);

public slots:
    void threadMoveToHome();
    void threadMoveAbsoluteTo(double position, double velocity);

signals:
    void sendLog(QString message);
    void finished();
    void inputChangedSignalThread(minas_control::MinasInput input);

private:

    minas_control::MinasInput threadReadInput();
    minas_control::MinasOutput threadReadOutput();
    minas_control::MinasClient *_client;
    void threadDisableServo();
    void ThreadEnableServo(int mode);
    struct timespec tick;// tick de tempo
    int period;
    minas_control::MinasClient *client;
};

#endif // WORKER_H
