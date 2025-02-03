#ifndef USERLOGIN_H
#define USERLOGIN_H

#include <QDialog>
#include <QDateTime>

class QComboBox;
class QLineEdit;
class QPushButton;

class UserLogin : public QDialog
{
    Q_OBJECT

public:
    explicit UserLogin(QWidget *parent = nullptr);
    ~UserLogin();

    QString getUserType() const;
    QString getUsername() const;
    QString getPassword() const;

private:
    QComboBox *userTypeCombo;
    QLineEdit *usernameEdit;
    QLineEdit *passwordEdit;
    QPushButton *okButton;
    QPushButton *cancelButton;

    bool validateCredentials(const QString &type, const QString &username, const QString &password);

private slots:
    void onOkClicked();
};

#endif // USERLOGIN_H
