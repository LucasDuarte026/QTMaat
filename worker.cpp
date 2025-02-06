#include "worker.h"
#include <QDebug>
#include <QException>

#define NSEC_PER_SECOND 1e+9

Worker::Worker(minas_control::MinasClient *_client, QObject* parent)
    : QObject(parent)
    , client(_client)
{

    // Set up real-time clock parameters
    period = 4e+6; // 4ms period in nanoseconds
    clock_gettime(CLOCK_REALTIME, &tick);
}

/*
void Worker::moveToHome() {
    if (!servo) {
        qWarning() << "ServoMinas não inicializado.";
        emit sendLog();
        return;
    }

    // Bloqueia o mutex para acesso seguro ao client
    QMutexLocker locker(mutex);

    // Executa a operação de moveToHome
    QString message;
    if(!client || !isCommunicationEnabled )
    {
        if(!client)
        {
            message = "Cliente não inicializado. Não é possível mover para o home.";
            QMessageBox::critical(nullptr, "Erro", "cliente não conectado");
        }
        if(!isCommunicationEnabled)
        {
            message = "Comunicação não habilitada. Não é possível mover para o home.";
            QMessageBox::critical(nullptr, "Erro", "Comunicação não habilitada");
        }
        emit logMessage(message);
        return;
    }
    if (client) {
        if (isCommunicationEnabled) {
            client->setSwitchSpeed(8000000);
            client->setZeroSpeed(8000000);
            client->setHomingAcceleration(33554432);
            client->setHomingTorqueLimit(500);
            client->setHomingDetectionTime(2048);
            client->setHomingDetectionVelocity(33554432);
            client->setCommunicationFuntionExtendedSetup(40);
            client->setHomingReturnSpeedLimit(20000);
            client->setHomingMode(34);  // (On Index Pulse +Ve direction)
            // client->setTouchProbe(7);

            enableServo(0x06);
            memset(&output, 0x00, sizeof(minas_control::MinasOutput));
            output.max_motor_speed = 500;
            output.target_torque = 500;
            output.max_torque = 500;
            output.controlword = 0x001F;
            output.operation_mode = 0x06; // definide de fato o modo de operação para a escrita

            client->writeOutputs(output);

            message = "Homing iniciado";
            emit logMessage(message);

        } else {
            QMessageBox::information(nullptr, "Operação inválida", "Habilite a comunicação entre com o equipamento");
            return;
        }
    }

    int iterationCount = 0;

    while(true) //  debug da operação
    {
        qDebug() << "passou aqui 2";
        if(iterationCount>=20000)
            break;
        Minas_control::MinasInput input = readInput();
        output = readOutput();
        if(iterationCount % 10 ==0){
            std::ostringstream logStream;
            logStream << "err = " << std::hex << std::setw(8) << std::setfill('0') << input.error_code
                      << ", ctrl = " << std::hex << std::setw(8) << std::setfill('0') << output.controlword
                      << ", status = " << std::hex << std::setw(8) << std::setfill('0') << input.statusword
                      << ", op_mode = " << std::hex << std::setw(8) << std::setfill('0') << input.operation_mode
                      << ", pos = " << std::hex << std::setw(8) << std::setfill('0') << input.position_actual_value
                      << ", vel = " << std::hex << std::setw(8) << std::setfill('0') << input.velocity_actual_value
                      << ", tor = " << std::hex << std::setw(8) << std::setfill('0') << input.torque_actual_value
                      << " -> interação: "<< iterationCount;
            emit logMessage(QString::fromStdString(logStream.str()));
        }

        if (input.statusword & 0x1000) { // Target reached
            emit logMessage("✅ Target reached!");

            this->disableServo();
            break;
        }

        // Move to next tick
        tick.tv_nsec += period;
        while (tick.tv_nsec >= NSEC_PER_SECOND)
        {
            tick.tv_nsec -= NSEC_PER_SECOND;
            tick.tv_sec++;
        }

        // detectar overrun de tempo
        struct timespec before;
        clock_gettime(CLOCK_REALTIME, &before);
        double overrun_time = (before.tv_sec + double(before.tv_nsec) / 1e9) -
                              (tick.tv_sec + double(tick.tv_nsec) / 1e9);

        if (overrun_time > 0.0) {
            emit logMessage(QString("⚠️ Overrun detected: %1 ms").arg(overrun_time * 1000));
        }
        clock_nanosleep(CLOCK_REALTIME, TIMER_ABSTIME, &tick, NULL); // esperar o prox tic

        iterationCount++;
    }

    emit sendLog();
}
*/



