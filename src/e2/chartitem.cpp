#include "chartitem.h"
#include "ui_chartitem.h"
#include <QSqlQuery>
#include <QMessageBox>
#include <QDebug>
ChartItem::ChartItem(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ChartItem)
{
    ui->setupUi(this);
    this->setStyleSheet("QWidget{background-color:white;border-radius:5px;}"
                        "QSpinBox{background-color:#F7F7F7;}");
    goods.setName("unkown");
    goods.setSize("unkown");
    goods.setPrice(0);
    goods.setSeller("unkown");
    isChecked = 0;
    isValid = 1;

}

ChartItem::~ChartItem()
{
    delete ui;
}

void ChartItem::InitItem(Goods newGoods)
{
    goods = newGoods;
    qDebug()<<goods.getCount();
    QSqlQuery query;
    QString sql = QString("SELECT * FROM Goods WHERE GoodsId = '%1' ").arg(newGoods.getID());
    if(!query.exec(sql))
    {
        QMessageBox::critical(this,"错误","读取数据库失败！");
    }
    if(!query.first())
    {
        isValid = 0;
    }
    if(isValid)
    {
        goods.setID(query.value(0).toString());
        goods.setName(query.value(1).toString());
        int maxNum = query.value(5).toInt();
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
        ui->label_price->setText("¥"+QString::number(goods.getPrice(),'f',2));
        ui->spinBox->setValue(goods.getCount());
        ui->spinBox->setRange(1,maxNum);
        ui->label_seller->setText("商家："+goods.getSeller());
        ui->label_size->setText(goods.getSize());
        ui->radioButton->setChecked(false);
    }
    else
    {
        ui->label_name->setText("商品已失效");
    }
}

void ChartItem::setChecked()
{
    ui->radioButton->setChecked(true);
    isChecked = 1;
}

QString ChartItem::getGoodsID()
{
    return goods.getID();
}

int ChartItem::getGoodsCount()
{
    return goods.getCount();
}

void ChartItem::on_radioButton_toggled(bool checked)
{
    if(checked==true)
    {
        isChecked = 1;
    }
    else isChecked = 0;
    qDebug()<<"isChecked = "<<isChecked;

}

//数量修改
void ChartItem::on_spinBox_valueChanged(int arg1)
{
    goods.setCount(arg1);
}
