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
    if (serial->isOpen()){
        emit errorOccurred("Porta já está aberta, reinicie a aplicação para reconectar");

        // serial->close();
        return true;
    }

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
    if (!serial->waitForReadyRead(1000)) {
        emit errorOccurred("Erro: Nenhuma resposta recebida.");
        return QString();
    }

    QByteArray responseData = serial->readAll().trimmed();
    qDebug() << "Dado enviado bruto :" << formattedCommand.toUtf8().toHex() << " | Como string:" << formattedCommand.toUtf8();
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


bool calculateParityBit(const QString& input) {
    QByteArray utf8Data = input.toUtf8();
    int ones = 0;

    for (char byte : utf8Data) {
        for (int i = 7; i >= 0; i--) {
            if ((byte >> i) & 1) {
                ones++;
            }
        }
    }
    return 1 - (ones % 2);
}

QString SerialMicronas::readAddress(uint8_t address){
    // transformar uint8_t (byte) em 2 dígitos hex em string
    QByteArray address2Byte= QString("%1").arg(address,2,16,QChar('0')).toUtf8();
    QString full_address = QString(address2Byte);
    return sendCommand("xxr" + full_address); // exemplos: xxr01 xxrFF xxr0E

}

/*
int calculateIntCrc(int data, int size) // função original exemplo da documentação
{
    const unsigned char CRC_POLY = 0x13; // x^4 + x + 1
    unsigned char crc = 0x0;
    for (int i=0; i<size; i++)
    {
        if ((crc<<3 & 0x1) != (data >> (size - 1 - i) & 1))
        {
            crc = crc<<1;
            crc ^= CRC_POLY;
            crc &= 0xF;
        }
        else
            crc <<= 1;
    }
    return crc&0xF;
}
*/

unsigned char calculateByteCrc(const QByteArray &data, int size) {
    const unsigned char CRC_POLY = 0x13; // polinomio X^4 + X + 1
    unsigned char crc = 0x0;

    // entendo pouco ainda de como isso aqui funciona, pode estar bem errado
    for (int i = 0; i < size; ++i) {
        unsigned char byte = data[i];
        for (int j = 7; j >= 0; --j) { // processar o bit mais significativo MSB
            unsigned char bit = (byte >> j) & 0x01;
            crc = (crc << 1) | bit;
            if (crc & 0x10) { // checar o 5º bit
                crc ^= CRC_POLY;
            }
            crc &= 0x0F; // exportar apenass os últimos 4 bits
        }
    }

    return crc;
}

bool SerialMicronas::setBaseAddress(uint8_t address, uint16_t data)
{
    // Construir o comando básico "xxsb" + address (2-digit hex) + data (4-digit hex)
    QString commandStr = "xxsb";
    QString addrStr = QString("%1").arg(address, 2, 16, QChar('0')); // 2 digitos hex
    QString dataStr = QString("%1").arg(data, 4, 16, QChar('0'));    // 4 dígitos hex
    QString baseCommand = commandStr + addrStr + dataStr;            // juntar tudo

    QByteArray full_command = baseCommand.toUtf8();                  // converter em binario

    // calcular o crc localmente pra melhor controle ( a saída para xxsb000001 é o char '3' = 0x33)
    // usando ferramentas online, o que saí para o polinomio 0x13 é crc = 0xD9 (sem sentido em UTF-8)
    // The polynomial is x^4 + x + 1 (0x13)
    const unsigned char CRC_POLY = 0x13;
    unsigned char crc = 0;
    int totalBits = full_command.size() * 8; // total number of bits

    // Process each bit (MSB first for each byte)
    for (int i = 0; i < totalBits; i++) {
        // Calculate the bit position: i/8 gives the byte index,
        // (7 - (i % 8)) gives the bit index within that byte (MSB first)
        bool bit = (full_command.at(i / 8) >> (7 - (i % 8))) & 0x1;
        // Compare the MSB (bit 3) of crc with the current data bit.
        if (((crc >> 3) & 0x1) != bit) {
            crc = (crc << 1) ^ CRC_POLY;
        } else {
            crc <<= 1;
        }
        crc &= 0xF; // keep crc to 4 bits
    }

    // colocar o CRC 1-digit hex no comando.
    full_command.append(QString::number(crc, 16).toUtf8());
    // full_command.append('d');


    qDebug() << "full_command:" << full_command << "CRC:" << QString::number(crc, 16);
    sendCommand(full_command);

    return true;
}

