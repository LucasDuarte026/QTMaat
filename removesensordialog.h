#ifndef REMOVESENSORDIALOG_H
#define REMOVESENSORDIALOG_H

#include <QDialog>
#include <QComboBox>
#include <QPushButton>
#include <QString>

class RemoveSensorDialog : public QDialog
{
    Q_OBJECT

public:
    explicit RemoveSensorDialog(QWidget *parent = nullptr);
    ~RemoveSensorDialog();

    QString getSelectedSensor() const;  // Retorna o nome do sensor selecionado

    void setSensorList(const QStringList &sensors);  // Define a lista de sensores

private:
    QComboBox *sensorComboBox;  // Menu suspenso para seleção de sensores
    QPushButton *removeButton;  // Botão "Remover"
    QPushButton *cancelButton;  // Botão "Cancelar"

private slots:
    void handleRemoveButton();  // Lida com a ação de remoção
};

#endif // REMOVESENSORDIALOG_H
