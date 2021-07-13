#include "login.h"
#include "ui_login.h"
#include <QMessageBox>

Login::Login(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Login)
{
    ui->setupUi(this);

    this->setWindowTitle("登录");
    //账户类型选择
    typeGroup = new QButtonGroup(this);
    typeGroup->addButton(ui->radioButton,0);
    typeGroup->addButton(ui->radioButton_2,1);
    //输入框限制输入字母/数字/下划线
    QRegExp regx("[a-zA-Z0-9_]+$");
    ui->password->setValidator(new QRegExpValidator(regx,this));
    ui->userName->setValidator(new QRegExpValidator(regx,this));
}

Login::~Login()
{
    delete ui;
}

/*
 * @brief 点击登录
 */
void Login::on_btn_login_clicked()
{
    userName = ui->userName->text();
    password = ui->password->text();

    //未完全输入
    if(ui->userName->text().isEmpty())
    {
        QMessageBox::critical(this,"警告","用户名为空！","确认");
        return;
    }
    if(ui->password->text().isEmpty())
    {
        QMessageBox::critical(this,"警告","密码为空！","确认");
        return;
    }
    if(!ui->radioButton->isChecked()&&!ui->radioButton_2->isChecked())
    {
        QMessageBox::critical(this,"警告","请选择用户类型！","确认");
        return;
    }

    //顾客登录信息确认
    if(ui->radioButton->isChecked())
    {
        userType = '1';
        QString loginINS = "l#"+userType+"#"+userName+"#"+password;
        emit sendMsg(loginINS);

    }

    //商家登录信息确认
    if(ui->radioButton_2->isChecked())
    {
        userType = '2';
        QString loginINS = "l#"+userType+"#"+userName+"#"+password;
        emit sendMsg(loginINS);
    }
    ui->userName->clear();
    ui->password->clear();
}


/*
 * @brief 点击注册
 */
void Login::on_btn_reg_clicked()
{
    userName = ui->userName->text();
    password = ui->password->text();

    //未完全输入
    if(ui->userName->text().isEmpty())
    {
        QMessageBox::critical(this,"警告","用户名为空！","确认");
        return;
    }
    if(ui->password->text().isEmpty())
    {
        QMessageBox::critical(this,"警告","密码为空！","确认");
        return;
    }
    if(!ui->radioButton->isChecked()&&!ui->radioButton_2->isChecked())
    {
        QMessageBox::critical(this,"警告","请选择用户类型！","确认");
        return;
    }
    //顾客注册信息确认
    if(ui->radioButton->isChecked())
    {
        userType = '1';
        QString registINS = "r#"+userType+"#"+userName+"#"+password;
        emit sendMsg(registINS);
    }

    //商家注册信息确认
    if(ui->radioButton_2->isChecked())
    {
        userType = '2';
        QString registINS = "r#"+userType+"#"+userName+"#"+password;
        emit sendMsg(registINS);
    }
    ui->userName->clear();
    ui->password->clear();
}

