#include "changepwd.h"
#include "ui_changepwd.h"
#include <QMessageBox>
#include <QDebug>
#include <QTextStream>
#include <QDir>
#include <QString>
#include "filehelp.h"
#include "global.h"
/*
 * @brief 构造函数
 * @param parent 父窗口
 * @param ui ui界面
 */
changePwd::changePwd(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::changePwd)
{
    ui->setupUi(this);
    fileHelp = new FileHelp();
}

/*
 * @brief 析构函数
 */
changePwd::~changePwd()
{
    delete ui;
}

/*
 * @brief 点击确定保存更改的密码
 */
void changePwd::on_pushButton_clicked()
{
    pwd = ui->pwd1->text();
    r_pwd = ui->pwd2->text();
    if(pwd!=r_pwd)
    {
        QMessageBox::critical(this,"错误","两次输入密码不同，请再次确认！");
        return;
    }

    if(global::login_state == 1)
    {
        if(!fileHelp->writeFile("customer.txt",global::userName,pwd,1))
        {
            QMessageBox::critical(this,"错误","密码修改失败！");
            return;
        }
        QMessageBox::information(this,"修改成功","密码修改成功！");
    }
    else
    {
        if(!fileHelp->writeFile("merchant.txt",global::userName,pwd,1))
        {
            QMessageBox::critical(this,"错误","密码修改失败！");
            return;
        }
        QMessageBox::information(this,"修改成功","密码修改成功！");
    }
    ui->pwd1->text().clear();
    ui->pwd2->text().clear();
}
