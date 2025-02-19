#include <iostream>
#include "servominas.h"
#include <cstring> // Para memset
#include <QDebug>
#include <QMessageBox>

#define NSEC_PER_SECOND 1e+9

ServoMinas::ServoMinas(QString interface, EngParameters *_engParameters)
    : QObject()
    ,   interfaceName(interface)
    ,   manager(nullptr)
    ,   client(nullptr)
    ,   isCommunicationEnabled(false)
    ,   worker(nullptr)
    ,   workerThread(nullptr)
    ,   engParameters(_engParameters)

{
    // configurar paramentros do tempo 
    period = 4e+6; // 4ms em nanossegundos
    clock_gettime(CLOCK_REALTIME, &tick);
}

// destruto
ServoMinas::~ServoMinas() {
    if (client) {
        client->servoOff();
        delete client;
    }
    if (manager) {
        delete manager;
    }
}

//  enviar log
void ServoMinas::newLogReceived(QString message){
    emit logMessage("-> Worker:" +message);
}

// inicializa o manager, o cliente e a comunicação com o servo
bool ServoMinas::initialize() {
    QString message;
    try {
        // inicializa o manager
        manager = new ethercat::EtherCatManager(interfaceName.toStdString());
        qDebug() << "Inicializando o gerenciador EtherCAT...";

        if (manager->getNumClients() < 1) {
            message = "Certifique-se que o servo está conectado";
            emit logMessage(message);
            emit initializationFinished(false); // Emitir sinal de não exito
            manager = nullptr;
            client = nullptr;
            return false; // Importante: Sair cedo se nenhum cliente for encontrado
        }
        //  inicializa o cliente
        client = new minas_control::MinasClient(*manager, 1);
        resetErrors();
        configureSafetyLimits();


        message = "ServoMinas inicializado com sucesso. ";
        emit logMessage(message);
        qDebug() << message ;
        emit initializationFinished(true); // Emitir sinal de sucesso
        return true;

    } catch (const ethercat::EtherCatError &e) {

        manager = nullptr; // Importante: Redefinir o gerenciador em caso de falha
        client = nullptr;  // Redefinir o cliente também

        message = "Certifique-se que o servo está conectado. " + QString(e.what());
        qDebug() << message;
        emit logMessage(message);
        emit initializationFinished(false);
        return false;
    }
}

// habilita o servo no modo especificado
void ServoMinas::enableServo(int mode) {
    QString message;
    if (client) {
        try{
            // habilita o servo
            client->servoOn(mode);
            // configura as acelerações e velocidade do perfil
            client->setProfileVelocity(engParameters->safetyLimits.speed);
            client->setProfileAcceleration(engParameters->safetyLimits.acceleration);
            client->setProfileDeceleration(engParameters->safetyLimits.deceleration);
            // definir o modo de operação
            QString name_mode;
            if (mode == 0x01) {
                name_mode = "Posição absoluta";
            } else if (mode == 0x06) {
                name_mode = "Homing position";
            } else {
                name_mode = QString("Modo desconhecido: %1").arg(mode); // Lidar com modos desconhecidos

            }
            message = QString("Servo habilitado no modo %1 | %2").arg(mode).arg(name_mode);

        }

        catch(const QException &e){
            qCritical() << "Exceção em habiltar o servo:" << QString(e.what());
            emit logMessage(QString("Erro: ") + QString(e.what()));
        }
    }

    else {
        message = "Cliente não inicializado. Não é possível habilitar o servo.";

    }
    emit logMessage(message);
    qDebug() << message;
}

// desabilita o servo
void ServoMinas::disableServo() {
    QString message;
    if (client) {
        minas_control::MinasInput input;
        input = readInput();
        client->printPDSStatus(input);
        client->printPDSOperation(input);
        client->servoOff();
        message =  "Servo desabilitado.";
    } else {
        message =  "Cliente não inicializado. Não é possível desabilitar o servo.";
    }
    emit logMessage(message);
    qDebug() << message;

}

// reseta os erros
void ServoMinas::resetErrors() {
    QString message;
    if (client) {
        client->reset();
        message = "Erros resetados";

    } else {
        message = "Cliente não inicializado. Não é possível resetar erros.";
        emit logMessage(message);

    }
    qDebug() << message;
}

// configura os limites de segurança
void ServoMinas::configureSafetyLimits() {
    QString message;
    if (client) {
        client->setTrqueForEmergencyStop(engParameters->safetyLimits.emergencyStopTorque);
        client->setOverLoadLevel(engParameters->safetyLimits.overloadLevel);
        client->setOverSpeedLevel(engParameters->safetyLimits.overspeedLevel);
        client->setMotorWorkingRange(engParameters->safetyLimits.motorWorkingLimit);
        client->setInterpolationTimePeriod(engParameters->safetyLimits.timeInterpolationPeriod);
    } else {
        message = "Cliente não inicializado. Não é possível configurar limites de segurança.";
        emit logMessage(message);
        qDebug() << message;
    }

}

