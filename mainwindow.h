#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QDial>

#include "sensorselectionwindow.h"
#include "datatypes.h"



namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    QString getModelName();
    double getStartAngle();
    double getArriveAngle();
    QString getTurnDirection();


private slots:
    void updateSensorDependencies(SensorData &_sensorData);
    void on_sensorSelected_clicked();
    void on_actionAdicionar_triggered();
    void on_actionRemover_triggered();
    void on_actionVer_sensores_triggered();

    void setSensorData(SensorData _data);
    // void setModelName(QString _model_name);
    // void setStartAngle(double angle);
    // void setArriveAngle(double angle);
    // void setTurnDirection(QString _turnDirection);



private:
    Ui::MainWindow *ui;
    QDial *myDial;
    SensorSelectionWindow *sensorWindow;
    SensorData sensorData;
};

#endif // MAINWINDOW_H
