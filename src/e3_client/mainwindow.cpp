#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "login.h"
#include "goodsshow.h"
#include "chart.h"
#include "chartitem.h"
#include <synchapi.h>
#include "order.h"
#include "ordershow.h"
#define ip "127.0.0.1"
#define port 8888

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("首页");
    cust = new Customer();
    merch = new Merchant();
    loginPage = new Login();
    showPage = new GoodsShow();
    myChart = new Chart();
    userType = 0;
    ui->btn_logout->setVisible(false);
    /*搜索框*/
    searchLine = new AppSearchLine(ui->widget_search);
    searchLine->setFixedWidth(480);
    connect(searchLine->searchBtn,&QPushButton::clicked,[=](){
        sendMsg("s#"+searchLine->text());
    });

    /*连接*/
    tcpSocket = new QTcpSocket(this);
    connect(tcpSocket, &QTcpSocket::connected, [=](){
        ui->label_connect->setText("已连接");
        sendMsg("g#");
    });
    connect(tcpSocket, &QTcpSocket::disconnected, [=](){
        ui->label_connect->setText("未连接");
    });
    connect(tcpSocket, &QTcpSocket::readyRead,this,&MainWindow::readMsg);

    //登陆注册
    connect(loginPage,&Login::sendMsg,[=](QString ins){
        sendMsg(ins);
    });
    //商品展示
    connect(this,&MainWindow::getGoods,this,&MainWindow::goodsShow);
    //加入购物车
    connect(showPage,&GoodsShow::addChart,this,&MainWindow::addToChart);
    //删除购物车
    connect(myChart,&Chart::deleteItems,this,&MainWindow::deleteChart);
    //生成订单
    connect(myChart,&Chart::createOrder,this,&MainWindow::createOrders);
    //取消订单
    connect(myChart->orderPage,&Order::cancelOrder,this,&MainWindow::cancelOrders);
    //支付成功
    connect(myChart->orderPage,&Order::finishTrade,this,&MainWindow::finishOrders);

}

MainWindow::~MainWindow()
{
    delete ui;
    delete myChart;
    delete loginPage;
    delete showPage;
}

/*
 * @brief 发送消息给服务器
 * @param ins 指令
 */
void MainWindow::sendMsg(const QString ins)
{
    tcpSocket->write(ins.toUtf8());
    qDebug()<<"发出消息："<<ins;
}

/*
 * @brief 设置商品展示列表
 * @param buffer 指令缓存
 */
void MainWindow::setGoodsList(const QString &buffer)
{
    goodsList.clear();
    ui->listWidget->clear();
    QStringList linesplit = buffer.split('#');
    for(int i = 1; i < linesplit.size(); i++)
    {
        goodsList.append(linesplit[i]);
        QListWidgetItem* item = new QListWidgetItem(linesplit[i]);
        ui->listWidget->addItem(item);
    }
    searchLine->initCompleter(goodsList);
}

/*
 * @brief 处理登录反馈
 * @param buffer 指令缓存
 */
void MainWindow::recvLogin(const QString &buffer)
{
    QStringList linesplit = buffer.split('#');
    if(linesplit[1]=='1')
    {
        userType = linesplit[2].toInt();
        if(userType==1)
        {
            cust->setUserName(linesplit[3]);
            cust->setBalance(linesplit[4].toDouble());
            ui->label_type->setText("消费者");
            ui->label_name->setText(cust->getUserName());
            ui->label_money->setText(QString::number(cust->getBalance(),'f',2));
        }
        else
        {
            merch->setUserName(linesplit[3]);
            merch->setBalance(linesplit[4].toDouble());
            ui->label_type->setText("商家");
            ui->label_name->setText(merch->getUserName());
            ui->label_money->setText(QString::number(merch->getBalance(),'f',2));
        }
        QMessageBox::information(loginPage,"提示","登录成功!");
        ui->btn_login->setVisible(false);
        ui->btn_logout->setVisible(true);
        loginPage->hide();
    }
    else
    {
        QMessageBox::critical(loginPage,"登录失败","用户名或密码错误！");
    }
}

/*
 * @brief 处理注册反馈
 * @param buffer 指令缓存
 */
void MainWindow::recvReg(const QString &buffer)
{
    QStringList linesplit = buffer.split('#');
    if(linesplit[1]=='1')
    {
        if(linesplit[2]=='1')
        {
            cust->setUserName(linesplit[3]);
            cust->setBalance(linesplit[4].toDouble());
            ui->label_type->setText("消费者");
            ui->label_name->setText(cust->getUserName());
            ui->label_money->setText(QString::number(cust->getBalance(),'f',2));
        }
        else
        {
            merch->setUserName(linesplit[3]);
            merch->setBalance(linesplit[4].toDouble());
            ui->label_type->setText("商家");
            ui->label_name->setText(merch->getUserName());
            ui->label_money->setText(QString::number(merch->getBalance(),'f',2));
        }
        QMessageBox::information(loginPage,"提示","注册成功!");
    }
    else
    {
        if(linesplit[2]=='0')
        {
             QMessageBox::critical(loginPage,"注册失败","该用户名已存在！");
        }
        else
             QMessageBox::critical(loginPage,"注册失败","读写文件错误！");
    }
}

