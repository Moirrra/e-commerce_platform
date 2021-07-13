#include "orderitem.h"
#include "ui_orderitem.h"
#include <QSqlQuery>
#include <QMessageBox>
#include <QDebug>

OrderItem::OrderItem(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::OrderItem)
{
    ui->setupUi(this);
    this->setStyleSheet("QWidget{background-color:white;border-radius:5px;}");
    goods.setName("unkown");
    goods.setSize("unkown");
    goods.setPrice(0);
    goods.setSeller("unkown");
    total = 0;
}

OrderItem::~OrderItem()
{
    delete ui;
}

void OrderItem::InitItem(Goods newGoods)
{
    goods = newGoods;
    QSqlQuery query;
    QString sql = QString("SELECT * FROM Goods WHERE GoodsId = '%1' ").arg(newGoods.getID());
    if(!query.exec(sql))
    {
        QMessageBox::critical(this,"错误","读取数据库失败！");
    }
    if(!query.first())
    {
        QString str = QString("商品(ID:%1)已失效！").arg(newGoods.getID());
        QMessageBox::warning(this,"警告",str);
    }
    goods.setName(query.value(1).toString());
    goods.setSize(query.value(3).toString());
    goods.setPrice(query.value(4).toDouble());
    goods.setSeller(query.value(8).toString());
    QByteArray outByteArray = query.value(11).toByteArray();
    QPixmap tempImg;
    tempImg.loadFromData(outByteArray);
    tempImg.scaled(ui->label_img->size(),Qt::KeepAspectRatio,Qt::SmoothTransformation);
    ui->label_img->setPixmap(tempImg);
    ui->label_img->setScaledContents(true);
    ui->label_name->setText(goods.getName());
    ui->label_price->setText(QString::number(goods.getPrice(),'f',2));
    ui->label_count->setText(QString::number(goods.getCount()));
    ui->label_seller->setText("商家："+goods.getSeller());
    ui->label_size->setText(goods.getSize());
    total = goods.getSum();
    ui->label_sum->setText(QString::number(total,'f',2));
}
