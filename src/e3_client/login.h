#ifndef LOGIN_H
#define LOGIN_H

#include <QWidget>
#include <QButtonGroup>

namespace Ui {
class Login;
}

class Login : public QWidget
{
    Q_OBJECT

public:
    explicit Login(QWidget *parent = nullptr);
    ~Login();


private slots:
    void on_btn_reg_clicked();
    void on_btn_login_clicked();

private:
    Ui::Login *ui;
    QString userType;
    QString userName;
    QString password;
    double money;
    QButtonGroup* typeGroup;

signals:
    void sendMsg(QString);

};

#endif // LOGIN_H
