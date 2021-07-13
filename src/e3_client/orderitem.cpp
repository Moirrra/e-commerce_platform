#include "orderitem.h"
#include "ui_orderitem.h"

OrderItem::OrderItem(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::OrderItem)
{
    ui->setupUi(this);
    this->setStyleSheet("QWidget{background-color:white;border-radius:5px;}");
}

OrderItem::~OrderItem()
{
    delete ui;
}

//初始化订单商品信息
void OrderItem::InitItem(QString n, int c, double p, int s)
{
    curGoods.setName(n);
    curGoods.setCount(c);
    curGoods.setPrice(p);
    stock = s;
    ui->label_name->setText(n);
    ui->label_price->setText(QString::number(p,'f',2));
    ui->label_count->setText(QString::number(curGoods.getCount()));
    total = curGoods.getSum();
    ui->label_total->setText(QString::number(total));
}
