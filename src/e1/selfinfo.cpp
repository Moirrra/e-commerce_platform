#include "selfinfo.h"
#include "ui_selfinfo.h"
#include "global.h"
#include "filehelp.h"
#include <QDebug>
/*
 * @brief 构造函数
 * @param parent 父窗口
 */
SelfInfo::SelfInfo(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SelfInfo)
{
    ui->setupUi(this);
    upDateInfo();

    myTimer = new QTimer(this);
    connect(myTimer,SIGNAL(timeout()),this,SLOT(myTimeOut()));
    myTimer->start(1000);

}

/*
 * @brief 析构函数
 */
SelfInfo::~SelfInfo()
{
    delete ui;
}


/*
 * @brief 更新账户信息
 */
void SelfInfo::upDateInfo()
{
    FileHelp* fileHelp = new FileHelp();
    ui->label_name->setText(global::userName);
    if(global::login_state==1)
    {
        ui->label_type->setText("消费者");
        if(!fileHelp->findValue("customer.txt",global::userName,money,2))
            qDebug()<<"find money error";
    }
    else if(global::login_state==2||global::login_state==3)
    {
        ui->label_type->setText("商家");
        if(!fileHelp->findValue("merchant.txt",global::userName,money,2))
            qDebug()<<"find money error";
    }

    ui->label_money->setText(money);
}

/*
 * @brief 超时更新账户信息
 */
void SelfInfo::myTimeOut()
{
    upDateInfo();
}
