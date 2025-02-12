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
    bool sendCommand(const QString &command);
    QString getLastResponse() const;

private:
    QSerialPort *serial;
    QString lastResponse;

    void processResponse(const QByteArray &response);
    QString interpretErrorCode(int errorCode);

signals:
    void responseReceived(const QString &response);
    void errorOccurred(const QString &errorMessage);

private slots:
    void handleReadyRead();
};

#endif // SERIALMICRONAS_H
