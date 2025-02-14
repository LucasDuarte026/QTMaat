#ifndef SERIALMICRONAS_H
#define SERIALMICRONAS_H

#include <QObject>
#include <QSerialPort>
#include <QSerialPortInfo>

class SerialMicronas : public QObject
{
    Q_OBJECT

public:
    explicit SerialMicronas(QObject *parent = nullptr);
    ~SerialMicronas();

    bool openPort(const QString &portName);
    void closePort();

    uint16_t readAddress(uint8_t address);
    bool writeAddress(uint8_t address, uint16_t data);
    bool setBaseAddress(uint16_t base);
    uint8_t getError();

private:
    QSerialPort *serial;
    bool calculateParityBit();
    void sendCommand(int length);
    void sendSync();
    void writeLogicalZero();
    void writeLogicalOne();

    uint8_t readResponse();
    uint8_t calculateCRC(uint32_t data, uint8_t length);
    uint8_t readBit();
    bool waitForACK();

    uint8_t buffer[30];
    bool outputState;
    uint8_t errorCode;
    uint16_t response;

signals:
    void responseReceived(const QString &response);
    void errorOccurred(const QString &errorMessage);
};

#endif // SERIALMICRONAS_H
