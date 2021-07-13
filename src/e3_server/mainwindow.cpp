#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <synchapi.h>
#define port 8888
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    sqlHelp = new SqlHelp();
    sqlHelp->open();
    fileHelp = new FileHelp();

    myServer = new QTcpServer(this);
    connect(myServer, &QTcpServer::newConnection, this, &MainWindow::newConnect);
    myServer->listen(QHostAddress::Any, port);
}

MainWindow::~MainWindow()
{
    delete ui;
    sqlHelp->close();
}

/*
 * @brief 发送信息给客户端
 * @param ins 指令
 */
void MainWindow::sendMsg(const QString &ins)
{
    ui->textBrowser->append("服务器:"+ins);
    mySocket->write(ins.toUtf8());
}

/*
 * @brief 处理登录请求
 * @param buffer 指令缓存
 */
void MainWindow::loginMsg(const QString &buffer)
{
    int ret;
    QStringList linesplit=buffer.split("#");
    QString type = linesplit[1];
    QString name = linesplit[2];
    QString psw = linesplit[3];
    QString money;
    if(type=="1")
        ret = fileHelp->findLogin("customer.txt",name,psw,money);
    else ret = fileHelp->findLogin("merchant.txt",name,psw,money);
    switch(ret)
    {
    case -1://密码错误
    case 0://用户名不存在
        sendMsg("l#0");
        break;
    case 1://登陆成功
        QString str = "l#1#"+type+"#"+name+"#"+money;
        sendMsg(str);
        break;
    }
}

/*
 * @brief 处理注册请求
 * @param buffer 指令缓存
 */
void MainWindow::regMsg(const QString &buffer)
{
    int ret;
    QStringList linesplit=buffer.split('#');
    QString type = linesplit[1];
    QString name = linesplit[2];
    QString psw = linesplit[3];
    if(type=="1")
        ret = fileHelp->findReg("customer.txt",name,psw);
    else ret = fileHelp->findReg("merchant.txt",name,psw);
    switch(ret)
    {
    case -1://读写文件错误
        sendMsg("r#0#1");
        break;
    case 0://重名
        sendMsg("r#0#0");
        break;
    case 1://注册成功
        QString str = "r#1#"+type+"#"+name+"#0";
        sendMsg(str);
        break;
    }
}

/*
 * @brief 处理获取商品列表请求
 * @param buffer 指令缓存
 */
void MainWindow::getGLMsg()
{
    QStringList goodsList;
    if(!sqlHelp->getGoodsList(&goodsList))
    {
        sendMsg("g#0");
    }
    else
    {
        QString str = "g#";
        int size = goodsList.size();
        for(int i = 0; i < size; i++)
        {
            str += goodsList[i];
            if(i!=size-1) str += "#";
        }
        sendMsg(str);
    }
}

/*
 * @brief 处理商品展示请求
 * @param buffer 指令缓存
 */
void MainWindow::goodsMsg(const QString &buffer)
{
    QStringList linesplit = buffer.split('#');
    QString gName = linesplit[1];
    QStringList list;
    if(!sqlHelp->getGoodsInfo(&list,gName))
    {
        sendMsg("s#0");
        return;
    }
    else
    {
        QString str = "s#"+gName;
        for(int i = 0; i < list.size(); i++)
        {
           str += "#";
           str += list[i];
        }
        sendMsg(str);
    }
}

/*
 * @brief 处理添加购物车请求
 * @param buffer 指令缓存
 */
void MainWindow::addChart(const QString &buffer)
{
    QStringList linesplit = buffer.split('#');
    QString uName = linesplit[1];
    QString gName = linesplit[2];
    int num = linesplit[3].toInt();
    if(!sqlHelp->addToChart(uName,gName,num))
    {
        sendMsg("a#0");
    }
    else
    {
        sendMsg("a#1");
    }
}

/*
 * @brief 处理展示购物车请求
 * @param buffer 指令缓存
 */
void MainWindow::showChart(const QString &buffer)
{
    QStringList linesplit = buffer.split('#');
    QString uName = linesplit[1];
    QList<QString> nList;
    QList<int> cList;
    QList<double> pList;
    QList<int> sList;
    if(!sqlHelp->showChart(uName,&nList,&cList,&pList,&sList))
    {
        sendMsg("c#0");
    }
    int size = nList.size();
    QString str = "c#"+QString::number(size);
    QString str1="#n", str2="#c", str3="#p",str4="#s";
    for(int i = 0; i < nList.size(); i++)
    {
        str1+="#";
        str1+=nList[i];
    }
    for(int i = 0; i < cList.size(); i++)
    {
        str2+="#";
        str2+=QString::number(cList[i]);
    }
    for(int i = 0; i < pList.size(); i++)
    {
        str3+="#";
        str3+=QString::number(pList[i],'f',2);
    }
    for(int i = 0; i < sList.size(); i++)
    {
        str4+="#";
        str4+=QString::number(sList[i]);
    }
    sendMsg(str+str1+str2+str3+str4);
}

/*
 * @brief 处理删除购物车请求
 * @param buffer 指令缓存
 */
void MainWindow::deleteChart(const QString &buffer)
{
    QStringList linesplit = buffer.split('#');
    QString uName = linesplit[1];
    QStringList gList;
    for(int i = 2; i < linesplit.size(); i++)
    {
        gList.append(linesplit[i]);
    }
    if(!sqlHelp->deleteChart(uName,&gList))
    {
        sendMsg("d#0");
    }
    else
    {
        sendMsg("d#1");
    }
}

/*
 * @brief 处理创建订单请求
 * @param buffer 指令缓存
 */
