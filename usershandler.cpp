#include "usershandler.h"
#include <QTableWidget>
#include <QObject>
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
#include <QLabel>


UsersHandler::UsersHandler(QObject *parent)
    : QObject(parent)
    ,full_user{"Comum","Produção"}
    , password("----")
{
    QString filePath = QDir::currentPath() + "/user_credentials.csv";
    QFile file(filePath);

    if (!file.exists()) {
        // Arquivo não existe, cria com cabeçalho e insere o admin
        if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QTextStream stream(&file);
            stream << "user_type,username,password,changed_date" << Qt::endl;
            stream << "Administrador,admin,admin_password,------" << Qt::endl;
            stream << "Administrador,lucas_luchiari,lucas_luchiari_senha,-----" << Qt::endl;
            file.close();
        } else {
            QMessageBox::critical(nullptr, "Erro", "Erro ao criar o arquivo de credenciais.");
            // Lidar com o erro, talvez sair do programa ou usar valores padrão.
            full_user.type = "Produção";
            full_user.username = "Comum";
            password = "----";
            return; // Importante: sair do construtor em caso de erro na criação do arquivo.
        }
    } else {
        // Arquivo existe, tenta ler o primeiro usuário (ou valores padrão)
        if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QTextStream stream(&file);
            stream.readLine(); // Ignora o cabeçalho

            if (!stream.atEnd()) { // Verifica se há pelo menos uma linha de dados
                QString line = stream.readLine();
                QStringList fields = line.split(',');
                if (fields.size() >= 3) { // Verifica se há pelo menos tipo, nome e senha
                    full_user.type = fields[0];
                    full_user.username = fields[1];
                    password = fields[2];
                } else {
                    // Arquivo corrompido ou com formato incorreto, usa valores padrão
                    QMessageBox::warning(nullptr, "Aviso", "Arquivo de credenciais corrompido. Usando valores padrão.");
                    full_user.type = "Produção";
                    full_user.username = "Comum";
                    password = "----";
                }
            } else {
                // Arquivo vazio (após o cabeçalho), usa valores padrão
                full_user.type = "Produção";
                full_user.username = "Comum";
                password = "----";
            }
            file.close();
        } else {
            QMessageBox::critical(nullptr, "Erro", "Erro ao abrir o arquivo de credenciais.");
            full_user.type = "Produção";
            full_user.username = "Comum";
            password = "----";
            return; // Importante: sair do construtor em caso de erro na abertura do arquivo.
        }
    }
}

UsersHandler::~UsersHandler()
{
}

//  Abrir a janela de adicionar um usuário
void UsersHandler::showAddUserDialog(UserType myUser)
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
            stream << "type,username,password,changed_date" << Qt::endl;
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

