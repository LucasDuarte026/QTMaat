#include "userlogin.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QComboBox>
#include <QLineEdit>
#include <QPushButton>
#include <QMessageBox>
#include <QFile>
#include <QTextStream>
#include <QDateTime>

UserLogin::UserLogin(QWidget *parent)
    : QDialog(parent)
    , userTypeCombo(new QComboBox(this))
    , usernameEdit(new QLineEdit(this))
    , passwordEdit(new QLineEdit(this))
    , okButton(new QPushButton("OK", this))
    , cancelButton(new QPushButton("Cancelar", this))
{
    setWindowTitle("Login de Usuário");

    // Setup user type combo box
    userTypeCombo->addItems({"Produção", "Administrador", "Engenharia", "Qualidade"});
    userTypeCombo->setCurrentText("Produção"); // Pre-select Production

    // Setup password field to show dots
    passwordEdit->setEchoMode(QLineEdit::Password);

    // Create layout
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    // User type selection
    QHBoxLayout *typeLayout = new QHBoxLayout();
    typeLayout->addWidget(new QLabel("Tipo de Usuário:"));
    typeLayout->addWidget(userTypeCombo);
    mainLayout->addLayout(typeLayout);

    // Username
    QHBoxLayout *userLayout = new QHBoxLayout();
    userLayout->addWidget(new QLabel("Usuário:"));
    userLayout->addWidget(usernameEdit);
    mainLayout->addLayout(userLayout);

    // Password
    QHBoxLayout *passLayout = new QHBoxLayout();
    passLayout->addWidget(new QLabel("Senha:"));
    passLayout->addWidget(passwordEdit);
    mainLayout->addLayout(passLayout);

    // Buttons
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget(okButton);
    buttonLayout->addWidget(cancelButton);
    mainLayout->addLayout(buttonLayout);

    // Connect signals
    connect(okButton, &QPushButton::clicked, this, &UserLogin::onOkClicked);
    connect(cancelButton, &QPushButton::clicked, this, &QDialog::reject);

    // Set minimum size
    setMinimumWidth(300);
}

UserLogin::~UserLogin()
{
}

QString UserLogin::getUserType() const
{
    return userTypeCombo->currentText();
}

QString UserLogin::getUsername() const
{
    return usernameEdit->text();
}

QString UserLogin::getPassword() const
{
    return passwordEdit->text();
}

bool UserLogin::validateCredentials(const QString &type, const QString &username, const QString &password)
{
    QFile file("LoginCredentials.csv");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::critical(this, "Erro", "Não foi possível abrir o arquivo de credenciais");
        return false;
    }

    QTextStream in(&file);
    // Skip header if exists
    QString line = in.readLine();

    while (!in.atEnd()) {
        line = in.readLine();
        QStringList fields = line.split(',');
        if (fields.size() >= 4) {
            QString fileType = fields[0];
            QString fileUsername = fields[2];
            QString filePassword = fields[3];

            if (fileType == type && fileUsername == username && filePassword == password) {
                file.close();
                return true;
            }
        }
    }

    file.close();
    return false;
}

void UserLogin::onOkClicked()
{
    QString type = getUserType();
    QString username = getUsername();
    QString password = getPassword();

    if (username.isEmpty() || password.isEmpty()) {
        QMessageBox::warning(this, "Entrada inválida", "Por favor, preencha todos os campos");
        return;
    }

    if (validateCredentials(type, username, password)) {
        accept();
    } else {
        QMessageBox::warning(this, "Login falhou", "Credenciais inválidas");
    }
}
