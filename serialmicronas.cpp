#include "serialmicronas.h"
#include <QDebug>

SerialMicronas::SerialMicronas(QObject *parent) : QObject(parent)
{
    serial = new QSerialPort(this);
    connect(serial, &QSerialPort::readyRead, this, &SerialMicronas::handleReadyRead);
}

SerialMicronas::~SerialMicronas()
{
    closePort();
}

bool SerialMicronas::openPort(const QString &portName)
{
    if (serial->isOpen())
        serial->close();

    serial->setPortName(portName);
    serial->setBaudRate(38400);
    serial->setDataBits(QSerialPort::Data8);
    serial->setParity(QSerialPort::EvenParity);
    serial->setStopBits(QSerialPort::OneStop);
    serial->setFlowControl(QSerialPort::NoFlowControl);

    if (!serial->open(QIODevice::ReadWrite)) {
        emit errorOccurred("Erro ao abrir a porta serial: " + serial->errorString());
        return false;
    }

    qDebug() << "Porta serial" << portName << "aberta com sucesso.";
    return true;
}

void SerialMicronas::closePort()
{
    if (serial->isOpen()) {
        serial->close();
        qDebug() << "Porta serial fechada.";
    }
}

bool SerialMicronas::sendCommand(const QByteArray &command)
{
    if (!serial->isOpen()) {
        emit errorOccurred("Erro: Porta serial não está aberta.");
        return false;
    }

    QByteArray formattedCommand = command + "\r";  // Adiciona <CR> (0x0D) ao final
    serial->write(formattedCommand);

    qDebug() << "Comando enviado:" << formattedCommand;
    return true;
}

QByteArray SerialMicronas::readResponse()
{
    if (!serial->isOpen()) {
        emit errorOccurred("Erro: Porta serial não está aberta.");
        return QByteArray();
    }

    if (serial->waitForReadyRead(100)) {
        return serial->readAll().trimmed();
    }
    return QByteArray();
}

bool SerialMicronas::setBaseAddress(uint8_t baseAddress)
{
    QByteArray command;
    command.append("xxsb");
    command.append(QString("%1").arg(baseAddress, 2, 16, QChar('0')).toUpper().toUtf8());
    command.append("0000");
    command.append(QString::number(calculateCRC(baseAddress, 4), 16).toUpper().toUtf8());

    sendCommand(command);
    QByteArray response = readResponse();
    return !response.isEmpty() && response.contains("0:000000");
}

QByteArray SerialMicronas::readRegister(uint8_t address)
{
    QByteArray command;
    command.append("xxr");
    command.append(QString("%1").arg(address, 2, 16, QChar('0')).toUpper().toUtf8());

    sendCommand(command);
    QByteArray response = readResponse();

    if (!response.isEmpty() && response.startsWith("0:")) {
        return response.mid(2, response.size() - 2); // Remove código de status
    }

    emit errorOccurred("Erro ao ler o registro.");
    return QByteArray();
}

bool SerialMicronas::writeRegister(uint8_t address, uint16_t data)
{
    QByteArray command;
    command.append("xxw");
    command.append(QString("%1").arg(address, 2, 16, QChar('0')).toUpper().toUtf8());
    command.append(QString("%1").arg(data, 4, 16, QChar('0')).toUpper().toUtf8());
    command.append(QString::number(calculateCRC((address << 16) | data, 20), 16).toUpper().toUtf8());

    sendCommand(command);
    QByteArray response = readResponse();

    return !response.isEmpty() && response.contains("0:000000");
}

uint8_t SerialMicronas::calculateParity(uint8_t command, uint8_t address)
{
    uint8_t parity = command ^ address;
    parity = parity ^ (parity >> 4) ^ (parity >> 2) ^ (parity >> 1);
    return parity & 1;
}

uint8_t SerialMicronas::calculateCRC(uint32_t data, uint8_t size)
{
    const uint8_t CRC_POLY = 0x13; // x^4 + x + 1
    uint8_t crc = 0x00;

    for (int i = 0; i < size; i++) {
        if (((crc << 3) & 0x1) != ((data >> (size - 1 - i)) & 1)) {
            crc = (crc << 1) ^ CRC_POLY;
            crc &= 0xF;
        } else {
            crc <<= 1;
        }
    }
    return crc & 0xF;
}

void SerialMicronas::handleReadyRead()
{
    QByteArray response = serial->readAll().trimmed();
    processResponse(response);
}

void SerialMicronas::processResponse(const QByteArray &response)
{
    qDebug() << "Resposta recebida:" << response;

    if (response.contains(":")) {
        QStringList parts;
        for (const QByteArray &part : response.split(':')) {
            parts.append(QString::fromUtf8(part));
        }
        if (parts.size() > 1) {
            int errorCode = parts[0].toInt();
            QString resultData = parts[1];

            if (errorCode != 0) {
                emit errorOccurred("Erro na resposta: " + interpretErrorCode(errorCode));
            } else {
                emit responseReceived(resultData);
            }
        }
    }
}

QString SerialMicronas::interpretErrorCode(int errorCode)
{
    switch (errorCode) {
    case 0: return "Sem erro";
    case 1: return "Erro de reconhecimento";
    case 2: return "Segundo erro de reconhecimento";
    case 3: return "Comando inválido para o modo selecionado";
    case 13: return "Erro de leitura de dados";
    case 14: return "Parâmetro de comando inválido";
    case 15: return "Comando inválido";
    default: return "Erro desconhecido (" + QString::number(errorCode) + ")";
    }
}
