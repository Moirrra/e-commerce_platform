#ifndef LOGIN_H
#define LOGIN_H

#include <QMainWindow>
#include <QList>
#include <QButtonGroup>
#include <QTimer>

namespace Ui {
class login;
}

class login : public QMainWindow
{
    Q_OBJECT

public:
    explicit login(QWidget *parent = nullptr);
    ~login();

    int readCustomerFile();
    int readMerchantFile();
    int appendCustomerFile();
    int appendMerchantFile();
    void readInfo();


signals:
    void loginSceneBack();

private slots:
    void on_btn_index_clicked();

    void on_btn_login_clicked();

    void on_btn_reg_clicked();

private:
    Ui::login *ui;

    QList<QString> customerList, merchantList;//账号密码信息
    QString userName;
    QString password;
    double money;
    QButtonGroup *TypeGroup;


};



#endif // LOGIN_H
