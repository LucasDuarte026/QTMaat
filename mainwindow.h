#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QButtonGroup>
#include <QMessageBox>
#include <QFile>
#include <QString>
#include <QLabel>
#include "sensorselectionwindow.h"  // Inclui a a janela de seleção de sensores

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    QString model_name;
    double start_angle;
    double arrive_angle;
    QString turn_direction;

private slots:
    void on_sensorSelected_clicked();  // Slot para abrir a janela de seleção

    void on_actionAdicionar_triggered();

    void on_actionRemover_triggered();

    void on_actionVer_sensores_triggered();

    void updateSensorLabel(const QString &sensorName);  // Slot to update the label

    void on_actionUpdatePositions_triggered();



private:
    Ui::MainWindow *ui;
    SensorSelectionWindow *sensorWindow;  // Ponteiro para a a janela de seleção de sensores

};
#endif // MAINWINDOW_H
