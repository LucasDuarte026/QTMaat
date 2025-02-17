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

    return processResponse(responseData);
}

QString SerialMicronas::processResponse(const QByteArray &response)
{
    if (response.isEmpty()) {
        emit errorOccurred("Erro: Resposta vazia do sensor.");
        return QString();
    }

    char status = response.at(0);
    if (status != '0') {
        static const QMap<char, QString> errorMessages = {
            {'1', "Acknowledge error"},
            {'2', "2nd acknowledge error"},
            {'3', "Invalid command for selected mode"},
            {'D', "Data read error"},
            {'E', "Invalid command parameter"},
            {'F', "Invalid command"}
        };

        QString errorMessage = errorMessages.contains(status) ? errorMessages[status] : "Erro desconhecido";
        emit errorOccurred("Erro recebido do sensor: " + errorMessage + " (Código: " + QString(status) + ")");
        return QString();
    }

    return response;
}

QString SerialMicronas::getFirmwareVersion()
{
    return sendCommand("?v");
}

bool SerialMicronas::setBoardMode(char mode)
{
    QString response = sendCommand("sm" + QString(mode));
    if (!response.isEmpty() && response.startsWith("0:0000") && response.endsWith(mode)) {
        if(mode == 'A')
            emit messageMicronas_signal("Comunicação configurada por Vsup");
        else if(mode == 'C'){
            emit messageMicronas_signal("Comunicação configurada por Vout");
        }
        return true;
    } else {
        emit errorOccurred("Erro: Falha ao configurar o modo da placa.");
        return false;
    }
}
int calculateParity(const QString &commandStr, const QString &addressStr)
{
    // Converter as strings para inteiros (assumindo que as strings representam valores hexadecimais)
    bool ok;
    int command = commandStr.toInt(&ok, 16);
    if (!ok) {
        // Lidar com o erro de conversão
        return -1;
    }
    int address = addressStr.toInt(&ok, 16);
    if (!ok) {
        // Lidar com o erro de conversão
        return -1;
    }

    // Calcular a paridade, iniciando com o comando
    int parity = command;
    parity ^= address;
    parity ^= (parity >> 1);
    parity ^= (parity >> 2);
    parity ^= (parity >> 4);
    return parity & 1;
}

int crcCalculate(int data, int size)
{
    const unsigned char CRC_POLY = 0x13; // x^4 + x + 1
    int i;
    unsigned char crc = 0x00;
    for (i = 0; i < size; i++)
    {
        if ((crc << 3 & 0x1) != (data >> (size - 1 - i) & 1))
        {
            crc = crc << 1;
            crc ^= CRC_POLY;
            crc &= 0xF;
        }
        else
            crc <<= 1;
    }
    return crc & 0xF;
}

bool SerialMicronas::setBaseAddress(uint8_t address)
{
    // Convert address and data to hex format
    int parity = calculateParity("xxsb",QString::number(address,16));
    int finalToCRC = QString("xxsb").toInt() + static_cast<int>(address) + parity;
    qDebug()<< "finalToCRC: " <<finalToCRC ;

    // Concatenate for CRC calculation
    int crc = crcCalculate(finalToCRC, sizeof(finalToCRC)); // Address (2 digits) + Data (4 digits) = 20 bits

    // Construct final command
    QString command = "xxsb" + QString::number(address,16) +QString::number( parity,16) + QString::number( crc,16);
    QString response = sendCommand(command);

    // Validate response
    if (!response.isEmpty() && response.startsWith("0:")) {
        return true;
    } else {
        emit errorOccurred("Erro ao definir base address.");
        return false;
    }
}
