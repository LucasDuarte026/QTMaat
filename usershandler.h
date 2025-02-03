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

    void showAddUserDialog(); //função que adiciona um novo usuário
    void showViewUsersDialog();  // função que abre a visibilidade dos usuários do sistema

    // Aba de login
    UserType loginAccess() ;
    UserType getFullUser() const;
    QString getUserType() const;
    QString getUsername() const;
    QString getPassword() const;

private:
    bool addUser(const QString &type, const QString &userName, const QString &password);
    void createFileWithHeader(const QString& filePath);

    bool validateCredentials(const QString &type, const QString &username, const QString &password);
    UserType full_user;
    QString user_type;
    QString username;
    QString password;
};

#endif // USERSHANDLER_H
