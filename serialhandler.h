#ifndef SERIALHANDLER_H
#define SERIALHANDLER_H

#include <QObject>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QVector>

class SerialHandler : public QObject
{
    Q_OBJECT

public:
    explicit SerialHandler(QObject *parent = nullptr);
    ~SerialHandler();

    void openPort(const QString &portName, int baudRate = 9600);
    void closePort();
    void sendData(const QByteArray &data);
    QByteArray readData();

private:
    QSerialPort *serial;

    void decodeBiphase(const QByteArray &data);
    QVector<int> differentialDecode(const QVector<int> &bits);

signals:
    void dataReceived(const QByteArray &data);
    void decodedDataReady(const QVector<int> &decodedBits);

private slots:
    void handleReadyRead();
};

#endif // SERIALHANDLER_H
