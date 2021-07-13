#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtNetwork>
#include <QMessageBox>
#include <QListWidgetItem>
#include "appsearchline.h"
#include "customer.h"
#include "merchant.h"
#include "login.h"
#include "goodsshow.h"
#include "chart.h"
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void Init();
    void connectServer();
    void sendMsg(QString ins);

private:
    Ui::MainWindow *ui;
    AppSearchLine *searchLine;
    QTcpSocket* tcpSocket;
    Login* loginPage;
    GoodsShow* showPage;
    Chart* myChart;
    //用户信息
    int userType;
    Customer* cust;
    Merchant* merch;

    void setGoodsList(const QString &buffer);
    void recvLogin(const QString &buffer);
    void recvReg(const QString &buffer);
    void recvGoods(const QString &buffer);
    void recvAddChart(const QString &buffer);
    void recvChart(const QString &buffer);
    void recvDeleteChart(const QString &buffer);
    void recvOrder(const QString &buffer);
    void recvCancel(const QString &buffer);
    void recvFinish(const QString &buffer);
    void showOrder(const QString &buffer);
    //商品
    QStringList goodsList;
    Goods tempGoods;
    QString tempID;//临时存储订单编号

public slots:
    void readMsg();
    void goodsShow();
    void addToChart(QString name, int num);
    void deleteChart(QList<QString> *dList);
    void createOrders(QList<QString> *nList,QList<int> *cList);
    void cancelOrders();
    void finishOrders(QString addr,double nMoney);

private slots:
    void on_btn_login_clicked();
    void on_btn_connect_clicked();
    void on_btn_logout_clicked();
    void on_listWidget_itemClicked(QListWidgetItem *item);
    void on_btn_chart_clicked();

    void on_btn_order_clicked();

signals:
    void getGoods();
    void getChart();
};
#endif // MAINWINDOW_H
