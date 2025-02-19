#include "viewsensorsdialog.h"
#include <QVBoxLayout>
#include <QFile>
#include <QTextStream>
#include <QHeaderView>
#include <QMessageBox>

ViewSensorsDialog::ViewSensorsDialog(const QString &filePath, QWidget *parent)
    : QDialog(parent)
    , tableWidget(new QTableWidget(this))
    , closeButton(new QPushButton("Fechar", this))
{
    setWindowTitle("Visualizar Sensores");
    resize(600, 400);

    // Configurar layout principal
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(tableWidget);
    mainLayout->addWidget(closeButton);

    // Conectar o botão Fechar
    connect(closeButton, &QPushButton::clicked, this, &QDialog::accept);

    // Carregar dados do CSV
    loadCsvData(filePath);
}

ViewSensorsDialog::~ViewSensorsDialog() {}

void ViewSensorsDialog::loadCsvData(const QString &filePath)
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::critical(this, "Erro", "Não foi possível abrir o arquivo: " + filePath);
        return;
    }

    QTextStream in(&file);
    QStringList headers;
    QVector<QStringList> rows;

    // Ler o arquivo CSV
    bool isFirstLine = true;
    while (!in.atEnd()) {
        QString line = in.readLine();
        QStringList parts = line.split(",");

        if (isFirstLine) {
            headers = parts;  // A primeira linha é o cabeçalho
            isFirstLine = false;
        } else {
            rows.append(parts);  // As demais são os dados
        }
    }

    file.close();

    // Configurar cabeçalhos na tabela
    tableWidget->setColumnCount(headers.size());
    tableWidget->setHorizontalHeaderLabels(headers);
    tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers); // Impede edição da tabela

    // Adicionar as linhas
    tableWidget->setRowCount(rows.size());
    for (int row = 0; row < rows.size(); ++row) {
        for (int col = 0; col < rows[row].size(); ++col) {
            tableWidget->setItem(row, col, new QTableWidgetItem(rows[row][col]));
        }
    }

    // Ajustar tamanhos das colunas
    tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}
