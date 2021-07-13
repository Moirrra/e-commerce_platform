#include "sqlhelp.h"
#include "QDebug"
#include <QTime>

SqlHelp::SqlHelp()
{

}

SqlHelp::~SqlHelp()
{

}

//打开数据库
void SqlHelp::open()
{
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("e_goods.db");
    if (!db.open())
    {
        qDebug() << "Error: Failed to connect database." << db.lastError();
        return;
    }
    qDebug()<<"Successfully concect to database.";
}

//关闭数据库
void SqlHelp::close()
{
    db.close();
}

/*
 * @brief 获取商品名称列表
 * @param list 存放结果指针
 * @retval true成功 false失败
 */
bool SqlHelp::getGoodsList(QStringList *list)
{
    bool flag = false;
    QSqlQuery query;
    QString sql = "select * from Goods";

    if(query.exec(sql))
    {
        qDebug()<<sql;
        while(query.next())
        {
            list->append(query.value(1).toString());
            flag = true;
        }
    }
    return flag;
}

/*
 * @brief 获取商品信息
 * @param list 存放结果指针
 * @param name 商品名称
 * @retval true成功 false失败
 */
bool SqlHelp::getGoodsInfo(QStringList *list, const QString &name)
{
    bool flag = false;
    QSqlQuery query;
    QString sql = QString("select * from Goods where GoodsName = '%1'").arg(name);
    if(query.exec(sql))
    {
        qDebug()<<sql;
        while(query.next())
        {
            list->append(query.value(3).toString());
            list->append(query.value(4).toString());
            list->append(query.value(5).toString());
            list->append(query.value(8).toString());
            list->append(query.value(11).toString());
            flag = true;
        }
    }
    return flag;
}

/*
 * @brief 添加商品至购物车
 * @param uName 用户名
 * @param uName 商品名
 * @param num 商品数量
 * @retval true成功 false失败
 */
bool SqlHelp::addToChart(const QString &uName, const QString &gName, const int num)
{
    QSqlQuery query;
    int flag = 0;
    QString sql = QString("select GoodsName from Chart where UserName = '%1'").arg(uName);
    if(query.exec(sql))
    {
        while(query.next())
        {
            if(gName==query.value(0).toString())
            {
                flag = 1;
                break;
            }
        }
    }
    if(flag)
    {//do nothing
        return true;
    }
    else
    {//新增
        QString sql2 = QString("insert into Chart values ('%1','%2',%3)").arg(uName).arg(gName).arg(num);
        if(query.exec(sql2))
        {
            return true;
        }
    }
    return false;
}

/*
 * @brief 获取购物车信息
 * @param uName 用户名
 * @param nList 商品名称表
 * @param cList 商品数量表
 * @param pList 商品价格表
 * @param sList 商品库存表
 * @retval true成功 false失败
 */
bool SqlHelp::showChart(const QString &uName, QList<QString> *nList, QList<int> *cList,
                        QList<double> *pList, QList<int> *sList)
{
    QSqlQuery query;
    QString sql = QString("select * from Chart where UserName = '%1'").arg(uName);
    if(query.exec(sql))
    {
        while(query.next())
        {
            nList->append(query.value(1).toString());
            cList->append(query.value(2).toInt());
        }
    }
    else return false;
    for(int i = 0; i < nList->size(); i++)
    {
        QString name = nList->at(i);
        QString sql2 = QString("select * from Goods where GoodsName = '%1'").arg(name);
        if(query.exec(sql2))
        {
            double p;
            int s;
            while(query.next())
            {
                p = query.value(4).toDouble();
                s = query.value(5).toInt();
            }
            pList->append(p);
            sList->append(s);
        }
        else return false;
    }
    return true;
}


/*
 * @brief 删除购物车
 * @param uName 用户名
 * @param gList 商品名称表
 * @retval true成功 false失败
 */
bool SqlHelp::deleteChart(const QString &uName, QStringList *gList)
{
    QSqlQuery query;
    for(int i = 0; i < gList->size(); i++)
    {
        QString sql = QString("delete from Chart where UserName = '%1' and GoodsName = '%2'")
                .arg(uName).arg(gList->at(i));
        if(!query.exec(sql))
        {
            return false;
        }
    }
    return true;
}

/*
 * @brief 新增订单信息
 * @param uName 用户名
 * @param nList 商品名称表
 * @param cList 商品数量表
 * @retval true成功 false失败
 */
bool SqlHelp::createOrder(const QString &uName, QList<QString> *nList, QList<int> *cList)
{
    QSqlQuery query;
    //get other
    QList<double> pList;
    QList<QString> sellerList;
    tempID = createOrderID();
    for(int i = 0; i < nList->size(); i++)
    {
        QString sql1 = QString("select * from Goods where GoodsName = '%1'").arg(nList->at(i));
        if(query.exec(sql1))
        {
            while(query.next())
            {
                pList.append(query.value(4).toDouble());
                sellerList.append(query.value(8).toString());
            }
            qDebug()<<"createOrder:select other!";
        }
        else
        {
            return false;
            qDebug()<<"createOrder:select error";
        }
    }
    //insert Order
    for(int i = 0; i < nList->size(); i++)
    {
        double sum = pList[i] * cList->at(i);
        QString sql2 = QString("insert into Orders values ('%1', '%2','%3', %4, %5, %6, '%7',0,'','')")
                .arg(tempID).arg(uName).arg(nList->at(i)).arg(pList[i]).arg(cList->at(i)).arg(sum).arg(sellerList[i]);
        if(!query.exec(sql2))
        {
            return false;
            qDebug()<<"createOrder:insert error";
        }
    }
    qDebug()<<"createOrder:insert!";
    //冻结商品
    for(int i = 0; i < nList->size(); i++)
    {
        QString sql3= QString("update Goods set GoodsNum=GoodsNum-%1,GoodsSum=GoodsSum-%1*GoodsUnitPrice where GoodsName = '%2'")
                .arg(cList->at(i)).arg(nList->at(i));
        if(!query.exec(sql3))
        {
            return false;
            qDebug()<<"createOrder:update Goods error";
        }
    }
    qDebug()<<"createOrder:update!";
    return true;
}

