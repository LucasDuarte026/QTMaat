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

            QString name_mode;
            if (mode == 0x01) {
                name_mode = "Posição absoluta";
            } else if (mode == 0x06) {
                name_mode = "Homing position";
            } else {
                name_mode = QString("Modo desconhecido: %1").arg(mode); // Lidar com modos desconhecidos

            }
            message = QString("Servo habilitado no modo %1|%2").arg(mode).arg(name_mode);
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


void ServoMinas::moveToHome() {
    QString message;
    if (client) {
        if (isCommunicationEnabled) {
            enableServo(0x06);
            output.controlword = 0x001f; // Procurar as definições de controlWord para os métodos pp e hm (são diferentes para cada)

            output.operation_mode = 0x06; // definide de fato o modo de operação para a escrita

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

    QElapsedTimer timer;
    timer.start();

    qDebug() << "\n\t->\tAcompanhar operação\n";
    int period = 1000000; // Define period INSIDE moveToHome (1ms = 1,000,000 ns)
    for (int i = 0; i <= 2000; i++) {
        minas_control::MinasInput input = client->readInputs();
        minas_control::MinasOutput output = client->readOutputs();

        if (i % 10 == 0) {
            qDebug() << "err =" << QString::number(input.error_code, 16).rightJustified(4, '0')
            << ", ctrl" << QString::number(output.controlword, 16).rightJustified(4, '0')
            << ", status" << QString::number(input.statusword, 16).rightJustified(4, '0')
            << ", op_mode =" << input.operation_mode
            << ", pos =" << QString::number(input.position_actual_value, 16).rightJustified(8, '0')
            << ", vel =" << QString::number(input.velocity_actual_value, 16).rightJustified(8, '0')
            << ", tor =" << QString::number(input.torque_actual_value, 16).rightJustified(8, '0');

            // if (input.statusword & 0x0400) { // target reached (bit 10)
            //     qDebug() << "target reached";
            //     break;
            // }

            qDebug() << "Elapsed time (ms): " << timer.elapsed();

            qDebug() << "Input:";
            qDebug() << " 603Fh" << QString::number(input.error_code, 16).rightJustified(8, '0') << ":Error code";
            qDebug() << " 6041h" << QString::number(input.statusword, 16).rightJustified(8, '0') << ":Statusword";
            qDebug() << " 6061h" << QString::number(input.operation_mode, 16).rightJustified(8, '0') << ":Modes of operation display";
            qDebug() << " 6064h" << QString::number(input.position_actual_value, 16).rightJustified(8, '0') << ":Position actual value";
            qDebug() << " 606Ch" << QString::number(input.velocity_actual_value, 16).rightJustified(8, '0') << ":Velocity actual value";
            qDebug() << " 6077h" << QString::number(input.torque_actual_value, 16).rightJustified(8, '0') << ":Torque actual value";
            qDebug() << " 60B9h" << QString::number(input.touch_probe_status, 16).rightJustified(8, '0') << ":Touch probe status";
            qDebug() << " 60BAh" << QString::number(input.touch_probe_posl_pos_value, 16).rightJustified(8, '0') << ":Touch probe pos1 pos value";
            qDebug() << " 60FDh" << QString::number(input.digital_inputs, 16).rightJustified(8, '0') << ":Digital inputs";

            qDebug() << "Output:";
            qDebug() << " 6040h" << QString::number(output.controlword, 16).rightJustified(8, '0') << ":Controlword";
            qDebug() << " 6060h" << QString::number(output.operation_mode, 16).rightJustified(8, '0') << ":Mode of operation";
            qDebug() << " 6071h" << QString::number(output.target_torque, 16).rightJustified(8, '0') << ":Target Torque";
            qDebug() << " 6072h" << QString::number(output.max_torque, 16).rightJustified(8, '0') << ":Max Torque";
            qDebug() << " 607Ah" << QString::number(output.target_position, 16).rightJustified(8, '0') << ":Target Position";
            qDebug() << " 6080h" << QString::number(output.max_motor_speed, 16).rightJustified(8, '0') << ":Max motor speed";
            qDebug() << " 60B8h" << QString::number(output.touch_probe_function, 16).rightJustified(8, '0') << ":Touch Probe function";
            qDebug() << " 60FFh" << QString::number(output.target_velocity, 16).rightJustified(8, '0') << ":Target Velocity";
            qDebug() << " 60B0h" << QString::number(output.position_offset, 16).rightJustified(8, '0') << ":Position Offset";
        }


        while (timer.elapsed() * 1000 < period) {
            QCoreApplication::processEvents();
        }
        timer.restart();

    }
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

        message =  QString("Movimento completo. Posição atual:").arg(input.position_actual_value);
    } else {
        message =  "Cliente não inicializado. Não é possível mover para a posição absoluta.";
    }
    emit logMessage(message);
    qDebug() << message;
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
