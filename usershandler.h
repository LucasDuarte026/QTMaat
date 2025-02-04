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

    void showAddUserDialog(UserType myUser);       // Função que adiciona um novo usuário
    void showViewUsersDialog(UserType myUser);     // Função que abre a visibilidade dos usuários do sistema
    void showRemoveUserDialog(UserType myUser);    // Função para remover um usuário
    void showUpdateUserDialog(UserType myUser);    // Função para dar update no usuário já cadastrado

    // Aba de login
    UserType loginAccess() ;
    UserType getFullUser() const;
    QString getUserType() const;
    QString getUsername() const;
    QString getPassword() const;

private:
    bool addUser(const QString &type, const QString &userName, const QString &password); // função acessora inserção
    void createFileWithHeader(const QString& filePath); // função acessora inserção
    bool removeUser(const QString &userName,QVector<QStringList> userData);// função acessora para remoção


    //  Login
    bool validateCredentials(const QString &type, const QString &username, const QString &password); // validar as credenciais para login
    //    Variáveis que seguram os dados atuais do usuário feito login
    UserType full_user;
    QString user_type;
    QString username;
    QString password;
};

#endif // USERSHANDLER_H
