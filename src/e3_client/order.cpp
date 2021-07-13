#include "order.h"
#include "ui_order.h"
#include <QMessageBox>
#include <QDateTime>
Order::Order(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Order)
{
    ui->setupUi(this);
    this->setWindowTitle("我的订单");
    tTimer = new QTimer(this);
    timeRecord = new QTime(0,0,20,0);
    connect(tTimer,&QTimer::timeout,[=](){
        *timeRecord = timeRecord->addSecs(-1);
        ui->label_timer->setText(timeRecord->toString("mm:ss"));
        //超时
        if(*timeRecord==QTime(0,0,0,0))
        {
            tTimer->stop();
            delete timeRecord;
            QMessageBox::critical(this,"支付超时","请重新生成订单");
            emit cancelOrder();
            this->close();
        }
    });
}

Order::~Order()
{
    delete ui;
}

//初始化订单信息
void Order::InitOrder()
{
    timeRecord = new QTime(0,0,20,0);
    itemList.clear();
    sum = 0;
    pLayout = new QVBoxLayout();
    for(int i = 0; i < nList.size(); i++)
    {
        OrderItem* item = new OrderItem();
        item->InitItem(nList[i],cList[i],pList[i],sList[i]);
        itemList.append(item);
        sum += item->total;
    }
    ui->label_sum->setText(QString::number(sum,'f',2));
    for(int i = 0; i < itemList.size(); i++)
    {
        pLayout->addWidget(itemList[i]);
    }
    if(!itemList.isEmpty())
    {
        ui->scrollAreaWidgetContents->setLayout(pLayout);
        pLayout->setContentsMargins(0, 0, 0, 0);
    }
    tTimer->start(1000);
}


//点击支付
void Order::on_btn_trade_clicked()
{
    if(ui->lineEdit->text().isEmpty())
    {
        QMessageBox::critical(this,"支付失败","您未填写收货地址！");
        return;
    }
    if(cust->getBalance()<sum)
    {
        QString str = QString("您的账户余额不足!当前余额：%1").arg(cust->getBalance());
        QMessageBox::critical(this,"支付失败",str);
        emit cancelOrder();
        this->close();
        return;
    }

    /*支付成功*/
    tTimer->stop();
    double nMoney = cust->getBalance() - sum;
    cust->setBalance(nMoney);
    emit finishTrade(ui->lineEdit->text(),nMoney);
    this->close();
}
