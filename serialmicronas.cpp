#include "serialmicronas.h"
#include <QDebug>

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

QString SerialMicronas::sendCommand(const QString &command)
{
    if (!serial->isOpen()) {
        emit errorOccurred("Erro: Porta serial não está aberta.");
        return QString();
    }

    QString formattedCommand = command + "\r\n";
    serial->write(formattedCommand.toUtf8());
    if (!serial->waitForReadyRead(500)) {
        emit errorOccurred("Erro: Nenhuma resposta recebida.");
        return QString();
    }

    QByteArray responseData = serial->readAll().trimmed();
    qDebug() << "Resposta bruta do sensor:" << responseData.toHex() << " | Como string:" << responseData;
    return responseData;
}

QString SerialMicronas::getFirmwareVersion()
{
    QString response = sendCommand("?v");
    if (response.startsWith("0:")) {
        return response.mid(2);
    } else {
        emit errorOccurred("Erro: Resposta inesperada ao obter versão do firmware.");
        return QString();
    }
}

bool SerialMicronas::setBoardMode(char mode)
{
    QString response = sendCommand("sm" + QString(mode));
    if (response.startsWith("0:0000") && response.endsWith(mode)) {
        return true;
    } else {
        emit errorOccurred("Erro: Falha ao configurar o modo da placa.");
        return false;
    }
}
