#include "user_center.h"
#include "ui_user_center.h"
#include "mainwindow.h"
#include <QMessageBox>
#include <QDebug>
#include <QTimer>
#include "global.h"
/*
 * @brief 构造函数
 * @param parent 父窗口
 */
user_center::user_center(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::user_center)
{
    ui->setupUi(this);
    this->setWindowTitle("用户中心");

    myInfo = new SelfInfo(this);
    change_pwd = new changePwd(this);
    rech = new recharge(this);

    ui->stackedWidget->addWidget(myInfo);
    ui->stackedWidget->addWidget(change_pwd);
    ui->stackedWidget->addWidget(rech);
    //默认显示个人信息界面
    ui->stackedWidget->setCurrentWidget(myInfo);
}

/*
 * @brief 析构函数
 */
user_center::~user_center()
{
    delete ui;
}

void user_center::on_btn_index_clicked()
{
    emit centerBack();
}

void user_center::on_btn_info_clicked()
{
    ui->stackedWidget->setCurrentWidget(myInfo);
}

void user_center::on_btn_changePwd_clicked()
{
    ui->stackedWidget->setCurrentWidget(change_pwd);
}

void user_center::on_btn_recharge_clicked()
{
    ui->stackedWidget->setCurrentWidget(rech);
}

void user_center::on_btn_mback_clicked()
{
    if(global::login_state==2||global::login_state==3)
    {
        gManage = new GoodsManage(this);
        gManage->show();
    }
    else
        QMessageBox::critical(this,"错误","您不是商家！");
}

void user_center::on_btn_chart_clicked()
{
    QMessageBox::information(this,"提示","请期待2.0版本!");
}

void user_center::on_pushButton_2_clicked()
{
    QMessageBox::information(this,"提示","请期待2.0版本!");
}
