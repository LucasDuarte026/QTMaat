#include <iostream>
#include "servominas.h"
#include <cstring> // Para memset
#include <QDebug>
#include <QMessageBox>

#define NSEC_PER_SECOND 1e+9

ServoMinas::ServoMinas(QString interface)
    : QObject()
    , interfaceName(interface)
    , manager(nullptr)
    , client(nullptr)
    , isCommunicationEnabled(false)
    , inOperation(false)
{

    // Set up real-time clock parameters
    period = 4e+6; // 4ms period in nanoseconds
    clock_gettime(CLOCK_REALTIME, &tick);
}

ServoMinas::~ServoMinas() {
    if (client) {
        client->servoOff();
        delete client;
    }
    if (manager) {
        delete manager;
    }
}

bool ServoMinas::initialize() {
    QString message;
    try {
        manager = new ethercat::EtherCatManager(interfaceName.toStdString());
        qDebug() << "Inicializando o gerenciador EtherCAT...";

        if (manager->getNumClients() < 1) {
            emit initializationFinished(false);
            QMessageBox::information(nullptr, "Falha", "Certifique-se que o servo está conectado");
            message = "Certifique-se que o servo está conectado";
            emit logMessage(message);
            return false; // Importante: Sair cedo se nenhum cliente for encontrado
        }

        client = new minas_control::MinasClient(*manager, 1);
        resetErrors();
        configureSafetyLimits();


        message = "ServoMinas inicializado com sucesso. ";
        emit logMessage(message);
        qDebug() << message ;
        emit initializationFinished(true); // Emitir sinal de sucesso
        return true;

    } catch (const ethercat::EtherCatError &e) {
        emit initializationFinished(false);
        message = "Certifique-se que o servo está conectado. " + QString(e.what());

        QMessageBox::critical(nullptr, "Erro", QString("Falha ao inicializar o EtherCAT:\n") + QString(e.what()));
        manager = nullptr; // Importante: Redefinir o gerenciador em caso de falha
        client = nullptr;  // Redefinir o cliente também

        qDebug() << message;
        emit logMessage(message);
        return false;
    }
}

void ServoMinas::enableServo(int mode) {
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
            qCritical() << "Exceção em habiltar o servo:" << QString(e.what());
            emit logMessage(QString("Erro durante moveToHome: ") + QString(e.what()));
        }
    }

    else {
        message = "Cliente não inicializado. Não é possível habilitar o servo.";

    }
    emit logMessage(message);
    qDebug() << message;
}

void ServoMinas::disableServo() {
    QString message;
    if (client) {

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

void ServoMinas::configureSafetyLimits() {
    QString message;
    if (client) {
        client->setTrqueForEmergencyStop(100);
        client->setOverLoadLevel(70);
        client->setOverSpeedLevel(600);
        client->setMotorWorkingRange(0.1);
        client->setInterpolationTimePeriod(4000);
    } else {
        message = "Cliente não inicializado. Não é possível configurar limites de segurança.";
        emit logMessage(message);
        qDebug() << message;
    }

}


minas_control::MinasOutput ServoMinas::readOutput(){
    QString message;
    minas_control::MinasOutput output;

    if(client)
    {
        output = client->readOutputs();
        emit dataChanged();
        return  output;
    }
    else{
        message =  "Cliente não inicializado. Não é possível fazer a leitura das saídas.";
        emit logMessage(message);
        return output;
    }
}

void ServoMinas::moveToHome() {
    QString message;
    if (client) {
        if (isCommunicationEnabled) {
            inOperation = true;
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
    } else {
        message = "Cliente não inicializado. Não é possível mover para o home.";
        emit logMessage(message);
        return;
    }
    qDebug() << "passou aqui";
    iterationCount = 0;

    while(true) //  debug da operação
    {
        qDebug() << "passou aqui 2";
        if(iterationCount>=20000)
            break;
        input = readInput();
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

    emit logMessage(" -> Homing realizado com sucesso");


}

void ServoMinas::moveAbsoluteTo(double position, double velocity ) {
    QString message;
    if (client && isCommunicationEnabled) {
        enableServo(0x01);

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

        message = QString("🔄 Movendo servo para posição %1 |%2| com velocidade %3").arg(position).arg(output.target_position).arg(velocity);
    } else {
        message =  "Cliente não inicializado. Não é possível mover para a posição absoluta.";
    }
    emit logMessage(message);

    iterationCount = 0;

    while(true) //  debug da operação
    {
        if(iterationCount>=20000)
            break;
        input = readInput();
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

        if (input.statusword & 0x0400) { // Target reached
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

    emit logMessage(QString("✅ Sucesso na operação para a posição %1 |%2| com velocidade %3").arg(position).arg(static_cast<qulonglong>(output.target_position)).arg(velocity));

}

void ServoMinas::moveAngularTo(double angle, double velocity){ // Move para uma posição angular

}
void ServoMinas::updateCommunicationState(bool checked) {
    isCommunicationEnabled = checked;
    if (isCommunicationEnabled) {
        qDebug() << "Servo habilitado. Executando ações...";
        emit logMessage(" ✅ Servo habilitado para comunicação com o servo.");
    } else {
        qDebug() << "Servo desabilitado. Apenas logando...";
        emit logMessage(" ❌ Servo desabilitado. Nenhuma ação será realizada.");
    }
}


// ---- ---- ---- ---- ---- Setters and Getters ---- ---- ---- ---- ----


minas_control::MinasInput ServoMinas::readInput(){
    QString message;
    minas_control::MinasInput input;

    if(client)
{
        input = client->readInputs();
        emit dataChanged();
        return  input;
    }
    else{
        message =  "Cliente não inicializado. Não é possível fazer a leitura das entradas.";
        emit logMessage(message);
        return input;
    }

}


void ServoMinas::setActualPosition(uint32_t value){    this->actual_position = value;     }

uint32_t ServoMinas::getActualPosition()          {    return this->actual_position;      }
