#ifndef USERSHANDLER_H
#define USERSHANDLER_H

#include <QObject>
#include <QDateTime>
#include <QDialog>
#include <QStringList>

class UsersHandler : public QObject
{
    Q_OBJECT

public:
    explicit UsersHandler(QObject *parent = nullptr);
    ~UsersHandler();

    void showAddUserDialog();


private:
    bool addUser(const QString &type, const QString &userName, const QString &password);
    void createFileWithHeader(const QString& filePath);


};

#endif // USERSHANDLER_H