/*
bool SerialMicronas::setBaseAddress(uint8_t address, uint16_t data) // versão 2
{
    // Build the base command string: "xxsb" + address (2-digit hex) + data (4-digit hex)
    QString commandStr = "xxsb";
    QString addrStr = QString("%1").arg(address, 2, 16, QChar('0'));
    QString dataStr = QString("%1").arg(data, 4, 16, QChar('0'));
    QString baseCommand = commandStr + addrStr + dataStr;

    QByteArray full_command = baseCommand.toUtf8();

    // Calculate the 4-bit CRC over all protocol bits.
    // The polynomial is x^4 + x + 1 (0x13)
    const unsigned char CRC_POLY = 0x13;
    unsigned char crc = 0;
    int totalBits = full_command.size() * 8; // total number of bits

    // Process each bit (MSB first for each byte)
    for (int i = 0; i < totalBits; i++) {
        // Calculate the bit position: i/8 gives the byte index,
        // (7 - (i % 8)) gives the bit index within that byte (MSB first)
        bool bit = (full_command.at(i / 8) >> (7 - (i % 8))) & 0x1;
        // Compare the MSB (bit 3) of crc with the current data bit.
        if (((crc >> 3) & 0x1) != bit) {
            crc = (crc << 1) ^ CRC_POLY;
        } else {
            crc <<= 1;
        }
        crc &= 0xF; // keep crc to 4 bits
    }

    // Append the CRC as a 1-digit hex (lowercase) to the command.
    full_command.append(QString::number(crc, 16).toUtf8());

    qDebug() << "full_command:" << full_command << "CRC:" << QString::number(crc, 16);
    sendCommand(full_command);

    return true;
}
*/


/*
// versão 1
 bool SerialMicronas::setBaseAddress(uint8_t address, uint16_t data)
{
    QString commandStr = "xxsb";
    QByteArray full_command = commandStr.toUtf8();
    QByteArray address2Byte = QString("%1").arg(address, 2, 16, QChar('0')).toUtf8();
    QByteArray baseData4Byte = QString("%1").arg(data, 4, 16, QChar('0')).toUtf8();

    full_command.append(address2Byte);
    full_command.append(baseData4Byte);

    // Calculate CRC over the command, address, and data
    unsigned char crc = calculateByteCrc(full_command, full_command.size());
    QByteArray crcHex = QByteArray::number(crc, 16).right(1).toLower();
    full_command.append(crcHex);

    qDebug() << "Full command with CRC: " << full_command;
    sendCommand(full_command);

    return true;
}
*/

/*
unsigned char computeCRC(const QString &input) {
    QByteArray data = input.toUtf8();
    unsigned char crc = 0; // Inicializa o CRC com 0

    for (char byte : data) {
        for (int i = 7; i >= 0; --i) { // Processa cada bit do byte, do MSB para o LSB
            unsigned char bit = (byte >> i) & 1; // Extrai o bit atual
            crc = (crc << 1) | bit; // Desloca o CRC para a esquerda e adiciona o novo bit

            // Verifica se o 5º bit (bit 4) está definido após o deslocamento
            if (crc & 0x10) { // 0x10 é 16 em decimal (10000 em binário)
                crc ^= 0x13; // Aplica XOR com o polinômio 0x13 (10011)
                crc &= 0x0F; // Mantém apenas os 4 bits inferiores
            }
        }
    }

    return crc; // Retorna o CRC de 4 bits
}



unsigned char calculateByteCrc(const QByteArray& data, unsigned char crc_width = 8) {
    const unsigned char CRC_POLY = 0x13; // x^4 + x + 1
    unsigned char crc = 0x0;
    unsigned char mask = (1 << crc_width) - 1;

    for (char byte : data) {
        for (int i = 0; i < 8; ++i) {
            if ((crc & mask) >> (crc_width - 1) != (byte >> (7 - i) & 1)) {
                crc = (crc << 1) ^ CRC_POLY;
            } else {
                crc <<= 1;
            }
            crc &= mask;
        }
    }

    return crc;
}

bool SerialMicronas::setBaseAddress(uint8_t address,uint16_t data)
{

    QString commandStr = "xxsb";
    QByteArray full_command = commandStr.toUtf8();
    QByteArray address2Byte= QString("%1").arg(address,2,16,QChar('0')).toUtf8();
    QByteArray baseData4Byte= QString("%1").arg(data,4,16,QChar('0')).toUtf8();

    full_command.append(address2Byte);
    full_command.append(baseData4Byte);
    // full_command.append(0x64); // supostamente o 'd' que a documentação coloca como o CRC de xxsb000001

    unsigned char crc = calculateByteCrc(full_command,full_command.size());

    qDebug() <<"full_command: "<< full_command <<"crc " << crc <<" crc: " << QString::number(crc,16) << "|" << QChar(crc)<< "|";
    qDebug() <<"size: "<< full_command.size();
    sendCommand(full_command);

    uint32_t value = 0;
    int command_size = full_command.size();
    for (int i = 0; i < command_size; ++i) {
        value |= full_command.at(i) << (i * 8);
    }
    int crc2 = calculateIntCrc(value,command_size);
    qDebug() <<"int value : "<< value << QString::number(value,16).toUpper();
    qDebug() <<"(CRC2):     "<< crc2  << QString::number(crc2 ,16).toUpper();

    return true;
}
*/
