#include "servominas.h"
#include <cstring> // For memset
#include <QDebug>
#include <QMessageBox>

ServoMinas::ServoMinas(QString interface)
    : QObject()
    , interfaceName(interface)
    , manager(nullptr)
    , client(nullptr)
    , isEnabled(false)
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
    try {
        manager = new ethercat::EtherCatManager(interfaceName.toStdString());
        qDebug() << "Initializing EtherCAT manager...";

        if (manager->getNumClients() < 1) {
            emit state(false);
            QMessageBox::information(nullptr, "Falha", "Certifique-se que o servo está conectado");
            return;
        }

        client = new minas_control::MinasClient(*manager, 1);
        resetErrors();
        configureSafetyLimits();
        qDebug() << "ServoMinas initialized successfully.";
        emit state(true);
    } catch (const ethercat::EtherCatError &e) {
        emit state(false);
        qDebug() << "Error initializing EtherCAT:" << e.what();
        QMessageBox::critical(nullptr, "Erro", QString("Falha ao inicializar o EtherCAT:\n") + QString(e.what()));
        manager = nullptr;
        client = nullptr;
    }
}

void ServoMinas::resetErrors() {
    if (client) {
        client->reset();
    } else {
        qDebug() << "Client is not initialized. Cannot reset errors.";
    }
}

void ServoMinas::configureSafetyLimits() {
    if (client) {
        client->setTrqueForEmergencyStop(100);
        client->setOverLoadLevel(70);
        client->setOverSpeedLevel(600);
        client->setMotorWorkingRange(0.1);
        client->setInterpolationTimePeriod(4000);
    } else {
        qDebug() << "Client is not initialized. Cannot configure safety limits.";
    }
}

void ServoMinas::enableServo(int mode) {
    if (client) {
        client->servoOn(mode);

        QString name_mode;
        if (mode == 0x01) {
            name_mode = "Posição absoluta";
        } else if (mode == 0x08) {
            name_mode = "Homing position";
        } else {
            name_mode = QString("Unknown mode: %1").arg(mode);
        }
        qDebug() << "Servo habilitado no modo" << mode << "|" << name_mode;
    } else {
        qDebug() << "Client is not initialized. Cannot enable servo.";
    }
}

void ServoMinas::disableServo() {
    if (client) {
        client->servoOff();
        qDebug() << "Servo desabilitado.";
    } else {
        qDebug() << "Client is not initialized. Cannot disable servo.";
    }
}

void ServoMinas::moveToHome() {
    if (client) {
        enableServo(0x08);

        memset(&output, 0x00, sizeof(minas_control::MinasOutput));
        output.target_torque = 500;
        output.max_torque = 500;
        output.controlword = 0x001F;
        output.operation_mode = 0x08;

        client->writeOutputs(output);

        do {
            input = client->readInputs();
        } while (!(input.statusword & 0x0400));

        qDebug() << "Movimento completo. Posição atual:" << input.position_actual_value;
    } else {
        qDebug() << "Client is not initialized. Cannot move to home.";
    }
}

void ServoMinas::moveAbsoluteTo(double position, double velocity) {
    if (client) {
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

        qDebug() << "Movimento completo. Posição atual:" << input.position_actual_value;
    } else {
        qDebug() << "Client is not initialized. Cannot move to absolute position.";
    }
}

void ServoMinas::updateCommunicationState(bool checked) {
    isEnabled = checked;
    if (isEnabled) {
        qDebug() << "Servo habilitado. Executando ações...";
        emit logMessage("✅ Servo habilitado para comunicação com o servo.");
    } else {
        qDebug() << "Servo desabilitado. Apenas logando...";
        emit logMessage("⚠️ Servo desabilitado. Nenhuma ação será realizada.");
    }
}
