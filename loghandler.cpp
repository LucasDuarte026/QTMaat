#include "loghandler.h"
#include <QDateTime>
#include <QTextCursor>
#include <iostream>

QTextEdit* LogHandler::generalLogTextEdit = nullptr;
QMutex LogHandler::logMutex;

LogHandler::LogHandler(QTextEdit *generalLogWidget, QObject *parent)
    : QObject(parent)
{
    generalLogTextEdit = generalLogWidget;

    // Instala o handler de mensagens do Qt
    qInstallMessageHandler(messageHandler);

    // Redireciona stdout e stderr para serem capturados também
    std::cout.rdbuf(std::cerr.rdbuf());
}

void LogHandler::messageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    QMutexLocker locker(&logMutex);

    QString formattedMessage = QDateTime::currentDateTime().toString("HH:mm:ss") + " - " + msg;

    // Define a cor conforme o tipo da mensagem
    QString color;
    switch (type) {
    case QtDebugMsg:
    case QtInfoMsg:
        color = "black";
        break;
    case QtWarningMsg:
    case QtCriticalMsg:
    case QtFatalMsg:
        color = "red";
        break;
    }

    // Adiciona a mensagem ao `general_log_screen`
    if (generalLogTextEdit) {
        generalLogTextEdit->append(QString("<span style=\"color:%1;\">%2</span>").arg(color, formattedMessage));
    }
}
