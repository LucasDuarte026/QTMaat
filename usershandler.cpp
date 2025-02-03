#include "usershandler.h"
#include <QDialog>
#include <QComboBox>
#include <QLineEdit>
#include <QPushButton>
#include <QMessageBox>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QFile>
#include <QTextStream>
#include <QDir>
#include <QDateTime>
#include <QList>
#include <QStringList>
#include <algorithm>

UsersHandler::UsersHandler(QObject *parent) : QObject(parent)
{
}

UsersHandler::~UsersHandler()
{
}

void UsersHandler::showAddUserDialog()
{
    QDialog *dialog = new QDialog();
    dialog->setWindowTitle("Adicionar Usuário");
    dialog->resize(400, 200);

    QComboBox *typeComboBox = new QComboBox;
    typeComboBox->addItem("Administrador");
    typeComboBox->addItem("Engenharia");
    typeComboBox->addItem("Produção");
    typeComboBox->addItem("Qualidade");

    QLineEdit *userNameEdit = new QLineEdit;
    userNameEdit->setPlaceholderText("Nome de Usuário");

    QLineEdit *passwordEdit = new QLineEdit;
    passwordEdit->setPlaceholderText("Senha");
    passwordEdit->setEchoMode(QLineEdit::Password);

    QPushButton *okButton = new QPushButton("OK");
    QPushButton *cancelButton = new QPushButton("Cancel");

    QVBoxLayout *mainLayout = new QVBoxLayout(dialog);
    mainLayout->setSpacing(15);
    QFormLayout *formLayout = new QFormLayout();
    mainLayout->setSpacing(15);
    formLayout->addRow("Tipo:", typeComboBox);
    formLayout->addRow(userNameEdit);
    formLayout->addRow(passwordEdit);

    QHBoxLayout *buttonLayout = new QHBoxLayout;
    mainLayout->setSpacing(15);

    buttonLayout->addWidget(okButton);
    buttonLayout->addWidget(cancelButton);

    mainLayout->addLayout(formLayout);
    mainLayout->addLayout(buttonLayout);

    dialog->setLayout(mainLayout);

    connect(okButton, &QPushButton::clicked, dialog, &QDialog::accept);
    connect(cancelButton, &QPushButton::clicked, dialog, &QDialog::reject);

    while (true) {
        if (dialog->exec() == QDialog::Accepted) {
            QString type = typeComboBox->currentText();
            QString userName = userNameEdit->text().trimmed();
            QString password = passwordEdit->text();

            // Validações
            if (password.length() < 4) {
                QMessageBox::critical(dialog, "Erro", "A senha deve ter no mínimo 4 dígitos.");
                continue;
            }

            if (userName.contains(" ")) {
                QMessageBox::critical(dialog, "Erro", "O nome de usuário não pode ter espaços internos.");
                continue;
            }

            if (userName.isEmpty()) {
                QMessageBox::critical(dialog, "Erro", "O nome de usuário não pode estar vazio.");
                continue;
            }

            if (addUser(type, userName, password)) {
                QMessageBox::information(dialog, "Sucesso", "Usuário adicionado com sucesso.");
                break;
            }
        } else {
            break;
        }
    }

    dialog->close();
}

void UsersHandler::createFileWithHeader(const QString& filePath) {
    QFile file(filePath);
    if (!file.exists()) {
        if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QTextStream stream(&file);
            stream << "user_type,username,password,changed_date" << Qt::endl;
            file.close();
        } else {
            QMessageBox::critical(nullptr, "Erro", "Erro ao criar o arquivo.");
        }
    }
}


bool UsersHandler::addUser(const QString &type, const QString &userName, const QString &password)
{
    QString filePath = QDir::currentPath() + "/user_credentials.csv";
    QString tempFilePath = filePath + ".temp";

    createFileWithHeader(filePath);

    QFile file(filePath);
    QFile tempFile(tempFilePath);
    QList<QStringList> users;
    bool userExists = false;

    try {
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            throw std::runtime_error("Erro ao abrir o arquivo para leitura.");
        }

        QTextStream inStream(&file);
        QString header = inStream.readLine();

        while (!inStream.atEnd()) {
            QString line = inStream.readLine();
            QStringList fields = line.split(',');
            users.append(fields);
            if (fields.size() >= 2 && fields[1] == userName) {
                userExists = true;
                break;
            }
        }
        file.close();

        if (userExists) {
            QMessageBox::critical(nullptr, "Erro", "Este nome de usuário já existe.");
            return false;
        }

        QStringList newUser;
        newUser << type << userName << password << QDateTime::currentDateTime().toString("dd-MM-yyyy-hh:mm:ss");
        users.append(newUser);

        std::sort(users.begin(), users.end(), [](const QStringList &a, const QStringList &b) {
            if (a[0] == b[0]) {
                return a[1] < b[1];
            }
            return a[0] < b[0];
        });

        if (!tempFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
            throw std::runtime_error("Erro ao abrir o arquivo temporário para escrita.");
        }

        QTextStream outStream(&tempFile);
        outStream << header << Qt::endl;

        for (const auto &user : users) {
            outStream << user.join(',') << Qt::endl;
        }

        tempFile.close();

        if (!QFile::remove(filePath)) {
            throw std::runtime_error("Erro ao remover o arquivo original.");
        }

        if (!QFile::rename(tempFilePath, filePath)) {
            throw std::runtime_error("Erro ao renomear o arquivo temporário.");
        }

        return true;

    } catch (const std::runtime_error& error) {
        QMessageBox::critical(nullptr, "Erro", error.what());
        if (file.isOpen()) file.close();
        if (tempFile.isOpen()) tempFile.close();
        QFile::remove(tempFilePath);
        return false;
    } catch (const std::exception& error) {
        QMessageBox::critical(nullptr, "Erro", error.what());
        if (file.isOpen()) file.close();
        if (tempFile.isOpen()) tempFile.close();
        QFile::remove(tempFilePath);
        return false;
    } catch (...) {
        QMessageBox::critical(nullptr, "Erro", "Ocorreu um erro desconhecido.");
        if (file.isOpen()) file.close();
        if (tempFile.isOpen()) tempFile.close();
        QFile::remove(tempFilePath);
        return false;
    }
}


