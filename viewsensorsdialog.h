#ifndef VIEWSENSORSDIALOG_H
#define VIEWSENSORSDIALOG_H

#include <QDialog>
#include <QTableWidget>
#include <QPushButton>
#include <QString>

class ViewSensorsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ViewSensorsDialog(const QString &filePath, QWidget *parent = nullptr);
    ~ViewSensorsDialog();

private:
    QTableWidget *tableWidget;  // Tabela para exibir os dados
    QPushButton *closeButton;  // Botão para fechar a janela

    void loadCsvData(const QString &filePath);  // Carrega os dados do CSV
};

#endif // VIEWSENSORSDIALOG_H