/*
 * @brief 处理商品信息反馈
 * @param buffer 指令缓存
 */
void MainWindow::recvGoods(const QString &buffer)
{
    QStringList linesplit = buffer.split('#');
    if(linesplit.size()==7)
    {
        tempGoods.setName(linesplit[1]);
        tempGoods.setSize(linesplit[2]);
        tempGoods.setPrice(linesplit[3].toDouble());
        tempGoods.setCount(linesplit[4].toInt());
        tempGoods.setSeller(linesplit[5]);
        tempGoods.setDetails(linesplit[6]);
        emit getGoods();
    }
    else
    {
        tempGoods.setName("unknown");
        QMessageBox::critical(this,"错误","该商品不存在！");
    }
}

/*
 * @brief 处理添加购物车反馈
 * @param buffer 指令缓存
 */
void MainWindow::recvAddChart(const QString &buffer)
{
    QStringList linesplit = buffer.split('#');
    if(linesplit[1]=="1")
    {
        QMessageBox::information(showPage,"提示","成功添加至购物车！");
        return;
    }
    if(linesplit[1]=="0")
    {
        QMessageBox::critical(showPage,"错误","添加至购物车失败！");
    }
    
}


/*
 * @brief 处理购物车反馈
 * @param buffer 指令缓存
 */
void MainWindow::recvChart(const QString &buffer)
{
    QStringList linesplit = buffer.split('#');
    if(linesplit[1]=="0")
    {
        QMessageBox::critical(this,"错误","获取购物车信息失败！");
        return;
    }
    if(linesplit.size()>=2)
    {
        myChart->nList.clear();
        myChart->cList.clear();
        myChart->pList.clear();
        myChart->sList.clear();
        int size = linesplit[1].toInt();//个数
        for(int i = 3; i < 3+size; i++)
        {
            myChart->nList.append(linesplit[i]);
        }
        for(int i = 4+size; i < 4+2*size; i++)
        {
            myChart->cList.append(linesplit[i].toInt());
        }
        for(int i = 5+2*size; i < 5+3*size; i++)
        {
            myChart->pList.append(linesplit[i].toDouble());
        }
        for(int i = 6+3*size; i < 6+4*size; i++)
        {
            myChart->sList.append(linesplit[i].toInt());
        }
        myChart->cust->setUserName(cust->getUserName());
        myChart->cust->setBalance(cust->getBalance());
        myChart->InitChart();
        myChart->show();
    }
}

/*
 * @brief 处理删除购物车商品反馈
 * @param buffer 指令缓存
 */
void MainWindow::recvDeleteChart(const QString &buffer)
{
    QStringList linesplit = buffer.split('#');
    if(linesplit[1]=="0")
    {
        QMessageBox::critical(myChart,"错误","删除购物车商品失败！");
        return;
    }
    else if(linesplit[1]=="1")
    {
        QMessageBox::information(myChart,"提示","成功删除购物车商品！");
    }
}

/*
 * @brief 处理订单反馈
 * @param buffer 指令缓存
 */
void MainWindow::recvOrder(const QString &buffer)
{
    QStringList linesplit = buffer.split('#');
    if(linesplit[1]=="0")
    {
        QMessageBox::critical(myChart,"错误","生成订单失败！");
        return;
    }
    else
    {
        tempID = linesplit[1];
        QMessageBox::information(myChart,"提示","成功生成订单！");
        myChart->showOrder();
    }

}

/*
 * @brief 处理取消订单反馈
 * @param buffer 指令缓存
 */
void MainWindow::recvCancel(const QString &buffer)
{
    QStringList linesplit = buffer.split('#');
    if(linesplit[1]=="0")
    {
        QMessageBox::critical(myChart,"错误","取消订单失败！");
        return;
    }
    else
    {
        QMessageBox::information(myChart,"提示","取消订单成功！");
    }
}

/*
 * @brief 处理支付反馈
 * @param buffer 指令缓存
 */
void MainWindow::recvFinish(const QString &buffer)
{
    QStringList linesplit = buffer.split('#');
    if(linesplit[1]=="0")
    {
        QMessageBox::critical(this,"错误","交易失败！");
        return;
    }
    else
    {
        QMessageBox::information(this,"提示","交易成功！");
    }
}

/*
 * @brief 处理订单展示反馈
 * @param buffer 指令缓存
 */
void MainWindow::showOrder(const QString &buffer)
{
    QStringList linesplit = buffer.split('#');
    int size = linesplit[1].toInt();
    OrderShow *oPage = new OrderShow();
    oPage->rowCount = size;
    for(int i = 2; i < 2+size; i++)
    {
        oPage->nList.append(linesplit[i]);
    }
    for(int i = 2+size; i < 2+2*size; i++)
    {
        oPage->cList.append(linesplit[i].toInt());
    }
    for(int i = 2+2*size; i < 2+3*size; i++)
    {
        oPage->pList.append(linesplit[i].toDouble());
    }
    for(int i = 2+3*size; i < 2+4*size; i++)
    {
        oPage->stList.append(linesplit[i].toInt());
    }
    for(int i = 2+4*size; i < 2+5*size; i++)
    {
        oPage->tList.append(linesplit[i]);
    }
    oPage->InitData();
    oPage->show();

}

