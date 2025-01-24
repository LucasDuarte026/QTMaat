#ifndef SENSORSELECTIONWINDOW_H
#define SENSORSELECTIONWINDOW_H

#include <QWidget>
#include <QListWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QFile>
#include <QDebug>
#include "datatypes.h"



class SensorSelectionWindow : public QWidget
{
    Q_OBJECT

public:
    explicit SensorSelectionWindow(QWidget *parent = nullptr);
    ~SensorSelectionWindow();

signals:
    void sensorSelected(SensorData *sensor_selected);  // Sinal emitido ao selecionar um sensor

private slots:
    void filterSensors(const QString &text);         // Filtro de busca
    void confirmSelection();                         // Confirmação da seleção
    void cancelSelection();                          // Cancelar operação

private:
    QLineEdit *filterEdit;       // Barra de filtro
    QListWidget *sensorList;     // Lista de sensores
    QPushButton *okButton;       // Botão "OK"
    QPushButton *cancelButton;   // Botão "Cancel"
    void loadSensors();          // Carrega os sensores do arquivo

};

#endif // SENSORSELECTIONWINDOW_H
