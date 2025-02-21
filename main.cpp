#include "mainwindow.h"

#include <QApplication>
#include <QWidget>
#include <QPushButton>
#include <QHBoxLayout>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // Defina o caminho para o ícone
    const QIcon icon("./maat_logo256.png");  // Usando o caminho relativo correto

    // Defina o ícone para a aplicação, o que afeta o ícone na taskbar
    a.setWindowIcon(icon);

    // Atribua o nome à aplicação
    a.setApplicationName("Ma'at");

    MainWindow w;

    // Defina o ícone também para a janela principal
    w.setWindowIcon(icon);

    w.show();

    return a.exec();
}
