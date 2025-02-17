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
    QString sendCommand(const QString &command);
    bool setBoardMode(char mode);

    uint8_t getError();
    QString getFirmwareVersion();
    bool setBaseAddress(uint8_t address);

private:
    QString processResponse(const QByteArray &response);

    QSerialPort *serial;

    uint8_t buffer[30];
    bool outputState;
    uint8_t errorCode;
    uint16_t response;

signals:
    void errorOccurred(const QString &errorMessage);
    void messageMicronas_signal(const QString &errorMessage);

};

#endif // SERIALMICRONAS_H
