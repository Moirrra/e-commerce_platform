#include "goodsshow.h"
#include "ui_goodsshow.h"
#include <QMessageBox>
#include <QDebug>
GoodsShow::GoodsShow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GoodsShow)
{
    ui->setupUi(this);
    userType = 0;

}

GoodsShow::~GoodsShow()
{
    delete ui;
}

/*
 * @brief 初始化界面商品信息
 */
void GoodsShow::InitData()
{
    ui->name->setText(curGoods.getName());
    ui->size->setText(curGoods.getSize());
    ui->price->setText(QString::number(curGoods.getPrice()));
    ui->stock->setText(QString::number(curGoods.getCount()));
    ui->label_seller->setText(curGoods.getSeller());
    ui->textBrowser->setText(curGoods.getDetails());
    ui->count->setRange(1,curGoods.getCount());
    ui->count->clear();
    this->setWindowTitle(curGoods.getName());
}

/*
 * @brief 添加至用户购物车
 */
void GoodsShow::on_btn_add_clicked()
{
    if(userType==0)
    {
        QMessageBox::critical(this,"错误","您未登录！");
        return;
    }
    if(userType!=1)
    {
        QMessageBox::critical(this,"错误","您不是消费者！");
        return;
    }
    qDebug()<<ui->count->value();
    emit addChart(curGoods.getName(),ui->count->value());
}