/*
 * @brief 服务器消息解析
 */
void MainWindow::readMsg()
{
    QString buffer;
    buffer = tcpSocket->readAll();
    qDebug()<<"收到服务器消息:"<<buffer;
    //登陆
    if(buffer[0]=='l')
    {
        recvLogin(buffer);
        return;
    }
    //注册
    if(buffer[0]=='r')
    {
        recvReg(buffer);
        return;
    }
    //获得商品列表
    if(buffer[0]=='g')
    {
        setGoodsList(buffer);
        return;
    }
    //查询商品信息
    if(buffer[0]=='s')
    {
        recvGoods(buffer);
        return;
    }
    //添加购物车
    if(buffer[0]=='a')
    {
        recvAddChart(buffer);
        return;
    }
    //展示购物车
    if(buffer[0]=='c')
    {
        recvChart(buffer);
        return;
    }
    //删除购物车商品
    if(buffer[0]=='d')
    {
        recvDeleteChart(buffer);
        return;
    }
    //生成订单
    if(buffer[0]=='o')
    {
        recvOrder(buffer);
        return;
    }
    //取消订单
    if(buffer[0]=='z')
    {
        recvCancel(buffer);
        return;
    }
    //交易成功
    if(buffer[0]=='f')
    {
        recvFinish(buffer);
        return;
    }
    //展示订单
    if(buffer[0]=='t')
    {
        showOrder(buffer);
        return;
    }
}

/*发送消息给服务器*/
//商品展示页
void MainWindow::goodsShow()
{
    showPage->curGoods = tempGoods;
    showPage->userType = userType;
    showPage->InitData();
    showPage->show();
}

//发送添加购物车信息
void MainWindow::addToChart(QString name, int num)
{
    sendMsg("a#"+cust->getUserName()+"#"+name+"#"+QString::number(num));
}

//发送删除购物车商品信息
void MainWindow::deleteChart(QList<QString> *dList)
{
    QString str = "d#"+cust->getUserName();
    for(int i = 0; i < dList->size(); i++)
    {
        str+="#";
        str+=dList->at(i);
    }
    sendMsg(str);
}

//发送创建订单信息
void MainWindow::createOrders(QList<QString> *nList, QList<int> *cList)
{
    qDebug()<<"create orders";
    QString str = "o#"+cust->getUserName()+"#";
    str += QString::number(nList->size());
    QString str1 = "#n",str2="#c";
    for(int i = 0; i < nList->size(); i++)
    {
        str1 += "#";
        str1 += nList->at(i);
    }
    for(int i = 0; i < cList->size(); i++)
    {
        str2 += "#";
        str2 += QString::number(cList->at(i));
    }
    sendMsg(str+str1+str2);
}

//发送取消订单消息
void MainWindow::cancelOrders()
{
    sendMsg("z#"+tempID);
}

//发送支付完成消息
void MainWindow::finishOrders(QString addr,double nMoney)
{
    sendMsg("f#"+tempID+"#"+addr);
    cust->setBalance(nMoney);
    ui->label_money->setText(QString::number(nMoney));
}

//点击登录
void MainWindow::on_btn_login_clicked()
{
    loginPage->show();
}

//点击连接
void MainWindow::on_btn_connect_clicked()
{
    tcpSocket->abort();
    tcpSocket->connectToHost(ip,port);
}

//点击登出
void MainWindow::on_btn_logout_clicked()
{
    ui->label_name->setText("unknown");
    ui->label_type->setText("unknown");
    ui->label_money->setText("unknown");
    if(userType==1)
    {
        cust->setUserName("");
        cust->setUserPwd("");
        cust->setBalance(0);
    }
    else if(userType==2)
    {
        merch->setUserName("");
        merch->setUserPwd("");
        merch->setBalance(0);
    }
    userType = 0;
    ui->btn_login->setVisible(true);
    ui->btn_logout->setVisible(false);
}

//点击商品列表
void MainWindow::on_listWidget_itemClicked(QListWidgetItem *item)
{
    QString gName = item->text();
    QString str = "s#"+gName;
    sendMsg(str);
    item->setSelected(false);
}

//点击购物车
void MainWindow::on_btn_chart_clicked()
{
    if(userType==0)
    {
        QMessageBox::critical(this,"错误","您未登录！");
        return;
    }
    if(userType!=1)
    {
        QMessageBox::critical(this,"错误","您不是消费者！");
        return;
    }
    sendMsg("c#"+cust->getUserName());

}

//点击订单
void MainWindow::on_btn_order_clicked()
{
    if(userType==0)
    {
        QMessageBox::critical(this,"错误","您未登录！");
        return;
    }
    if(userType!=1)
    {
        QMessageBox::critical(this,"错误","您不是消费者！");
        return;
    }
    sendMsg("t#"+cust->getUserName());
}
