#include <iostream>
#include <QDebug>
#include <QException>
#include "worker.h"
#include "datatypes.h"

#define NSEC_PER_SECOND 1e+9

Worker::Worker(minas_control::MinasClient *_client,EngParameters*_engParameters, QObject* parent)
    : QObject(parent)
    , client(_client)
    , engParameters(_engParameters)
{

    // Set up real-time clock parameters
    period = 4e+6; // 4ms period in nanoseconds
    clock_gettime(CLOCK_REALTIME, &tick);
}


void Worker::threadMoveToHome() {
    QString message;
    if(!client )
    {
        message = "Cliente não inicializado. Não é possível mover para o home.";
        emit sendLog(message);
        return;
    }
    minas_control::MinasOutput output;
    minas_control::MinasInput input;
    if (client) {
        client->setSwitchSpeed(engParameters->homing.switchSpeed);
        client->setZeroSpeed(engParameters->homing.zeroSpeed);
        client->setHomingAcceleration(engParameters->homing.homingAcceleration);
        client->setHomingTorqueLimit(engParameters->homing.homingTorqueLimit);
        client->setHomingDetectionTime(engParameters->homing.homingDetectionTime);
        client->setHomingDetectionVelocity(engParameters->homing.homingDetectionVelocity);
        client->setCommunicationFuntionExtendedSetup(engParameters->homing.communicationFunctionExtendedSetup);
        client->setHomingReturnSpeedLimit(engParameters->homing.homingReturnSpeedLimit);
        client->setHomingMode(engParameters->homing.homingMode);  // (On Index Pulse +Ve direction)
        client->setTouchProbe(engParameters->homing.touchprobeFunction);

        threadEnableServo(0x06);
        memset(&output, 0x00, sizeof(minas_control::MinasOutput));
        output.max_motor_speed = engParameters->homing.max_motor_speed;
        output.target_torque = engParameters->homing.target_torque;
        output.max_torque = engParameters->homing.max_torque;
        output.controlword = 0x001F;
        output.operation_mode = 0x06; // definide de fato o modo de operação para a escrita

        client->writeOutputs(output);

        message = QString("🔄 Movendo servo para posição 0 -> Homing iniciado");
        emit sendLog(message);


    }

    int iterationCount = 0;

    while(true) //  debug da operação
    {
        if(iterationCount>=20000){
            emit sendLog("FALHA EM HOMING Target NOT reached!");
            break;
        }
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

        if (input.statusword & 0x1000) { // Target reached
            emit sendLog("✅ Target reached!");
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
            // emit sendLog(QString("⚠️ Overrun detected: %1 ms").arg(overrun_time * 1000));
        }
        clock_nanosleep(CLOCK_REALTIME, TIMER_ABSTIME, &tick, NULL); // esperar o prox tic

        iterationCount++;
    }
    threadDisableServo();

    emit sendLog("✅ Sucesso na operação de homing");
    emit finished();
}

void Worker::threadMoveAbsoluteTo(double position, double velocity) {

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

    minas_control::MinasOutput output;
    minas_control::MinasInput input;

    if (client) {
        threadEnableServo(0x01);
        memset(&output, 0x00, sizeof(minas_control::MinasOutput));
        output.target_position = static_cast<int32_t>((0x800000 * position)/360);
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

        message = QString("🔄 Movendo servo para posição %1º | %2h | com velocidade %3").arg(position).arg(QString::number(output.target_position, 16).toUpper()).arg(velocity);
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
    emit sendLog(QString("✅ Sucesso na operação para a posição atual: %1º |  %2h | com velocidade %3").arg(position).arg(input.position_actual_value).arg(velocity));

    emit finished();
}

void Worker::threadMoveOffset(double amount, double velocity, double step) {
    QString message;
    if(!client )
    {
        if(!client)
        {
            message = "Cliente não inicializado. Não é possível fazer o jog.";
        }
        emit sendLog(message);
        return;
    }

    minas_control::MinasOutput output;
    minas_control::MinasInput input;

    int32_t offset_units = static_cast<int32_t>((amount * step) * (0x800000 / 360.0));

    if (client) {
        threadEnableServo(0x01);
        input = client->readInputs();
        memset(&output, 0x00, sizeof(minas_control::MinasOutput));
        output.target_position = static_cast<int32_t>(input.position_actual_value) + offset_units;
        output.max_motor_speed = static_cast<int32_t>(velocity);
        output.target_torque = engParameters->positioning.targetTorque;
        output.max_torque = engParameters->positioning.maxTorque;
        output.controlword = 0x001F;
        output.operation_mode = 0x01;

        std::cout << "-> output.target_position: " << output.target_position << "| input.position_actual_value: " <<input.position_actual_value << "\n";

        client->writeOutputs(output);

        while (!(input.statusword & 0x1000))
        { // bit12 (set-point-acknowledge)
            input = client->readInputs();
        }

        output.controlword &= ~0x0010; // clear new-set-point (bit4)
        client->writeOutputs(output);

        message = QString("🔄 Movendo servo para posição de  %1º | %2h | com velocidade %3").arg(amount*step).arg(QString::number(output.target_position, 16).toUpper()).arg(velocity);
    } else {
        message =  "Cliente não inicializado. Não foi possivel executar executar o posicionamento relativo.";
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
    emit sendLog(QString("✅ Sucesso na operação para a posição relativa de %1º | para:  %2h | com velocidade %3").arg(amount*step).arg(output.target_position).arg(velocity));
    emit finished();
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

void Worker::threadEnableServo(int mode) {
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
