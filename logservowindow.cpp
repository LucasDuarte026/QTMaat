#include "logservowindow.h"

LogServoWindow::LogServoWindow(QTextEdit *logTextArea, QLineEdit *searchBar,
                               QPushButton *clearButton, QPushButton *searchButton, QWidget *parent)
    : QWidget(parent), myLogTextArea(logTextArea), mySearchBar(searchBar),
    myClearButton(clearButton), mySearchButton(searchButton) {

    setWindowTitle("Log do Sistema");

    // Configurar a área de log
    myLogTextArea->setReadOnly(true);
    // myLogTextArea->setStyleSheet("background-color: #1E1E1E; color: #D4D4D4; font: 12px Monospace;");

    mySearchBar->setPlaceholderText("Buscar no log...");

    // Conectar os botões aos slots
    connect(myClearButton, &QPushButton::clicked, this, &LogServoWindow::clearLog);
    connect(mySearchButton, &QPushButton::clicked, this, &LogServoWindow::searchLog);
}

void LogServoWindow::appendLog(const QString &message) {
    myLogTextArea->append(message);

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
