#ifndef LOGSERVOWINDOW_H
#define LOGSERVOWINDOW_H

#include <QWidget>
#include <QTextEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QScrollBar>

class LogServoWindow : public QWidget {
    Q_OBJECT

public:
    explicit LogServoWindow(QTextEdit *logTextArea, QLineEdit *searchBar,
                            QPushButton *clearButton, QPushButton *searchButton, QWidget *parent = nullptr);

    void appendLog(const QString &message);  // Adiciona mensagem ao log

private slots:
    void clearLog();      // Limpa o log
    void searchLog();     // Procura por texto no log

private:
    QTextEdit *myLogTextArea;  // Área de exibição do log
    QLineEdit *mySearchBar;    // Barra de pesquisa
    QPushButton *myClearButton; // Botão para limpar logs
    QPushButton *mySearchButton; // Botão para pesquisar logs
};

#endif // LOGSERVOWINDOW_H
