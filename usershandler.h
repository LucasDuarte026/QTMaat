#ifndef USERSHANDLER_H
#define USERSHANDLER_H

#include <QObject>
#include <QDateTime>
#include <QDialog>
#include <QStringList>
#include "datatypes.h"
class UsersHandler : public QObject
{
    Q_OBJECT

public:
    explicit UsersHandler(QObject *parent = nullptr);
    ~UsersHandler();

    void showAddUserDialog();       // Função que adiciona um novo usuário
    void showViewUsersDialog();     // Função que abre a visibilidade dos usuários do sistema
    void showRemoveUserDialog();    // Função para remover um usuário
    void showUpdateUserDialog();    // Função para dar update no usuário já cadastrado

    // Aba de login
    UserType loginAccess() ;
    UserType getFullUser() const;
    QString getUserType() const;
    QString getUsername() const;
    QString getPassword() const;

private:
    bool addUser(const QString &type, const QString &userName, const QString &password); // função acessora inserção
    void createFileWithHeader(const QString& filePath); // função acessora inserção
    bool removeUser(const QString& username); // função acessora para remoção


    //  Login
    bool validateCredentials(const QString &type, const QString &username, const QString &password); // validar as credenciais para login
    //    Variáveis que seguram os dados atuais do usuário feito login
    UserType full_user;
    QString user_type;
    QString username;
    QString password;
};

#endif // USERSHANDLER_H
