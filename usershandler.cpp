#include "usershandler.h"
#include <QTableWidget>
#include <QTableView>
#include <QHeaderView>
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


UsersHandler::UsersHandler(QObject *parent)
    : QObject(parent)
    , user_type("prod")
    , username("comum")
    , password("----")
{
}

UsersHandler::~UsersHandler()
{
}

//  Abrir a janela de adicionar um usuário
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
//  Criar o cabeçlho
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
//  Função acessora de inserir o usuário previamente filtrado no banco de dados
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

void UsersHandler::showViewUsersDialog() {
    // Criando a janela de diálogo
    QDialog *dialog = new QDialog();
    dialog->setWindowTitle("Lista de Usuários");
    dialog->resize(500, 400);

    // Criando a tabela
    QTableWidget *tableWidget = new QTableWidget(dialog);
    tableWidget->setColumnCount(3); // Exibir apenas tipo, nome de usuário e data de modificação
    tableWidget->setHorizontalHeaderLabels({"Tipo", "Nome de Usuário", "Data de Modificação"});
    tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers); // Impede edição da tabela

    // Criando botão de fechar
    QPushButton *closeButton = new QPushButton("Fechar", dialog);

    // Layout
    QVBoxLayout *layout = new QVBoxLayout(dialog);
    layout->addWidget(tableWidget);
    layout->addWidget(closeButton);
    dialog->setLayout(layout);
    try {

        // Carregar os dados do CSV
        QString filePath = QDir::currentPath() + "/user_credentials.csv";
        QFile file(filePath);

        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QMessageBox::critical(dialog, "Erro", "Erro ao abrir o arquivo de usuários.");
            return;
        }

        QTextStream stream(&file);
        QString header = stream.readLine(); // Ignorar o cabeçalho

        QList<QStringList> users;

        while (!stream.atEnd()) {
            QString line = stream.readLine();
            QStringList fields = line.split(',');

            if (fields.size() >= 4) {
                QStringList userData;
                userData << fields[0] << fields[1] << fields[3]; // Tipo, Nome, Data de Modificação
                users.append(userData);
            }
        }

        file.close();

        // Ordenar os usuários primeiro por tipo, depois por nome
        std::sort(users.begin(), users.end(), [](const QStringList &a, const QStringList &b) {
            return a[0] == b[0] ? a[1] < b[1] : a[0] < b[0];
        });

        // Preencher a tabela
        tableWidget->setRowCount(users.size());
        for (int i = 0; i < users.size(); ++i) {
            for (int j = 0; j < 3; ++j) {
                tableWidget->setItem(i, j, new QTableWidgetItem(users[i][j]));
            }
        }
    }
    catch (const std::runtime_error& error) {
        QMessageBox::critical(dialog, "Erro", error.what());
        return; // Importante: sair da função em caso de erro
    } catch (const std::exception& error) {
        QMessageBox::critical(dialog, "Erro", error.what());
        return; // Importante: sair da função em caso de erro
    } catch (...) {
        QMessageBox::critical(dialog, "Erro", "Ocorreu um erro desconhecido.");
        return; // Importante: sair da função em caso de erro
    }
    // Conectar botão de fechar
    connect(closeButton, &QPushButton::clicked, dialog, &QDialog::accept);

    // Exibir a janela
    dialog->exec();
}


UserType UsersHandler::loginAccess() {
    UserType _user;
    _user.username = "comum";
    _user.type = "prod";

    QDialog dialog;
    dialog.setWindowTitle("Login de Usuário");

    QComboBox *userTypeCombo = new QComboBox;
    userTypeCombo->addItem("Administrador");
    userTypeCombo->addItem("Engenharia");
    userTypeCombo->addItem("Produção");
    userTypeCombo->addItem("Qualidade");

    QLineEdit *usernameEdit = new QLineEdit;
    usernameEdit->setPlaceholderText("Nome de Usuário");

    QLineEdit *passwordEdit = new QLineEdit;
    passwordEdit->setPlaceholderText("Senha");
    passwordEdit->setEchoMode(QLineEdit::Password);

    QPushButton *okButton = new QPushButton("OK");
    QPushButton *cancelButton = new QPushButton("Cancelar");

    QVBoxLayout *mainLayout = new QVBoxLayout(&dialog);
    QFormLayout *formLayout = new QFormLayout();
    formLayout->addRow("Tipo:", userTypeCombo);
    formLayout->addRow("Usuário:", usernameEdit);
    formLayout->addRow("Senha:", passwordEdit);

    QHBoxLayout *buttonLayout = new QHBoxLayout;
    buttonLayout->addWidget(okButton);
    buttonLayout->addWidget(cancelButton);

    mainLayout->addLayout(formLayout);
    mainLayout->addLayout(buttonLayout);
    dialog.setLayout(mainLayout);

    connect(cancelButton, &QPushButton::clicked, &dialog, &QDialog::reject);

    connect(okButton, &QPushButton::clicked, [&]() {
        QString enteredType = userTypeCombo->currentText();
        QString enteredUsername = usernameEdit->text().trimmed();
        QString enteredPassword = passwordEdit->text();

        QString filePath = QDir::currentPath() + "/user_credentials.csv";
        QFile file(filePath);

        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QMessageBox::critical(&dialog, "Erro", "Erro ao abrir o arquivo de credenciais.");
            return;
        }

        QTextStream stream(&file);
        stream.readLine(); // Ignora o cabeçalho

        bool userFound = false;
        bool passwordCorrect = false;

        while (!stream.atEnd()) {
            QString line = stream.readLine();
            QStringList fields = line.split(',');

            if (fields.size() >= 4 && fields[1] == enteredUsername) {
                userFound = true;
                if (fields[0] == enteredType && fields[2] == enteredPassword) {
                    passwordCorrect = true;
                    _user.type = enteredType;
                    _user.username = enteredUsername;
                    break; // Sai do loop após encontrar o usuário e senha corretos
                }
            }
        }

        file.close();

        if (!userFound) {
            QMessageBox::warning(&dialog, "Erro", "Usuário não cadastrado.");
        } else if (!passwordCorrect) {
            QMessageBox::warning(&dialog, "Erro", "Credenciais incorretas. Tente novamente.");
        } else {
            QMessageBox::information(&dialog, "Sucesso", QString("Login realizado com sucesso!\nUsuário: %1\nTipo: %2").arg(_user.username, _user.type));
            dialog.accept(); // Aceita o diálogo apenas em caso de sucesso
        }
    });

    if (dialog.exec() == QDialog::Accepted) {
        return _user;
    } else {
        return {"comum", "prod"}; // Retorna um usuário padrão se o login for cancelado
    }
}

UserType UsersHandler::getFullUser() const
{
    return full_user;
}

QString UsersHandler::getUserType() const
{
    return user_type;
}

QString UsersHandler::getUsername() const
{
    return username;
}

QString UsersHandler::getPassword() const
{
    return password;
}