// Função para visualização dos usuários cadastrados
void UsersHandler::showViewUsersDialog(UserType myUser) {
    // Criando a janela de diálogo

    QDialog *dialog = new QDialog();
    dialog->setWindowTitle("Lista de Usuários");
    dialog->resize(600, 400);

    // Criando a tabela
    QTableWidget *tableWidget = new QTableWidget(dialog);
    if(myUser.type== "Administrador"){
        tableWidget->setColumnCount(4);// Exibir apenas tipo, nome de usuário, senha e data de modificação
        tableWidget->setHorizontalHeaderLabels({"Tipo", "Nome de Usuário", "Senha","Data de Modificação"});
    }
    else{
        tableWidget->setColumnCount(3); // Exibir apenas tipo, nome de usuário e data de modificação
        tableWidget->setHorizontalHeaderLabels({"Tipo", "Nome de Usuário", "Data de Modificação"});
    }
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
                if(myUser.type == "Administrador")
                    userData << fields[0] << fields[1] << fields[2] << fields[3]; // Tipo, Nome, senha, Data de Modificação
                else
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
            if (myUser.type == "Administrador"){
                for (int j = 0; j < 4; ++j) {
                    tableWidget->setItem(i, j, new QTableWidgetItem(users[i][j]));
                }
            }else {
                for (int j = 0; j < 3; ++j) {
                    tableWidget->setItem(i, j, new QTableWidgetItem(users[i][j]));
                }
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

//  Remove usuário
void UsersHandler::showRemoveUserDialog() {
    QString filePath = "user_credentials.csv"; // Caminho do arquivo CSV

    // Tenta abrir o arquivo CSV
    QVector<QStringList> userData;
    try {
        QFile file(filePath);
        if (!file.open(QIODevice::ReadOnly)) {
            QMessageBox::critical(nullptr, "Erro", "Não foi possível abrir o arquivo CSV.");
            return; // Sai da função se não conseguir abrir o arquivo
        }

        QTextStream in(&file);
        in.readLine(); // Pula o cabeçalho
        while (!in.atEnd()) {
            userData.append(in.readLine().split(",")); // Lê os dados do CSV
        }
        file.close();
    } catch (const std::exception& error) {
        QMessageBox::critical(nullptr, "Erro", QString("Erro ao ler o arquivo: %1").arg(error.what()));
        return; // Sai da função em caso de erro
    } catch (...) {
        QMessageBox::critical(nullptr, "Erro", "Ocorreu um erro desconhecido ao tentar abrir o arquivo.");
        return; // Sai da função em caso de erro desconhecido
    }

    // Cria a interface do diálogo
    QDialog *dialog = new QDialog();
    dialog->setWindowTitle("Remover Usuário");
    dialog->resize(300,150);

    QComboBox *userComboBox = new QComboBox(dialog);
    QPushButton *removeButton = new QPushButton("Remover", dialog);
    QPushButton *cancelButton = new QPushButton("Cancelar", dialog);

    // layout principal
    QVBoxLayout *mainLayout = new QVBoxLayout(dialog);

    // Label e combo box
    QHBoxLayout *selectionLayout = new QHBoxLayout();
    selectionLayout->addWidget(new QLabel("Selecione o usuário:"));
    selectionLayout->addWidget(userComboBox);
    mainLayout->addLayout(selectionLayout);

    // bottons abaixo
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget(removeButton);
    buttonLayout->addWidget(cancelButton);
    mainLayout->addLayout(buttonLayout);

    // Preenche o ComboBox com os usuários
    for (const QStringList& user : userData) {
        if (user.size() > 1) { // Verifica se há pelo menos um nome de usuário
            userComboBox->addItem(user[1]); // Adiciona o nome do usuário ao ComboBox
        }
    }



    // Conectar os sinais dos botões
    connect(cancelButton, &QPushButton::clicked, dialog, &QDialog::reject);
    connect(removeButton, &QPushButton::clicked, dialog, &QDialog::accept);

    while (true){
        if(dialog->exec() == QDialog::Accepted){
            QString user_to_remove = userComboBox->currentText();

            // Verifica se um usuário foi selecionado
            if (user_to_remove.isEmpty()) {
                QMessageBox::warning(dialog, "Erro", "Selecione um usuário para remover.");
                continue;
            }
            // Confirmação de remoção
            if (QMessageBox::question(dialog, "Confirmação"
                                      , QString("Deseja realmente remover o usuário %1?").arg(user_to_remove)
                                      , QMessageBox::Yes | QMessageBox::No
                                      , QMessageBox::No)
                != QMessageBox::Yes)
            {
                return; // Aborta a operação se o usuário não confirmar
            }
            if(removeUser(user_to_remove,userData)){
                QMessageBox::information(dialog, "Sucesso", "Usuário removido com sucesso.");
                break;
            }
        } else
            break;
    }
    dialog->accept(); // Fecha o diálogo após a remoção bem-sucedida
}

// Função acessora para remoção de um usuário
bool UsersHandler::removeUser(const QString &userName,QVector<QStringList> userData){
    QString filePath = "user_credentials.csv"; // Caminho do arquivo CSV

    // Remove o usuário selecionado do QVector
    userData.erase(std::remove_if(userData.begin(), userData.end(),
                                  [userName](const QStringList& row) {
                                      return row.size() > 1 && row[1] == userName;
                                  }), userData.end());

    // Tenta reescrever o arquivo CSV
    try {
        QFile file(filePath);
        if (!file.open(QIODevice::WriteOnly)) {
            throw std::runtime_error("Erro ao abrir o arquivo para leitura.");
            return false;
        }

        QTextStream out(&file);
        out << "type,username,password,changed_date\n"; // Escreve o cabeçalho
        for (const QStringList& row : userData) {
            out << row.join(",") << "\n"; // Escreve os dados atualizados
        }
        file.close();
        return true;

    } catch (const std::exception& error) {
        QMessageBox::critical(nullptr, "Erro", QString("Erro ao salvar o arquivo: %1").arg(error.what()));
    } catch (...) {
        QMessageBox::critical(nullptr, "Erro", "Ocorreu um erro desconhecido ao tentar salvar o arquivo.");
    }
    return true;
}

//  Atualizar usuário
void UsersHandler::showUpdateUserDialog(UserType myUser) {
    if(full_user.username =="Comum"){
        QMessageBox::information(nullptr, "Erro", "Login não foi feito para atualizar perfil.");

        return;
    }

    QString filePath = "user_credentials.csv";
    QVector<QStringList> userData;

    try {
        QFile file(filePath);
        if (!file.open(QIODevice::ReadOnly)) {
            QMessageBox::critical(nullptr, "Erro", "Não foi possível abrir o arquivo CSV.");
            return;
        }

        QTextStream in(&file);
        in.readLine(); // Pular o cabeçalho
        while (!in.atEnd()) {
            userData.append(in.readLine().split(","));
        }
        file.close();
    } catch (const std::exception& error) {
        QMessageBox::critical(nullptr, "Erro", QString("Erro ao ler o arquivo: %1").arg(error.what()));
        return;
    } catch (...) {
        QMessageBox::critical(nullptr, "Erro", "Ocorreu um erro desconhecido ao tentar abrir o arquivo.");
        return;
    }

    if (myUser.type != "Administrador") {
        QString selectedUser = myUser.username;
        QString _userType = myUser.type;

        QDialog *updateDialog = new QDialog();
        updateDialog->setWindowTitle("Atualizar Usuário");

        QLineEdit *userNameEdit = new QLineEdit(selectedUser);
        QLineEdit *passwordEdit = new QLineEdit();

        QPushButton *updateButton = new QPushButton("Atualizar", updateDialog);
        QPushButton *cancelUpdateButton = new QPushButton("Cancelar", updateDialog);

        QVBoxLayout *mainLayout = new QVBoxLayout(updateDialog);

        QFormLayout *formLayout = new QFormLayout();
        formLayout->addRow("Nome de Usuário:", userNameEdit);
        formLayout->addRow("Senha:", passwordEdit);
        mainLayout->addLayout(formLayout);

        QHBoxLayout *buttonLayout = new QHBoxLayout();
        buttonLayout->addWidget(updateButton);
        buttonLayout->addWidget(cancelUpdateButton);
        mainLayout->addLayout(buttonLayout);

        int selectedIndex = -1;
        for (int i = 0; i < userData.size(); ++i) {
            if (userData[i][1] == selectedUser) {
                selectedIndex = i;
                break;
            }
        }

        connect(cancelUpdateButton, &QPushButton::clicked, updateDialog, &QDialog::reject);
        connect(updateButton, &QPushButton::clicked, [&, this, updateDialog, userData, selectedIndex,_userType]() mutable {
            QString newUsername = userNameEdit->text().trimmed();
            QString newPassword = passwordEdit->text();

            if (newUsername.isEmpty()) {
                QMessageBox::critical(updateDialog, "Erro", "O nome de usuário não pode estar vazio.");
                return;
            }

            if (newUsername.contains(" ")) {
                QMessageBox::critical(updateDialog, "Erro", "O nome de usuário não pode ter espaços internos.");
                return;
            }

            userData[selectedIndex][0] = _userType;
            userData[selectedIndex][1] = newUsername;
            userData[selectedIndex][2] = newPassword;
            userData[selectedIndex][3] = QDateTime::currentDateTime().toString("dd-MM-yyyy-hh:mm:ss");

            try {
                QFile file(filePath);
                if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
                    QMessageBox::critical(updateDialog, "Erro", "Não foi possível salvar as alterações no arquivo.");
                    return;
                }

                QTextStream out(&file);
                out << "type,username,password,changed_date\n";
                for (const QStringList& row : userData) {
                    out << row.join(",") << "\n";
                }
                file.close();

                QMessageBox::information(updateDialog, "Sucesso", "Usuário atualizado com sucesso.");
                updateDialog->accept();
            } catch (const std::exception& error) {
                QMessageBox::critical(updateDialog, "Erro", QString("Erro ao salvar o arquivo: %1").arg(error.what()));
            } catch (...) {
                QMessageBox::critical(updateDialog, "Erro", "Ocorreu um erro desconhecido ao tentar salvar o arquivo.");
            }
        });

        updateDialog->exec();
        return;
    } else { // Is Administrator
        QDialog* selectDialog = new QDialog();
        selectDialog->setWindowTitle("Selecionar Usuário para update");

        QComboBox *userComboBox = new QComboBox(selectDialog);
        QPushButton *selectButton = new QPushButton("Selecionar", selectDialog);
        QPushButton *cancelButton = new QPushButton("Cancelar", selectDialog);

        QVBoxLayout *selectLayout = new QVBoxLayout(selectDialog);
        selectLayout->addWidget(userComboBox);

        QHBoxLayout *selectionLayout = new QHBoxLayout();
        selectionLayout->addWidget(new QLabel("Selecione o usuário para update:"));
        selectionLayout->addWidget(userComboBox);
        selectLayout->addLayout(selectionLayout);

        QHBoxLayout *buttonLayout = new QHBoxLayout();
        buttonLayout->addWidget(selectButton);
        buttonLayout->addWidget(cancelButton);
        selectLayout->addLayout(buttonLayout);

        for (const QStringList& user : userData) {
            if (user.size() > 1) {
                userComboBox->addItem(user[1]);
            }
        }

        connect(cancelButton, &QPushButton::clicked, selectDialog, &QDialog::reject);
        connect(selectButton, &QPushButton::clicked, [&, this, selectDialog, userData]() {
            QString selectedUser = userComboBox->currentText();

            if (selectedUser.isEmpty()) {
                QMessageBox::warning(selectDialog, "Erro", "Selecione um usuário.");
                return;
            }

            QDialog *updateDialog = new QDialog(selectDialog);
            updateDialog->setWindowTitle("Atualizar Usuário");

            QComboBox *typeComboBox = new QComboBox;
            typeComboBox->addItem("Administrador");
            typeComboBox->addItem("Engenharia");
            typeComboBox->addItem("Produção");
            typeComboBox->addItem("Qualidade");

            QLineEdit *userNameEdit = new QLineEdit(selectedUser);
            QLineEdit *passwordEdit = new QLineEdit();

            QPushButton *updateButton = new QPushButton("Atualizar", updateDialog);
            QPushButton *cancelUpdateButton = new QPushButton("Cancelar", updateDialog);

            QVBoxLayout *mainLayout = new QVBoxLayout(updateDialog); // Corrected: Main layout added

            QFormLayout *formLayout = new QFormLayout();
            formLayout->addRow("Tipo:", typeComboBox);
            formLayout->addRow("Nome de Usuário:", userNameEdit);
            formLayout->addRow("Senha:", passwordEdit);
            mainLayout->addLayout(formLayout);

            QHBoxLayout *buttonLayout = new QHBoxLayout();
            buttonLayout->addWidget(updateButton);
            buttonLayout->addWidget(cancelUpdateButton);
            mainLayout->addLayout(buttonLayout);

            int selectedIndex = -1;
            for (int i = 0; i < userData.size(); ++i) {
                if (userData[i][1] == selectedUser) {
                    selectedIndex = i;
                    break;
                }
            }

            if (selectedIndex != -1) {
                typeComboBox->setCurrentText(userData[selectedIndex][0]);
            }

            connect(cancelUpdateButton, &QPushButton::clicked, updateDialog, &QDialog::reject);
            connect(updateButton, &QPushButton::clicked, [&, this, updateDialog, userData, selectedIndex]() mutable {
                QString newType = typeComboBox->currentText();
                QString newUsername = userNameEdit->text().trimmed();
                QString newPassword = passwordEdit->text();

                if (newUsername.isEmpty()) {
                    QMessageBox::critical(updateDialog, "Erro", "O nome de usuário não pode estar vazio.");
                    return;
                }

                if (newUsername.contains(" ")) {
                    QMessageBox::critical(updateDialog, "Erro", "O nome de usuário não pode ter espaços internos.");
                    return;
                }

                userData[selectedIndex][0] = newType;
                userData[selectedIndex][1] = newUsername;
                userData[selectedIndex][2] = newPassword;
                userData[selectedIndex][3] = QDateTime::currentDateTime().toString("dd-MM-yyyy-hh:mm:ss");

                try {
                    QFile file(filePath);
                    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
                        QMessageBox::critical(updateDialog, "Erro", "Não foi possível salvar as alterações no arquivo.");
                        return;
                    }

                    QTextStream out(&file);
                    out << "type,username,password,changed_date\n";
                    for (const QStringList& row : userData) {
                        out << row.join(",") << "\n";
                    }
                    file.close();

                QMessageBox::information(updateDialog, "Sucesso", "Usuário atualizado com sucesso.");
                updateDialog->accept(); // Fecha o diálogo após a atualização bem-sucedida
            } catch (const std::exception& error) {
                QMessageBox::critical(updateDialog, "Erro", QString("Erro ao salvar o arquivo: %1").arg(error.what()));
            } catch (...) {
                QMessageBox::critical(updateDialog, "Erro", "Ocorreu um erro desconhecido ao tentar salvar o arquivo.");
            }
        });

        updateDialog->exec();
    });

    selectDialog->exec();
    }
}

