#include "recharge.h"
#include "ui_recharge.h"
#include <QMessageBox>
#include <QTextStream>
#include <QFile>
#include <QDebug>
#include "global.h"

/*
 * @brief 构造函数
 * @param parent 父窗口 继承自QDialog
 */
recharge::recharge(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::recharge)
{
    ui->setupUi(this);
    fileHelp = new FileHelp();
    QString val;
    if(global::login_state==1)
    {
        if(!fileHelp->findValue("customer.txt",global::userName,val,2))
        {
            qDebug()<<"find money error";
        }

    }
    else if(global::login_state==2||global::login_state==3)
    {
        if(!fileHelp->findValue("merchant.txt",global::userName,val,2))
        {
            qDebug()<<"find money error";
        }
    }

    curMoney = val.toDouble();
     ui->label_cur->setText(QString::number(curMoney,'f',2));
    myTimer = new QTimer(this);
    connect(myTimer,SIGNAL(timeout()),this,SLOT(myTimeOut()));
    myTimer->start(1000);
}

/*
 * @brief 析构函数
 */
recharge::~recharge()
{
    delete ui;
}

/*
 * @brief 点击确认充值
 */
void recharge::on_btn_sure_clicked()
{
    addMoney = ui->doubleSpinBox->value();
    if(addMoney == 0)
    {
        QMessageBox::critical(this,"错误","充值金额为零，请重新输入！","确认");
        return;
    }

    if (global::login_state == 1)
    {
        sum = addMoney + curMoney;
        if(fileHelp->writeFile("customer.txt",global::userName,QString::number(sum,'f',2),2))
        {
            QMessageBox::information(this,"充值成功","余额充值成功！");
            curMoney = sum;
        }
    }
    else
    {
        sum = addMoney + curMoney;
        if(fileHelp->writeFile("merchant.txt",global::userName,QString::number(sum,'f',2),2))
        {
            QMessageBox::information(this,"充值成功","余额充值成功！");
            curMoney = sum;
        }
    }
    ui->doubleSpinBox->clear();
}
/*
 * @brief 超时更新账户余额
 */
void recharge::myTimeOut()
{
    if (global::login_state == 1)
        ui->label_cur->setText(QString::number(curMoney,'f',2));
    else if(global::login_state==2||global::login_state==3)
        ui->label_cur->setText(QString::number(curMoney,'f',2));
}


