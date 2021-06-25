#include "searchresultitem.h"
#include "ui_searchresultitem.h"
#include <QSqlQuery>
#include <QDebug>
#include "goodsshow.h"
SearchResultItem::SearchResultItem(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SearchResultItem)
{
    ui->setupUi(this);
    this->setStyleSheet("QWidget{background-color:white;border-radius:5px;}");
    sqlHelp = new SqlHelp();
}

SearchResultItem::~SearchResultItem()
{
    delete ui;
}

void SearchResultItem::initInfo(QString GoodsID)
{
    curGoods.setID(GoodsID);
    //获取数据
    QSqlQuery query;
    QString sql = QString("SELECT * FROM Goods WHERE GoodsId = '%1' ").arg(GoodsID);
    query.exec(sql);
    if(query.first())
    {
        curGoods.setName(query.value(1).toString());
        curGoods.setPrice(query.value(4).toDouble());
        curGoods.setSeller(query.value(8).toString());
        QByteArray outByteArray = query.value(11).toByteArray();
        QPixmap tempImg;
        tempImg.loadFromData(outByteArray);
        curGoods.setImg(tempImg);
    }
    else
    {
        qDebug()<<"not found";
    }
    //显示
    QPixmap pixmap = curGoods.getImage();
    pixmap.scaled(ui->label_img->size(),Qt::KeepAspectRatio,Qt::SmoothTransformation);
    ui->label_img->setPixmap(pixmap);
    ui->label_img->setScaledContents(true);
    ui->label_name->setText(curGoods.getName());
    ui->label_price->setText(QString::number(curGoods.getPrice(),'f',2));
    ui->label_seller->setText("商家"+curGoods.getSeller());
}

void SearchResultItem::mousePressEvent(QMouseEvent *event)
{
    GoodsShow* showPage = new GoodsShow();
    showPage->setShowPage(curGoods.getID());
    showPage->show();
}