void Worker::threadMoveAbsoluteTo(double position, double velocity) {
    // QTimer timerLimit;
    // timerLimit.start(50);
    QString message;
    if(!client )
    {
        if(!client)
        {
            message = "Cliente não inicializado. Não é possível mover para o home.";
        }
        emit sendLog(message);
        return;
    }

    // Bloqueia o mutex para acesso seguro ao client
    // QMutexLocker locker(mutex);
    minas_control::MinasOutput output;
    minas_control::MinasInput input;

    if (client) {
        ThreadEnableServo(0x01);
        minas_control::MinasOutput output;
        minas_control::MinasInput input;
        memset(&output, 0x00, sizeof(minas_control::MinasOutput));
        output.target_position = static_cast<int32_t>(0x800000 * position);
        output.max_motor_speed = static_cast<int32_t>(velocity);
        output.target_torque = 500;
        output.max_torque = 500;
        output.controlword = 0x001F;
        output.operation_mode = 0x01;

        client->writeOutputs(output);

        while (!(input.statusword & 0x1000))
        { // bit12 (set-point-acknowledge)
            input = client->readInputs();
        }

        output.controlword &= ~0x0010; // clear new-set-point (bit4)
        client->writeOutputs(output);

        message = QString("🔄 Movendo servo para posição %1 | %2h | com velocidade %3").arg(position).arg(output.target_position).arg(velocity);
    } else {
        message =  "Cliente não inicializado. Não é possível mover para a posição absoluta.";
        emit sendLog(message);
        return;
    }
    emit sendLog(message);

    int iterationCount = 0;

    while(true) //  debug da operação
    {
        if(iterationCount>=20000)
            break;
        input = threadReadInput();
        output = threadReadOutput();
        if(iterationCount % 10 ==0){
            std::ostringstream logStream;
            logStream << "err = " << std::hex << std::setw(8) << std::setfill('0') << input.error_code
                      << ", ctrl = " << std::hex << std::setw(8) << std::setfill('0') << output.controlword
                      << ", status = " << std::hex << std::setw(8) << std::setfill('0') << input.statusword
                      << ", op_mode = " << std::hex << std::setw(8) << std::setfill('0') << input.operation_mode
                      << ", pos = " << std::hex << std::setw(8) << std::setfill('0') << input.position_actual_value
                      << ", vel = " << std::hex << std::setw(8) << std::setfill('0') << input.velocity_actual_value
                      << ", tor = " << std::hex << std::setw(8) << std::setfill('0') << input.torque_actual_value
                      << " -> interação: "<< iterationCount;
            // emit sendLog(QString::fromStdString(logStream.str()));
        }

        if (input.statusword & 0x0400) { // Target reached
            emit sendLog("✅ Target reached!");

            this->threadDisableServo();
            break;
        }

        // Move to next tick
        tick.tv_nsec += period;
        while (tick.tv_nsec >= NSEC_PER_SECOND)
        {
            tick.tv_nsec -= NSEC_PER_SECOND;
            tick.tv_sec++;
        }

        // detectar overrun de tempo
        struct timespec before;
        clock_gettime(CLOCK_REALTIME, &before);
        double overrun_time = (before.tv_sec + double(before.tv_nsec) / 1e9) -
                              (tick.tv_sec + double(tick.tv_nsec) / 1e9);

        if (overrun_time > 0.0) {
            // emit logMessage(QString("⚠️ Overrun detected: %1 ms").arg(overrun_time * 1000));
        }
        clock_nanosleep(CLOCK_REALTIME, TIMER_ABSTIME, &tick, NULL); // esperar o prox tic

        iterationCount++;
    }
    threadDisableServo();
    emit sendLog(QString("✅ Sucesso na operação para a posição %1 | %2h | com velocidade %3").arg(position).arg(static_cast<qulonglong>(output.target_position)).arg(velocity));
    emit finished();
}

void Worker::threadMoveToHome(){

}


//funções privadas
void Worker::threadDisableServo() {
    QString message;
    if (client) {
        minas_control::MinasInput input;
        input = threadReadInput();
        client->printPDSStatus(input);
        client->printPDSOperation(input);
        client->servoOff();
        message =  "Servo desabilitado.";
    } else {
        message =  "Cliente não inicializado. Não é possível desabilitar o servo.";
    }
    emit sendLog(message);

}

void Worker::ThreadEnableServo(int mode) {
    QString message;
    if (client) {
        try{
            client->servoOn(mode);
            client->setProfileVelocity(0x16000000);
            client->setProfileAcceleration(0x80000000);
            client->setProfileDeceleration(0x80000000);

            QString name_mode;
            if (mode == 0x01) {
                name_mode = "Posição absoluta";
            } else if (mode == 0x06) {
                name_mode = "Homing position";
            } else {
                name_mode = QString("Modo desconhecido: %1").arg(mode); // Lidar com modos desconhecidos

            }
            message = QString("Servo habilitado no modo %1|%2").arg(mode).arg(name_mode);
            // configurar as acelerações e velocidades de perfil. Não alterar

        }

        catch(const QException &e){
            message = QString("Erro: ") + QString(e.what());
        }
    }

    else {
        message = "Cliente não inicializado. Não é possível habilitar o servo.";

    }
    emit sendLog(message);
}

minas_control::MinasInput Worker::threadReadInput(){
    minas_control::MinasInput input;
    input = client->readInputs();
    emit inputChangedSignalThread(input);
    return  input;
}

minas_control::MinasOutput Worker::threadReadOutput(){
    minas_control::MinasOutput output;
    output = client->readOutputs();
    // emit outputChangedSignal(output); // não usado por enquanto
    return  output;
}
