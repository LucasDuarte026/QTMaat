#include "logservowindow.h"

LogServoWindow::LogServoWindow(QTextEdit *logTextArea, QLineEdit *searchBar,
                               QPushButton *clearButton, QWidget *parent)
    : QWidget(parent), myLogTextArea(logTextArea), mySearchBar(searchBar),
    myClearButton(clearButton){

    // Configura o título da janela
    setWindowTitle("Log do Servo");

    // Configura a área de log
    myLogTextArea->setReadOnly(true);

    // Configura a barra de pesquisa
    mySearchBar->setPlaceholderText("Buscar no log...");

    // Conecta os botões aos slots
    connect(myClearButton, &QPushButton::clicked, this, &LogServoWindow::clearLog);
    connect(mySearchBar, &QLineEdit::textChanged, this, &LogServoWindow::searchLog);
}

// Adiciona uma mensagem ao log
void LogServoWindow::appendLog(const QString &message) {
    // Pega a data e hora atuais
    QDateTime now = QDateTime::currentDateTime();
    // Formata a data e hora em uma string com o formato AAAA-MM-DD HH:MM:SS.MS
    QString timestamp = now.toString("yyyy-MM-dd hh:mm:ss.zzz");

    // Concatena o timestamp com a mensagem
    QString logMessage = " |" + timestamp + "| " + message;

    // Adiciona a mensagem formatada à área de texto de log
    myLogTextArea->append(logMessage);

    // Rola para baixo automaticamente
    QScrollBar *scrollBar = myLogTextArea->verticalScrollBar();
    scrollBar->setValue(scrollBar->maximum());
}

// Limpa o log
void LogServoWindow::clearLog() {
    // Limpa a área de texto de log
    myLogTextArea->clear();
}

// Procura por texto no log
void LogServoWindow::searchLog() {
    // Pega o texto da barra de pesquisa
    QString searchText = mySearchBar->text();
    // Verifica se o texto é vazio
    if (searchText.isEmpty()) {
        return;
    }

    // Pega o conteúdo do log
    QString logContent = myLogTextArea->toPlainText();
    // Verifica se o texto da barra de pesquisa está no log
    if (logContent.contains(searchText, Qt::CaseInsensitive)) {
        // Se sim, busca o texto no log
        myLogTextArea->find(searchText);
    }
}