/*  conexões de controle da worker:
    - O que ela inicia
    - a espera de terminar e se auto deletar
    - responsividade pelos sinais
*/
// envia o servo para a home com a procura do sinal vindo do próprio motor
void ServoMinas::moveToHome() {

    if (!client) {
        emit logMessage("Operação Homing abortada: Cliente não habilitado");
        return;
    }
    if (!isCommunicationEnabled) {
        emit logMessage("Operação Homing abortada: Comunicação não habilitada");
        return;
    }

    workerThread = new QThread();
    if(client)
        worker = new Worker(client,engParameters);

    worker->moveToThread(workerThread);
 
    connect(workerThread, &QThread::started, worker, &Worker::threadMoveToHome);
    connect(worker, &Worker::finished, workerThread, &QThread::quit);
    connect(worker, &Worker::finished, worker, &Worker::deleteLater);
    connect(workerThread, &QThread::finished, workerThread, [&](){
        workerThread->deleteLater();
        worker = nullptr;
        workerThread = nullptr;
    });

    // log e envio do dado lido
    connect(worker, &Worker::sendLog,this, &ServoMinas::newLogReceived);
    connect(worker, &Worker::inputChangedSignalThread, this, [this](minas_control::MinasInput input){
        emit inputChangedSignal(input);
    });
    // envia para mainwindow que a thread saiu e a execução terminou
    connect(worker, &Worker::finished,this, [this](){
        emit operationStatusSignal(false);
    });

    workerThread->start();
    emit operationStatusSignal(true); // iniciou a operação

}

// envia o servo para uma posição absoluta
void ServoMinas::moveAbsoluteTo(double position, double velocity ) {

    if (!client) {
        emit logMessage("Operação de giro abortada: Cliente não habilitado");
        return;
    }
    if (!isCommunicationEnabled) {
        emit logMessage("Operação de giro abortada: Comunicação não habilitada");
        return;
    }
    workerThread = new QThread();
    if(client)
        worker = new Worker(client,engParameters);

    worker->moveToThread(workerThread);

    connect(workerThread, &QThread::started, worker, [this,position,velocity](){
        worker->threadMoveAbsoluteTo(position,velocity);
    });
    connect(worker, &Worker::finished, workerThread, &QThread::quit);
    connect(worker, &Worker::finished, worker, &Worker::deleteLater);
    connect(workerThread, &QThread::finished, workerThread, [&](){
        workerThread->deleteLater();
        worker = nullptr;
        workerThread = nullptr;
    });
    connect(worker, &Worker::sendLog,this, &ServoMinas::newLogReceived);
    connect(worker, &Worker::inputChangedSignalThread, this, [this](minas_control::MinasInput input){
        emit inputChangedSignal(input);
    });
    // envia para mainwindow que a thread saiu e a execução terminou
    connect(worker, &Worker::finished,this, [this](){
        emit operationStatusSignal(false);
    });

    workerThread->start();
    emit operationStatusSignal(true); // iniciou a operação
}

// envia o servo para uma posição relativa
void ServoMinas::moveOffset(double amount, double velocity,double step) {

    if (!client) {
        emit logMessage("Operação de giro abortada: Cliente não habilitado");
        return;
    }
    if (!isCommunicationEnabled) {
        emit logMessage("Operação de giro abortada: Comunicação não habilitada");
        return;
    }
    workerThread = new QThread();
    if(client)
        worker = new Worker(client,engParameters);

    worker->moveToThread(workerThread);

    connect(workerThread, &QThread::started, worker, [this,amount,velocity,step](){
        worker->threadMoveOffset(amount,velocity,step);
    });
    connect(worker, &Worker::finished, workerThread, &QThread::quit);
    connect(worker, &Worker::finished, worker, &Worker::deleteLater);
    connect(workerThread, &QThread::finished, workerThread, [&](){
        workerThread->deleteLater();
        worker = nullptr;
        workerThread = nullptr;
    });
    connect(worker, &Worker::sendLog,this, &ServoMinas::newLogReceived);
    connect(worker, &Worker::inputChangedSignalThread, this, [this](minas_control::MinasInput input){
        emit inputChangedSignal(input);
    });
    // envia para mainwindow que a thread saiu e a execução terminou
    connect(worker, &Worker::finished,this, [this](){
        emit operationStatusSignal(false);
    });

    workerThread->start();
    emit operationStatusSignal(true); // iniciou a operação
}

// dar update no estado de comunicação
void ServoMinas::updateCommunicationState(bool checked) {
    isCommunicationEnabled = checked;
    if (isCommunicationEnabled) {
        qDebug() << "Servo habilitado. Executando ações...";
        emit logMessage(" ✅ Servo habilitado para comunicação.");
    } else {
        qDebug() << "Servo desabilitado. Apenas logando...";
        emit logMessage(" ❌ Servo desabilitado. Nenhuma ação será realizada.");
    }
}


// ---- ---- ---- ---- ---- Setters and Getters ---- ---- ---- ---- ----

//  ler entradas do servo para uso geral
minas_control::MinasInput ServoMinas::readInput(){
    QString message;
    minas_control::MinasInput input;

    if(client)
    {
        input = client->readInputs();
        emit inputChangedSignal(input);
        return  input;
    }
    else{
        message =  "Cliente não inicializado. Não é possível fazer a leitura das entradas.";
        emit logMessage(message);
        return input;
    }

}

// ler saidas para controle do que o servo recebeu
minas_control::MinasOutput ServoMinas::readOutput(){
    QString message;
    minas_control::MinasOutput output;

    if(client)
    {
        output = client->readOutputs();
        // emit outputChangedSignal(output); // não usado por enquanto
        return  output;
    }
    else{
        message =  "Cliente não inicializado. Não é possível fazer a leitura das saídas.";
        emit logMessage(message);
        return output;
    }
}

// obter a posição atual do servo
uint32_t ServoMinas::getActualPosition()          {    return this->actual_position;      }