//  login de usuário
UserType UsersHandler::loginAccess() {
    full_user.username = "Comum"; // usuário base caso falha de login
    full_user.type = "Produção";

    QDialog dialog;
    dialog.setWindowTitle("Login de Usuário");



    QLineEdit *usernameEdit = new QLineEdit;
    usernameEdit->setPlaceholderText("Nome de Usuário");

    QLineEdit *passwordEdit = new QLineEdit;
    passwordEdit->setPlaceholderText("Senha");
    passwordEdit->setEchoMode(QLineEdit::Password);

    QPushButton *okButton = new QPushButton("OK");
    QPushButton *cancelButton = new QPushButton("Cancelar");

    QVBoxLayout *mainLayout = new QVBoxLayout(&dialog);
    QFormLayout *formLayout = new QFormLayout();
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
                if (fields[1] == enteredUsername && fields[2] == enteredPassword) {
                    passwordCorrect = true;
                    full_user.type = fields[0];
                    full_user.username = enteredUsername;
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
            QMessageBox::information(&dialog, "Sucesso", QString("Login realizado com sucesso!\nUsuário: %1\nTipo: %2").arg(full_user.username, full_user.type));
            dialog.accept(); // Aceita o diálogo apenas em caso de sucesso
        }
    });

    if (dialog.exec() == QDialog::Accepted) {
        return full_user;
    } else {
        return {"Comum", "Produção"}; // Retorna um usuário padrão se o login for cancelado
    }
}

UserType UsersHandler::getFullUser() const
{
    return full_user;
}

QString UsersHandler::getUserType() const
{
    return full_user.type;
}

QString UsersHandler::getUsername() const
{
    return full_user.username;
}

QString UsersHandler::getPassword() const
{
    return password;
}
