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
    bool sendCommand(const QByteArray &command);
    QByteArray readResponse();

    bool setBaseAddress(uint8_t baseAddress);
    QByteArray readRegister(uint8_t address);
    bool writeRegister(uint8_t address, uint16_t data);

private:
    QSerialPort *serial;
    uint8_t calculateParity(uint8_t command, uint8_t address);
    uint8_t calculateCRC(uint32_t data, uint8_t size);

    void processResponse(const QByteArray &response);
    QString interpretErrorCode(int errorCode);

signals:
    void responseReceived(const QString &response);
    void errorOccurred(const QString &errorMessage);

private slots:
    void handleReadyRead();
};

#endif // SERIALMICRONAS_H