void MainWindow::createOrder(const QString &buffer)
{
    QStringList linesplit = buffer.split('#');
    QString uName = linesplit[1];
    int size = linesplit[2].toInt();
    QList<QString> nList;
    QList<int> cList;
    for(int i = 4; i < 4 + size; i++)
    {
        nList.append(linesplit[i]);
    }
    for(int i = 5 + size; i < 5 + 2*size; i++)
    {
        cList.append(linesplit[i].toInt());
    }

    if(!sqlHelp->createOrder(uName,&nList,&cList))
    {
        sendMsg("o#0");
        return;
    }
    else
    {
        sendMsg("o#"+sqlHelp->tempID);
    }
}

/*
 * @brief 处理取消订单请求
 * @param buffer 指令缓存
 */
void MainWindow::cancelOrder(const QString &buffer)
{
    QStringList linesplit = buffer.split('#');
    QString id = linesplit[1];
    if(!sqlHelp->cancelOrder(id))
    {
        sendMsg("z#0");
        return;
    }
    else
    {
        sendMsg("z#1");
    }
}

/*
 * @brief 处理支付请求
 * @param buffer 指令缓存
 */
void MainWindow::finishOrder(const QString &buffer)
{
    QStringList linesplit = buffer.split('#');
    QString id = linesplit[1];
    QString addr = linesplit[2];
    QString userName;
    double uTotal;
    QList<QString> sList;
    QList<double> tList;

    //更新订单
    if(!sqlHelp->finishOrder(id,addr,userName,uTotal,&sList,&tList))
    {
        sendMsg("f#0#0");
        return;
    }
    //修改用户金额
    QString oldVal;
    if(fileHelp->findValue("customer.txt",userName,oldVal,2))
    {
        double nMoney = oldVal.toDouble() - uTotal;
        QString value = QString::number(nMoney,'f',2);
        if(!fileHelp->writeFile("customer.txt",userName,value,2))
        {
            sendMsg("f#0#1");
            return;
        }
    }

    //修改商家金额
    for(int i = 0; i < sList.size(); i++)
    {
        if(fileHelp->findValue("merchant.txt",sList[i],oldVal,2))
        {
            double nMoney = tList[i] + oldVal.toDouble();
            QString value = QString::number(nMoney,'f',2);
            if(!fileHelp->writeFile("merchant.txt",sList[i],value,2))
            {
                sendMsg("f#0#2");
                return;
            }
        }
    }
    sendMsg("f#1");
}

/*
 * @brief 处理订单展示请求
 * @param buffer 指令缓存
 */
void MainWindow::showOrder(const QString &buffer)
{
    QStringList linesplit = buffer.split('#');
    QString uName = linesplit[1];
    QList<QString> nList;
    QList<int> cList;
    QList<double> pList;
    QList<int> stList;
    QList<QString> tList;
    if(!sqlHelp->showOrder(uName,&nList,&cList,&pList,&stList,&tList))
    {
        sendMsg("t#0");
        return;
    }
    int num = nList.size();
    QString str = "t#"+QString::number(num);
    QString str1="",str2="",str3="",str4="",str5="";
    for(int i = 0; i < nList.size(); i++)
    {
        str1 += "#";
        str1 += nList[i];
    }
    for(int i = 0; i < cList.size(); i++)
    {
        str2 += "#";
        str2 += QString::number(cList[i]);
    }
    for(int i = 0; i < pList.size(); i++)
    {
        str3 += "#";
        str3 += QString::number(pList[i],'f',2);
    }
    for(int i = 0; i < stList.size(); i++)
    {
        str4 += "#";
        str4 += QString::number(stList[i]);
    }
    for(int i = 0; i < tList.size(); i++)
    {
        str5 += "#";
        str5 += tList[i];
    }
    sendMsg(str+str1+str2+str3+str4+str5);
}

//建立新连接
void MainWindow::newConnect()
{
    qDebug()<<"成功连接客户端";
    mySocket = myServer->nextPendingConnection();
    connect(mySocket, &QTcpSocket::readyRead, this, &MainWindow::readMsg);
    connect(mySocket, &QTcpSocket::disconnected, this, &MainWindow::disConnect);
}

//读取客户端请求
void MainWindow::readMsg()
{
    QString buffer;
    buffer = mySocket->readAll();
    ui->textBrowser->append("客户端："+buffer);
    //登陆
    if(buffer[0]=='l')
    {
        loginMsg(buffer);
        return;
    }
    //注册
    if(buffer[0]=='r')
    {
        regMsg(buffer);
        return;
    }
    //获得商品列表
    if(buffer[0]=='g')
    {
        getGLMsg();
        return;
    }
    //查询商品信息
    if(buffer[0]=='s')
    {
        goodsMsg(buffer);
        return;
    }
    //加入购物车
    if(buffer[0]=='a')
    {
        addChart(buffer);
        return;
    }
    //展示购物车
    if(buffer[0]=='c')
    {
        showChart(buffer);
        return;
    }
    //删除购物车商品
    if(buffer[0]=='d')
    {
        deleteChart(buffer);
        return;
    }
    //生成订单
    if(buffer[0]=='o')
    {
        createOrder(buffer);
        return;
    }
    //取消订单
    if(buffer[0]=='z')
    {
        cancelOrder(buffer);
        return;
    }
    //进行交易
    if(buffer[0]=='f')
    {
        finishOrder(buffer);
        return;
    }
    //查看订单
    if(buffer[0]=='t')
    {
        showOrder(buffer);
        return;
    }
}

void MainWindow::disConnect()
{
    qDebug()<<"断开连接";
}

