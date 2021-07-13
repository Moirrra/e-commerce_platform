#include "order.h"
#include "ui_order.h"
#include "global.h"
#include <QMessageBox>
#include <QTimer>
#include <QDateTime>
#include <QDebug>
#include <QMessageBox>
#include <sqlhelp.h>
#include <QSqlQuery>
#include "filehelp.h"
Order::Order(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Order)
{
    ui->setupUi(this);
    mSqlhelp = new SqlHelp();
    this->setWindowTitle("我的订单");
    cust = new Customer();
    cust->setUserName(global::userName);

    sum = 0;
    //显示商品信息
    pLayout = new QVBoxLayout(this);
    itemList.clear();
    if(!cust->updateChartInfo())
    {
        QMessageBox::critical(this,"错误","读取数据库失败！");
    }
    tTimer = new QTimer(this);
    timeRecord = new QTime(0,0,30,0);
    ui->label_timer->setText(timeRecord->toString("mm:ss"));
    connect(tTimer,&QTimer::timeout,[=](){
        *timeRecord = timeRecord->addSecs(-1);
        ui->label_timer->setText(timeRecord->toString("mm:ss"));
        //超时
        if(*timeRecord==QTime(0,0,0,0))
        {
            tTimer->stop();
            QMessageBox::critical(this,"支付超时","请重新生成订单");
            orderDelete();
        }
    });
}

Order::~Order()
{
    delete ui;

}

void Order::InitOrder(QList<int> list)
{
    itemList.clear();
    sum = 0;
    QList<Goods> goodslist = cust->getChartList();
    for(int i = 0; i < list.size(); i++)
    {
        OrderItem* item = new OrderItem(this);
        item->InitItem(goodslist[list[i]]);
        itemList.append(item);
        sum += item->total;
    }
    for(int i = 0; i < itemList.size(); i++)
    {
        pLayout->addWidget(itemList[i]);
    }
    if(!itemList.isEmpty())
    {
        ui->scrollAreaWidgetContents->setLayout(pLayout);
        pLayout->setContentsMargins(0, 0, 0, 0);
        ui->label_sum->setText(QString::number(sum,'f',2));
    }
    tTimer->start(1000);
    orderID = createID();
    //添加到订单表
    for(int i = 0; i < itemList.size(); i++)
    {
        QString sql = QString("INSERT INTO Orders VALUES ('%1', '%2','%3','%4', '%5', %6, %7, %8, '%9',0, '', '')")
                .arg(orderID).arg(global::userName).arg(itemList[i]->goods.getID())
                .arg(itemList[i]->goods.getName()).arg(itemList[i]->goods.getSize()).arg(itemList[i]->goods.getPrice())
                .arg(itemList[i]->goods.getCount()).arg(itemList[i]->total).arg(itemList[i]->goods.getSeller());
        qDebug()<<sql;
        if(!mSqlhelp->SqlExec(sql))
        {
            QMessageBox::critical(this,"错误","写入数据库失败!");
            qDebug()<<"添加到订单表失败";
            this->close();
            return;
        }

    }
    //冻结订单中的商品(修改数量)
    for(int i = 0; i < itemList.size(); i++)
    {
        int count = itemList[i]->goods.getCount();
        QString gID = itemList[i]->goods.getID();
        QString sql = QString("UPDATE Goods SET GoodsNum = GoodsNum - %1, GoodsSum=GoodsSum-%1*GoodsUnitPrice WHERE GoodsId = '%2'").arg(count).arg(gID);
        if(!mSqlhelp->SqlExec(sql))
        {
            QMessageBox::critical(this,"错误","写入数据库失败!");
            qDebug()<<"冻结商品失败！";
        }

    }
}

//收货地址
void Order::on_lineEdit_editingFinished()
{
    address = ui->lineEdit->text();
}

QString Order::createID()
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
    while(mSqlhelp->FindValue("OrderID","Orders",id))
    {
        int _n = qrand() % 10000;
        QString _id2 = QString("%1").arg(_n,4,10,QLatin1Char('0'));
        id = id1 + _id2;  //更新id
    }
    return id;
}

void Order::orderDelete()
{

    //解除冻结
    for(int i = 0; i < itemList.size(); i++)
    {
        int count = itemList[i]->goods.getCount();
        QString gID = itemList[i]->goods.getID();
        QString sql = QString("UPDATE Goods SET GoodsNum=GoodsNum+%1, GoodsSum=GoodsSum+%1*GoodsUnitPrice WHERE GoodsId = '%2'").arg(count).arg(gID);
        if(!mSqlhelp->SqlExec(sql))
        {
            QMessageBox::critical(this,"错误","写入数据库失败!");
            qDebug()<<"解除冻结商品失败！";
        }
    }
    //删去未支付订单
    QString where = QString("WHERE OrderID = '%1'").arg(orderID);
    if(!mSqlhelp->DeleteWhere("Orders",where))
    {
        QMessageBox::critical(this,"错误","写入数据库失败!");
        qDebug()<<"删除未支付商品失败！";
    }
    this->close();
}

//支付
void Order::on_pushButton_clicked()
{
    FileHelp* fileHelp = new FileHelp();
    QString val;
    if(!fileHelp->findValue("customer.txt",global::userName,val,2))
    {
        QMessageBox::critical(this,"支付失败","找不到用户金额！");
        orderDelete();
        return;
    }
    cust->setBalance(val.toDouble());
    if(ui->lineEdit->text().isEmpty())
    {
        QMessageBox::critical(this,"支付失败","您未填写收货地址！");
        return;
    }
    if(cust->getBalance()<sum)
    {
        QString str = QString("您的账户余额不足!当前余额：%1").arg(cust->getBalance());
        QMessageBox::critical(this,"支付失败",str);
        orderDelete();
        return;
    }

    /*支付成功*/
    tTimer->stop();
    //更改数据库状态
    tradeTime = QDateTime::currentDateTime().toString("yyyy/MM/dd hh:mm:ss");
    QString sql = QString("UPDATE Orders SET OrderState = 1, TradeTime = '%1', Address = '%2' WHERE OrderID = '%3'")
            .arg(tradeTime).arg(address).arg(orderID);
    if(!mSqlhelp->SqlExec(sql))
    {
        qDebug()<<"更新订单表失败！";
        orderDelete();//删除订单，解除冻结
        QMessageBox::critical(this,"支付失败","写入数据库失败！");
        return;
    }
    //删除已售空商品
    QString where = "WHERE GoodsNum = 0";
    if(!mSqlhelp->DeleteWhere("Goods",where))
    {
        qDebug()<<"删除已售空商品失败！";
        return;
    }
    //修改账户金额
    //消费者
    double newMoney1 = cust->getBalance() - sum;

    if(!fileHelp->writeFile("customer.txt",cust->getUserName(),QString::number(newMoney1,'f',2),2))
    {
        qDebug()<<"修改用户余额失败！";
        return;
    }
    //商家
    for(int i = 0; i < itemList.size(); i++)
    {
        QString seller = itemList[i]->goods.getSeller();
        qDebug()<<seller;
        QString val = "";
        if(!fileHelp->findValue("merchant.txt",seller,val,2))
        {
            qDebug()<<"获取商家余额失败！";
            return;
        }
        double newMoney2 = val.toDouble() + itemList[i]->total;
        if(!fileHelp->writeFile("merchant.txt",seller,QString::number(newMoney2,'f',2),2))
        {
            qDebug()<<"修改商家余额失败！";
            return;
        }
    }
    QMessageBox::information(this,"提示","成功支付！");
    this->close();
}
