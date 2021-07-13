#include "goodsshow.h"
#include "ui_goodsshow.h"
#include <QSqlQuery>
#include <QDebug>
#include <QByteArray>
#include <QMessageBox>
#include "searchresult.h"
#include "chart.h"
#include "global.h"
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
    if(global::login_state==1)
    {
        cust = new Customer();
        cust->setUserName(global::userName);
        cust->setBalance(global::money);
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
        curGoods.setCount(query.value(5).toInt());
        curGoods.setUnit(query.value(7).toString());
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

    ui->spinBox->setRange(1,curGoods.getCount());
}


/*
 * @brief 点击购物车
 */
void GoodsShow::on_btn_chart_clicked()
{
    if(global::login_state==0)
    {
        QMessageBox::warning(this,"警告","请您先登录！");
        return;
    }
    if(global::login_state==2||global::login_state==3)
    {
        QMessageBox::information(this,"提示","您不是消费者！");
        return;
    }
    Chart* chart = new Chart();
    chart->show();
}

//加入购物车
void GoodsShow::on_btn_addChart_clicked()
{
    if(global::login_state==0)
    {
        QMessageBox::critical(this,"错误","请先登录！");
        return;
    }
    if(global::login_state==2||global::login_state==3)
    {
        QMessageBox::information(this,"提示","您不是消费者！");
        return;
    }
    Goods partGoods;
    partGoods.setID(curGoods.getID());
    partGoods.setCount(ui->spinBox->value());
    //提示
    QString str = QString("成功加入购物车！数量：%1").arg(partGoods.getCount());
    QMessageBox::information(this,"提示",str);
    cust->updateChartInfo();
    //加入用户购物车并保存至数据库
    if(!cust->addToChartList(partGoods))
    {
        QMessageBox::warning(this,"警告","数据库写入失败！");
        return;
    }
}


//立即购买
void GoodsShow::on_btn_buy_clicked()
{
    QMessageBox::information(this,"提示","此功能见 Version 2.0");
}



