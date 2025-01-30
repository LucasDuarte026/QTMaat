#include <iostream>
#include "servominas.h"
#include <cstring> // Para memset
#include <QDebug>
#include <QMessageBox>

ServoMinas::ServoMinas(QString interface)
    : QObject()
    , interfaceName(interface)
    , manager(nullptr)
    , client(nullptr)
    , isCommunicationEnabled(false)
{
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

void ServoMinas::initialize() {
    QString message;
    try {
        manager = new ethercat::EtherCatManager(interfaceName.toStdString());
        qDebug() << "Inicializando o gerenciador EtherCAT...";

        if (manager->getNumClients() < 1) {
            emit state(false);
            QMessageBox::information(nullptr, "Falha", "Certifique-se que o servo está conectado");
            return; // Importante: Sair cedo se nenhum cliente for encontrado
            message = "Certifique-se que o servo está conectado";
            emit logMessage(message);
        }

        client = new minas_control::MinasClient(*manager, 1);
        resetErrors();
        configureSafetyLimits();


        message = "ServoMinas inicializado com sucesso. ";
        emit logMessage(message);
        qDebug() << message ;
        emit state(true); // Emitir sinal de sucesso

    } catch (const ethercat::EtherCatError &e) {
        emit state(false);
        message = "Certifique-se que o servo está conectado. " + QString(e.what());

        QMessageBox::critical(nullptr, "Erro", QString("Falha ao inicializar o EtherCAT:\n") + QString(e.what()));
        manager = nullptr; // Importante: Redefinir o gerenciador em caso de falha
        client = nullptr;  // Redefinir o cliente também

        qDebug() << message;
        emit logMessage(message);
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

        minas_control::MinasInput input = client->readInputs();
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


bool ServoMinas::debugOperation(int durationMs) {
    if (!client || !isCommunicationEnabled) {
        emit logMessage("Cliente não inicializado ou comunicação desabilitada.");
        return false;
    }

    quint64 start_time = QDateTime::currentMSecsSinceEpoch();
    quint64 end_time = start_time + durationMs;

    // Send the command to your device *before* starting the observation
    // This part depends on how you send commands.  Examples:
    // client->sendCommand(yourCommand);  // If you have a sendCommand method
    // Or write directly to registers:
    // client->writeOutputs(yourOutputStructure);


    while (QDateTime::currentMSecsSinceEpoch() < end_time) {
        minas_control::MinasInput input = client->readInputs();
        minas_control::MinasOutput output = client->readOutputs();

        if ((QDateTime::currentMSecsSinceEpoch() - start_time) % 40 == 0) { // Log every 40ms
            std::cout << "err = " << std::hex << std::setw(4) << std::setfill('0') << input.error_code
                      << ", ctrl = " << std::hex << std::setw(4) << std::setfill('0') << output.controlword
                      << ", status = " << std::hex << std::setw(4) << std::setfill('0') << input.statusword
                      << ", op_mode = " << std::dec << input.operation_mode
                      << ", pos = " << std::hex << std::setw(8) << std::setfill('0') << input.position_actual_value
                      << ", vel = " << std::hex << std::setw(8) << std::setfill('0') << input.velocity_actual_value
                      << ", tor = " << std::hex << std::setw(8) << std::setfill('0') << input.torque_actual_value
                      << " -> Tempo decorrido: " << std::dec << (QDateTime::currentMSecsSinceEpoch() - start_time) << " ms" << std::endl;

            std::cout << "Raw 6060h (decimal): " << output.operation_mode << std::endl;
            std::cout << "Raw 6061h (decimal): " << input.operation_mode << std::endl;

            std::cout << "Input:" << std::endl;
            std::cout << " 603Fh " << std::hex << std::setw(8) << std::setfill('0') << input.error_code << ":Error code" << std::endl;
            std::cout << " 6041h " << std::hex << std::setw(8) << std::setfill('0') << input.statusword << ":Statusword" << std::endl;
            std::cout << " 6061h " << std::hex << std::setw(8) << std::setfill('0') << input.operation_mode << ":Modes of operation display" << std::endl;
            std::cout << " 6064h " << std::hex << std::setw(8) << std::setfill('0') << input.position_actual_value << ":Position actual value" << std::endl;
            std::cout << " 606Ch " << std::hex << std::setw(8) << std::setfill('0') << input.velocity_actual_value << ":Velocity actual value" << std::endl;
            std::cout << " 6077h " << std::hex << std::setw(8) << std::setfill('0') << input.torque_actual_value << ":Torque actual value" << std::endl;
            std::cout << " 60B9h " << std::hex << std::setw(8) << std::setfill('0') << input.touch_probe_status << ":Touch probe status" << std::endl;
            std::cout << " 60BAh " << std::hex << std::setw(8) << std::setfill('0') << input.touch_probe_posl_pos_value << ":Touch probe pos1 pos value" << std::endl;
            std::cout << " 60FDh " << std::hex << std::setw(8) << std::setfill('0') << input.digital_inputs << ":Digital inputs" << std::endl;

            std::cout << "Output:" << std::endl;
            std::cout << " 6040h " << std::hex << std::setw(8) << std::setfill('0') << output.controlword << ":Controlword" << std::endl;
            std::cout << " 6060h " << std::hex << std::setw(8) << std::setfill('0') << output.operation_mode << ":Mode of operation" << std::endl;
            std::cout << " 6071h " << std::hex << std::setw(8) << std::setfill('0') << output.target_torque << ":Target Torque" << std::endl;
            std::cout << " 6072h " << std::hex << std::setw(8) << std::setfill('0') << output.max_torque << ":Max Torque" << std::endl;
            std::cout << " 607Ah " << std::hex << std::setw(8) << std::setfill('0') << output.target_position << ":Target Position" << std::endl;
            std::cout << " 6080h " << std::hex << std::setw(8) << std::setfill('0') << output.max_motor_speed << ":Max motor speed" << std::endl;
            std::cout << " 60B8h " << std::hex << std::setw(8) << std::setfill('0') << output.touch_probe_function << ":Touch Probe function" << std::endl;
            std::cout << " 60FFh " << std::hex << std::setw(8) << std::setfill('0') << output.target_velocity << ":Target Velocity" << std::endl;
            std::cout << " 60B0h " << std::hex << std::setw(8) << std::setfill('0') << output.position_offset << ":Position Offset" << std::endl;
        }

        QCoreApplication::processEvents(); // manter a aplicação rodando
    }

    emit logMessage("Observação concluída.");
    return true;
}

void ServoMinas::moveToHome() {
    QString message;
    if (client) {
        if (isCommunicationEnabled) {
            enableServo(0x06);
            client->setSwitchSpeed(8000000);
            client->setZeroSpeed(8000000);
            client->setHomingAcceleration(33554432);
            client->setHomingTorqueLimit(500);
            client->setHomingDetectionTime(2048);
            client->setHomingDetectionVelocity(33554432);
            client->setCommunicationFuntionExtendedSetup(40);
            client->setHomingReturnSpeedLimit(20000);
            client->setHomingMode(34);  // (On Index Pulse +Ve direction)
            client->setTouchProbe(7);

            memset(&output, 0x00, sizeof(minas_control::MinasOutput));
            output.target_torque = 500;
            output.max_torque = 500;
            output.controlword = 0x001F;
            output.operation_mode = 0x06; // definide de fato o modo de operação para a escrita

            client->writeOutputs(output);

            message = "Homing iniciado";
        } else {
            QMessageBox::information(nullptr, "Operação inválida", "Habilite a comunicação entre com o equipamento");
            return;
        }
    } else {
        message = "Cliente não inicializado. Não é possível mover para o home.";
        emit logMessage(message);
        qDebug() << message;
        return;
    }

    emit logMessage(message);
    qDebug() << message;


    // bool success = debugOperation(4000);

}

void ServoMinas::moveAbsoluteTo(double position, double velocity) {
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

        do {
            input = client->readInputs();
        } while (!(input.statusword & 0x0400));

        message =  QString("Movimento completo. Posição atual: %1").arg(input.position_actual_value);
    } else {
        message =  "Cliente não inicializado. Não é possível mover para a posição absoluta.";
    }
    emit logMessage(message);
    qDebug() << message;
    bool success = debugOperation(4000);
    if(success)
        emit logMessage("Operação de movimentação absoluta para " + QString::number(position) + "foi um sucesso");
    else
        emit logMessage("Operação de movimentação absoluta para " + QString::number(position) + "fracassou");

}

void ServoMinas::updateCommunicationState(bool checked) {
    isCommunicationEnabled = checked;
    if (isCommunicationEnabled) {
        qDebug() << "Servo habilitado. Executando ações...";
        emit logMessage("✅ Servo habilitado para comunicação com o servo.");
    } else {
        qDebug() << "Servo desabilitado. Apenas logando...";
        emit logMessage("⚠️ Servo desabilitado. Nenhuma ação será realizada.");
    }
}
