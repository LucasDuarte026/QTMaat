#include "logservowindow.h"

LogServoWindow::LogServoWindow(QTextEdit *logTextArea, QLineEdit *searchBar,
                               QPushButton *clearButton, QWidget *parent)
    : QWidget(parent), myLogTextArea(logTextArea), mySearchBar(searchBar),
    myClearButton(clearButton){

    setWindowTitle("Log do Sistema");

    // Configurar a área de log
    myLogTextArea->setReadOnly(true);
    // myLogTextArea->setStyleSheet("background-color: #1E1E1E; color: #D4D4D4; font: 12px Monospace;");

    mySearchBar->setPlaceholderText("Buscar no log...");

    // Conectar os botões aos slots
    connect(myClearButton, &QPushButton::clicked, this, &LogServoWindow::clearLog);
}

void LogServoWindow::appendLog(const QString &message) {
    QDateTime now = QDateTime::currentDateTime();
    QString timestamp = now.toString("yyyy-MM-dd hh:mm:ss.zzz"); // Formato: AAAA-MM-DD HH:MM:SS.MS

    // Concatenar o timestamp com a mensagem
    QString logMessage = " |" + timestamp + "| " + message;

    myLogTextArea->append(logMessage);

    // Rolar para baixo automaticamente
    QScrollBar *scrollBar = myLogTextArea->verticalScrollBar();
    scrollBar->setValue(scrollBar->maximum());
}

void LogServoWindow::clearLog() {
    myLogTextArea->clear();
}

void LogServoWindow::searchLog() {
    QString searchText = mySearchBar->text();
    if (searchText.isEmpty()) {
        return;
    }

    QString logContent = myLogTextArea->toPlainText();
    if (logContent.contains(searchText, Qt::CaseInsensitive)) {
        myLogTextArea->find(searchText);
    }
}
