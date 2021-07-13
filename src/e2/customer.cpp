#include "customer.h"
#include <QSqlQuery>
#include <QMessageBox>
#include <QDebug>
Customer::Customer()
{
    userType = "消费者";
}

QString Customer::getUserType()
{
    return userType;
}

//添加列表商品至购物车并保存
bool Customer::addToChartList(Goods newGoods)
{
    QSqlQuery query;
    int i = 0;
    for(i = 0; i < ChartList.size(); i++)
    {
        //相同商品不做处理
        if(ChartList[i].getID()==newGoods.getID())
        {
            qDebug()<<"already in chart!";
            return true;
        }
    }
    if(i==ChartList.size())//无需合并直接加入
    {
        ChartList.append(newGoods);
        //保存数据库中
        QString add = QString("INSERT INTO Chart VALUES ('%1', '%2', '%3')")
                .arg(getUserName()).arg(newGoods.getID()).arg(newGoods.getCount());
        if(!query.exec(add))
        {
            return false;
        }
    }

    return true;
}


QList<Goods> Customer::getChartList()
{
    return ChartList;
}

void Customer::clearChartList()
{
    ChartList.clear();
}

//根据数据库更新账户的购物车信息
bool Customer::updateChartInfo()
{
    clearChartList();
    QSqlQuery query;
    QString sql = QString("SELECT * FROM Chart WHERE UserName = '%1' ").arg(getUserName());
    qDebug()<<userName;
    if(!query.exec(sql))
    {
        qDebug()<<"false";
        return false;
    }
    while(query.next())
    {
        Goods tempGoods;
        tempGoods.setID(query.value(1).toString());
        qDebug()<<"ID:"<<tempGoods.getID();
        tempGoods.setCount(query.value(2).toInt());
        addToChartList(tempGoods);
    }
    return true;
}

//更新数据库数量
bool Customer::changeChartItemCount(QString id, int c)
{
    QSqlQuery query;
    QString sql = QString("UPDATE Chart SET GoodsNum = %1 ").arg(c)
            +QString("WHERE UserName = '%1' AND GoodsID = '%2'").arg(getUserName()).arg(id);
    if(!query.exec(sql))
    {
        qDebug()<<"false";
        return false;
    }
    return true;
}

//删除数据库中商品
bool Customer::deleteChartItem(QString id)
{
    QSqlQuery query;
    QString sql = QString("DELETE FROM Chart WHERE GoodsID = '%1'").arg(id);
    if(!query.exec(sql))
    {
        qDebug()<<"false";
        return false;
    }
    return true;
}

