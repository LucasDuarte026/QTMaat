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

bool SerialMicronas::sendCommand(const QString &command)
{
    if (!serial->isOpen()) {
        emit errorOccurred("Erro: Porta serial não está aberta.");
        return false;
    }

    QString formattedCommand = command + "\r";  // Adiciona <CR> (0x0D) ao final
    QByteArray data = formattedCommand.toUtf8();
    serial->write(data);

    qDebug() << "Comando enviado:" << formattedCommand;
    return true;
}

QString SerialMicronas::getLastResponse() const
{
    return lastResponse;
}

void SerialMicronas::handleReadyRead()
{
    QByteArray response = serial->readAll();
    lastResponse = QString::fromUtf8(response).trimmed();
    processResponse(response);
}

void SerialMicronas::processResponse(const QByteArray &response)
{
    QString decodedResponse = QString::fromUtf8(response).trimmed();
    qDebug() << "Resposta recebida:" << decodedResponse;

    if (decodedResponse.contains(":")) {
        QStringList parts = decodedResponse.split(":");
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
