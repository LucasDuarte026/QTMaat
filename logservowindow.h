#ifndef LOGSERVOWINDOW_H
#define LOGSERVOWINDOW_H

#include <QWidget>
#include <QTextEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QScrollBar>
#include <QDateTime>
class LogServoWindow : public QWidget {
    Q_OBJECT

public:
    explicit LogServoWindow(QTextEdit *logTextArea, QLineEdit *searchBar,
                            QPushButton *clearButton, QWidget *parent = nullptr);

    void appendLog(const QString &message);  // Adiciona mensagem ao log

private slots:
    void clearLog();      // Limpa o log
    void searchLog();     // Procura por texto no log

private:
    QTextEdit *myLogTextArea;  // Área de exibição do log
    QLineEdit *mySearchBar;    // Barra de pesquisa
    QPushButton *myClearButton; // Botão para limpar logs
};

#endif // LOGSERVOWINDOW_H