/*
 * @brief 取消订单
 * @param ID 订单编号
 * @retval true成功 false失败
 */
bool SqlHelp::cancelOrder(const QString &ID)
{
    QSqlQuery query;
    //get nlist clist
    QList<QString> nList;
    QList<int> cList;
    QString sql1 = QString("select * from Orders where OrderID='%1'").arg(ID);
    if(query.exec(sql1))
    {
        while(query.next())
        {
            nList.append(query.value(2).toString());
            cList.append(query.value(4).toInt());
        }
        qDebug()<<"cancelOrder:select!";
    }
    else
    {
        qDebug()<<"cancelOrder:select error!";
        return false;
    }

    //delete order
    QString sql2 = QString("delete from Orders where OrderID = '%1'").arg(ID);
    if(query.exec(sql2))
    {
        qDebug()<<"cancelOrder:delete!";
    }
    else
    {
        qDebug()<<"cancelOrder:delete error!";
        return false;
    }
    //update
    for(int i = 0; i < nList.size(); i++)
    {
        QString sql3 = QString("update Goods set GoodsNum=GoodsNum+%1, GoodsSum=GoodsSum+%1*GoodsUnitPrice where GoodsName = '%2'")
                .arg(cList[i]).arg(nList[i]);
        if(!query.exec(sql3))
        {
            qDebug()<<"cancelOrder:update error!";
            return false;
        }
        else
        {
            qDebug()<<"cancelOrder:update";
        }
    }
    return true;
}

/*
 * @brief 支付完善订单信息
 * @param ID 订单编号
 * @param addr 收货地址
 * @param uName 用户名
 * @param uTotal 总金额
 * @param sList 商家表
 * param tList 商家收益表
 * @retval true成功 false失败
 */
bool SqlHelp::finishOrder(const QString& ID, const QString& addr,QString &uName, double &uTotal,
                          QList<QString> *sList, QList<double> *tList)
{
    QSqlQuery query;
    //更新订单
    QString tradeTime = QDateTime::currentDateTime().toString("yyyy/MM/dd hh:mm:ss");
    QString sql1 = QString("update Orders set OrderState=1,TradeTime='%1',Address='%2' where OrderID = '%3'")
            .arg(tradeTime).arg(addr).arg(ID);
    if(!query.exec(sql1))
    {
        qDebug()<<"finishOrder:update error!";
        return false;
    }
    //获得sList tList
    uTotal = 0;
    QString sql2 = QString("select * from Orders where OrderID = '%1'").arg(ID);
    if(query.exec(sql2))
    {
        while(query.next())
        {
            uName = query.value(1).toString();
            sList->append(query.value(6).toString());
            tList->append(query.value(5).toDouble());
            uTotal += query.value(5).toDouble();
        }
        qDebug()<<"finishOrder:select!";
        return true;
    }
    return false;
}

/*
 * @brief 获取订单信息
 * @param uName 用户名
 * @param nList 商品名称表
 * @param cList 商品数量表
 * @param pList 商品价格表
 * @param stList 商品状态表
 * @param tList 订单创建时间表
 * @retval true成功 false失败
 */
bool SqlHelp::showOrder(const QString &uName, QList<QString> *nList, QList<int> *cList, QList<double> *pList,
                        QList<int> *stList, QList<QString> *tList)
{
    QSqlQuery query;
    QString sql = QString("select * from Orders where UserName='%1'").arg(uName);
    if(query.exec(sql))
    {
        while(query.next())
        {
            nList->append(query.value(2).toString());
            cList->append(query.value(3).toInt());
            pList->append(query.value(4).toDouble());
            stList->append(query.value(7).toInt());
            tList->append(query.value(8).toString());
        }
        return true;
    }
    return false;
}

/*
 * @brief
 * @param columnName 列名
 * @param tableName 表名
 * @param value 查找值
 * @retval true 查找成功
 * @retval false 查找失败
 */
bool SqlHelp::FindValue(const QString &columnName,const QString &tableName,const QString &value)
{
    QSqlQuery query;
    QString sql = QString("SELECT %1 FROM %2").arg(tableName).arg(columnName);
    while(query.exec())
    {
        if(query.value(0).toString()==value)
            return true;
    }
    return false;
}

//创建订单编号
QString SqlHelp::createOrderID()
{
    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));//设置随机数种子
    QDate date(QDate::currentDate());
    int year = date.year();
    int month = date.month();
    int day = date.day();
    QString id1 = QString::number(year) + QString("%1").arg(month,2,10,QLatin1Char('0')) + QString("%1").arg(day,2,10,QLatin1Char('0'));
    int n = qrand() % 10000;  //产生10000以内的随机数
    QString id2 = QString("%1").arg(n,4,10,QLatin1Char('0'));
    QString id = id1 + id2;

    //查重
    while(FindValue("OrderID","Orders",id))
    {
        int _n = qrand() % 10000;
        QString _id2 = QString("%1").arg(_n,4,10,QLatin1Char('0'));
        id = id1 + _id2;  //更新id
    }
    return id;
}

