#include "serialmicronas.h"
#include <QDebug>

#define PROGSPEED 1000
#define HALFPROGSPEED (PROGSPEED / 2)
#define THRESHOLD (HALFPROGSPEED * 1.2)

SerialMicronas::SerialMicronas(QObject *parent) : QObject(parent)
{
    serial = new QSerialPort(this);
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

bool SerialMicronas::calculateParityBit()
{
    uint8_t ones = 0;
    for (int i = 0; i < 8; i++) {
        if (buffer[i] == 1) { ones++; }
    }
    return 1 - (ones % 2);
}

void SerialMicronas::sendCommand(int length)
{
    if (!serial->isOpen()) return;

    sendSync();

    for (int i = 0; i < length; i++) {
        if (buffer[i] == 1) writeLogicalOne();
        else writeLogicalZero();
    }

    outputState = 1;
}

void SerialMicronas::sendSync()
{
    serial->write("H");
    serial->waitForBytesWritten(2);
    serial->write("L");
    serial->waitForBytesWritten(2);
    serial->write("H");
    serial->waitForBytesWritten(4);
    serial->write("L");
    serial->waitForBytesWritten(PROGSPEED);
}

void SerialMicronas::writeLogicalOne()
{
    serial->write("1");
    serial->waitForBytesWritten(HALFPROGSPEED);
    serial->write("0");
    serial->waitForBytesWritten(HALFPROGSPEED);
}

void SerialMicronas::writeLogicalZero()
{
    serial->write("1");
    serial->waitForBytesWritten(PROGSPEED);
}

uint8_t SerialMicronas::getError() { return errorCode; }

uint8_t SerialMicronas::readResponse()
{
    if (!serial->isOpen()) return 1;

    serial->waitForReadyRead(10);
    QByteArray responseData = serial->readAll();

    if (responseData.isEmpty()) return 1;

    response = responseData.toUInt();

    uint8_t responseCRC = calculateCRC(response, 16);
    if (responseCRC != responseData.at(responseData.size() - 1)) return 2;

    return 0;
}

uint8_t SerialMicronas::calculateCRC(uint32_t data, uint8_t length)
{
    const uint8_t CRC_POLY = 0x13;
    uint8_t crc = 0x00;

    for (int i = length; i > -1; i--) {
        uint8_t bit_in = (data >> i) & 0x1;
        uint8_t bit_out = (crc >> 3) & 0x1;
        uint8_t bit_comp = (bit_out ^ bit_in) & 0x1;
        crc = (crc << 1) + bit_comp;
        crc = crc ^ (bit_comp << 1);
    }

    return crc & 0xF;
}

bool SerialMicronas::setBaseAddress(uint16_t base)
{
    buffer[0] = 0;
    buffer[1] = 1;
    buffer[2] = 1;

    for (int i = 3; i <= 8; i++) {
        buffer[i] = 0;
    }

    buffer[8] = calculateParityBit();
    buffer[9] = 0;

    for (int i = 0; i < 16; i++) {
        buffer[25 - i] = (base >> i) & 0x1;
    }

    sendCommand(30);
    return waitForACK();
}

uint16_t SerialMicronas::readAddress(uint8_t address)
{
    buffer[0] = 0;
    buffer[1] = 0;
    buffer[2] = 1;

    for (int i = 0; i < 5; i++) {
        buffer[7 - i] = (address >> i) & 0x1;
    }

    buffer[8] = calculateParityBit();

    sendCommand(9);
    errorCode = readResponse();

    return response;
}

bool SerialMicronas::writeAddress(uint8_t address, uint16_t data)
{
    buffer[0] = 1;
    buffer[1] = 1;
    buffer[2] = 0;

    for (int i = 0; i < 5; i++) {
        buffer[7 - i] = (address >> i) & 0x1;
    }

    buffer[8] = calculateParityBit();
    buffer[9] = 0;

    for (int i = 0; i < 16; i++) {
        buffer[25 - i] = (data >> i) & 0x1;
    }

    sendCommand(30);
    return waitForACK();
}

bool SerialMicronas::waitForACK()
{
    if (!serial->isOpen()) return false;

    serial->waitForReadyRead(10);
    QByteArray ack = serial->readAll();

    return !ack.isEmpty();
}
