#ifndef LOGHANDLER_H
#define LOGHANDLER_H

#include <QObject>
#include <QTextEdit>
#include <QMutex>

class LogHandler : public QObject
{
    Q_OBJECT

public:
    explicit LogHandler(QTextEdit *generalLogWidget, QObject *parent = nullptr);
    static void messageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg);

private:
    static QTextEdit *generalLogTextEdit;
    static QMutex logMutex;
};

#endif // LOGHANDLER_H
