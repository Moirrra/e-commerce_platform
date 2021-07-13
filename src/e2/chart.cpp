#include "chart.h"
#include "ui_chart.h"
#include <QPixmap>
#include <QMessageBox>
#include <QString>
#include <QSqlQuery>
#include <QTime>
#include <QDate>
#include <QList>
#include <QVBoxLayout>
#include "global.h"
#include <QDebug>
#include "order.h"
Chart::Chart(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Chart)
{
    ui->setupUi(this);
    this->setWindowTitle("我的购物车");
    cust = new Customer();
    cust->setUserName(global::userName);
    cust->setBalance(global::money);

    //显示商品信息
    pLayout = new QVBoxLayout(this);
    itemList.clear();

    if(!cust->updateChartInfo())
    {
        QMessageBox::critical(this,"错误","读取数据库失败！");
    }
    QList<Goods> goodslist = cust->getChartList();
    for(int i = 0; i < goodslist.size(); i++)
    {

        ChartItem* item = new ChartItem();
        item->InitItem(goodslist[i]);
        if(item->isValid)
        {
            itemList.append(item);
        }
        else
        {
            QString str = QString("商品(ID:%1)已失效！").arg(item->getGoodsID());
            QMessageBox::warning(this,"警告",str);
            goodslist.removeAt(i);
            cust->deleteChartItem(item->getGoodsID());//数据库中删除
            cust->updateChartInfo();
            i--;
        }
    }
    for(int i = 0; i < itemList.size(); i++)
    {
        pLayout->addWidget(itemList[i]);
    }
    if(!itemList.isEmpty())
    {
        ui->scrollAreaWidgetContents->setLayout(pLayout);
        pLayout->setContentsMargins(0, 0, 0, 0);
    }

}

Chart::~Chart()
{
    delete ui;
}

//结算
void Chart::on_pushButton_clicked()
{
    //购物车空
    if(itemList.isEmpty())
    {
        QMessageBox::critical(this,"错误","您的购物车是空的！");
        return;
    }
    //选中商品
    for(int i = 0; i < itemList.size(); i++)
    {
        if(itemList[i]->isChecked)
        {
            qDebug()<<"goodID"<<itemList[i]->getGoodsID();
            qDebug()<<"goodCount"<<itemList[i]->getGoodsCount();
            //修改数据库中商品数量
            if(!cust->changeChartItemCount(itemList[i]->getGoodsID(),itemList[i]->getGoodsCount()))
            {
                qDebug()<<"修改数据库商品数量失败";
                QMessageBox::critical(this,"错误","写入数据库失败！");
                return;
            }
            selectList.append(i);
        }
    }
    if(selectList.isEmpty())
    {
        QMessageBox::critical(this,"错误","您未选择任何商品！");
        return;
    }
    if(!cust->updateChartInfo())//更新数据库至列表
    {
        qDebug()<<"用户购物车信息更新失败！";
        return;
    }
    //选择成功
    order = new Order();
    order->InitOrder(selectList);
    order->show();
    this->hide();
    qDebug()<<"跳转订单页面";
}

//全选
void Chart::on_btn_selectAll_clicked()
{
    for(int i = 0; i < itemList.size(); i++)
    {
        itemList[i]->setChecked();
    }
}

//删除选中项
void Chart::on_btn_delete_clicked()
{
    for(int i = 0; i < itemList.size(); i++)
    {
        if(itemList[i]->isChecked)
        {
            pLayout->removeWidget(itemList[i]);//显示删除
            cust->deleteChartItem(itemList[i]->getGoodsID());//数据库中删除
            itemList.removeAt(i);
            i--;//删除后序号会改变
        }
    }
    if(!cust->updateChartInfo())//更新数据库至列表
        qDebug()<<"用户购物车信息更新失败！";
}

