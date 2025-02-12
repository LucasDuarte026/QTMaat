#include "serialhandler.h"
#include <QDebug>

SerialHandler::SerialHandler(QObject *parent)
    : QObject(parent)
{
    serial = new QSerialPort(this);
    connect(serial, &QSerialPort::readyRead, this, &SerialHandler::handleReadyRead);
}

SerialHandler::~SerialHandler()
{
    closePort();
}

void SerialHandler::openPort(const QString &portName, int baudRate)
{
    if (serial->isOpen()) {
        serial->close();
    }

    serial->setPortName(portName);
    serial->setBaudRate(baudRate);
    serial->setDataBits(QSerialPort::Data8);
    serial->setParity(QSerialPort::NoParity);
    serial->setStopBits(QSerialPort::OneStop);
    serial->setFlowControl(QSerialPort::NoFlowControl);

    if (!serial->open(QIODevice::ReadWrite)) {
        qWarning() << "Erro ao abrir a porta serial:" << serial->errorString();
    } else {
        qDebug() << "Porta serial aberta com sucesso!";
    }
}

void SerialHandler::closePort()
{
    if (serial->isOpen()) {
        serial->close();
        qDebug() << "Porta serial fechada.";
    }
}

void SerialHandler::sendData(const QByteArray &data)
{
    if (serial->isOpen() && serial->isWritable()) {
        serial->write(data);
        qDebug() << "Enviando:" << data.toHex();
    } else {
        qWarning() << "Não foi possível enviar os dados. Porta fechada?";
    }
}

QByteArray SerialHandler::readData()
{
    QByteArray data;
    if (serial->isOpen() && serial->isReadable()) {
        data = serial->readAll();
        qDebug() << "Recebido:" << data.toHex();
    }
    return data;
}

void SerialHandler::handleReadyRead()
{
    QByteArray data = serial->readAll();
    qDebug() << "Dados brutos recebidos:" << data.toHex();

    decodeBiphase(data);
}

void SerialHandler::decodeBiphase(const QByteArray &data)
{
    QVector<int> decodedBits;

    for (char byte : data) {
        quint8 currentByte = static_cast<quint8>(byte);

        for (int i = 7; i >= 0; --i) {
            int bit = (currentByte >> i) & 1;
            decodedBits.append(bit);
        }
    }

    qDebug() << "Bits extraídos:" << decodedBits;

    QVector<int> decodedData = differentialDecode(decodedBits);
    qDebug() << "Dados decodificados:" << decodedData;

    emit decodedDataReady(decodedData);
}

QVector<int> SerialHandler::differentialDecode(const QVector<int> &bits)
{
    QVector<int> outputBits;
    if (bits.isEmpty()) return outputBits;

    int lastBit = 0;

    for (int i = 0; i < bits.size(); i += 2) {
        if (i + 1 >= bits.size()) break;

        int first = bits[i];
        int second = bits[i + 1];

        if (first == second) {
            qWarning() << "Erro na sincronização Biphase";
            continue;
        }

        int decodedBit = (second == lastBit) ? 0 : 1;
        outputBits.append(decodedBit);

        lastBit = second;
    }

    return outputBits;
}
