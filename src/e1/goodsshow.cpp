#include "goodsshow.h"
#include "ui_goodsshow.h"
#include <QSqlQuery>
#include <QDebug>
#include <QByteArray>
#include <QMessageBox>
#include "searchresult.h"
/*
 * @brief 构造函数
 * @param parent 父窗口
 * @param ui ui界面
 */
GoodsShow::GoodsShow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GoodsShow)
{
    ui->setupUi(this);

    sqlHelp = new SqlHelp();
    /*搜索框*/
    searchLine = new AppSearchLine(ui->widget_search);
    searchLine->initCompleter();
    searchLine->setFixedWidth(450);
    //搜索
    connect(searchLine->searchBtn,&QPushButton::clicked,[=](){
        QString name = searchLine->text();
        qDebug()<<name;
        searchLine->clear();
        SearchResult* resultPage = new SearchResult();
        resultPage->findResult(name);
        resultPage->show();
    });

    //数量
    ui->spinBox->setRange(1,curGoods.getRemnant());
}

/*
 * @brief 析构函数
 */
GoodsShow::~GoodsShow()
{
    delete ui;
}


//初始化页面信息用
void GoodsShow::setShowPage(QString ID)
{
    curGoods.setID(ID);
    if(!initGoodsInfo())
    {
        QMessageBox::critical(this,"错误","找不到该商品！");
    }
}


bool GoodsShow::initGoodsInfo()
{
    QSqlQuery query;
    QString sql = QString("SELECT * FROM Goods WHERE GoodsId = '%1'").arg(curGoods.getID());
    query.exec(sql);
    if(query.first())
    {
        qDebug()<<"???";
        curGoods.setName(query.value(1).toString());
        curGoods.setType(query.value(2).toString());
        curGoods.setSize(query.value(3).toString());
        curGoods.setPrice(query.value(4).toDouble());
        curGoods.setUnit(query.value(7).toString());
        curGoods.setCount(query.value(5).toInt());
        curGoods.setSeller(query.value(8).toString());
        curGoods.setDiscount(query.value(10).toDouble());
        QByteArray outByteArray = query.value(11).toByteArray();
        QPixmap tempImg;
        tempImg.loadFromData(outByteArray);
        curGoods.setImg(tempImg);
        curGoods.setDetails(query.value(12).toString());
        InitData();
        return true;
    }
    return false;
}

/*
 * @brief 初始化商品展示信息
 */
void GoodsShow::InitData()
{
    this->setWindowTitle(curGoods.getName());
    ui->label_name->setText(curGoods.getName());
    ui->label_price->setText("¥"+QString::number(curGoods.getPrice(),'f',2));
    ui->label_stock->setText(QString::number(curGoods.getCount()));
    ui->label_img->setPixmap(curGoods.getImage().scaled(420,300,Qt::KeepAspectRatio, Qt::SmoothTransformation));
    ui->textBrowser->setText(curGoods.getDetails());
}


/*
 * @brief 点击购物车
 */
void GoodsShow::on_btn_chart_clicked()
{
    QMessageBox::information(this,"提示","请期待2.0版本!");
}

//加入购物车
void GoodsShow::on_btn_addChart_clicked()
{
    QMessageBox::information(this,"提示","请期待2.0版本!");
}


//立即购买
void GoodsShow::on_btn_buy_clicked()
{
    QMessageBox::information(this,"提示","请期待2.0版本!");
}
