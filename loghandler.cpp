#include "loghandler.h"
#include <QDateTime>
#include <QTextCursor>
#include <iostream>

// Ponteiro para a área de texto que irá mostrar os logs
QTextEdit* LogHandler::generalLogTextEdit = nullptr;

// Mutex para proteger a área de texto de logs, pois pode ser acessada por várias threads
QMutex LogHandler::logMutex;

// Construtor da classe LogHandler
LogHandler::LogHandler(QTextEdit *generalLogWidget, QObject *parent)
    : QObject(parent)
{
    // Salva o ponteiro para a área de texto que irá mostrar os logs
    generalLogTextEdit = generalLogWidget;

    // Instala o handler de mensagens do Qt, que irá chamar a função messageHandler
    qInstallMessageHandler(messageHandler);

    // Redireciona stdout e stderr para serem capturados também
    // Isso é feito para que as mensagens de stdout e stderr sejam exibidas na área de texto de logs
    std::cout.rdbuf(std::cerr.rdbuf());
}

// Função que irá ser chamada pelo Qt para tratar as mensagens de log
void LogHandler::messageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    // Protege a área de texto de logs com um mutex, para que apenas uma thread possa acessá-la por vez
    QMutexLocker locker(&logMutex);

    // Formata a mensagem com a data e hora atuais
    QString formattedMessage = QDateTime::currentDateTime().toString("HH:mm:ss") + " - " + msg;

    // Define a cor conforme o tipo da mensagem
    QString color;
    switch (type) {
    case QtDebugMsg:
    case QtInfoMsg:
        // Mensagens de debug e info são mostradas em preto
        color = "black";
        break;
    case QtWarningMsg:
    case QtCriticalMsg:
    case QtFatalMsg:
        // Mensagens de warning, critical e fatal são mostradas em vermelho
        color = "red";
        break;
    }

    // Adiciona a mensagem formatada à área de texto de logs
    if (generalLogTextEdit) {
        generalLogTextEdit->append(QString("<span style=\"color:%1;\">%2</span>").arg(color, formattedMessage));
    }
}
