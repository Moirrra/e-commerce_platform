#ifndef ORDER_H
#define ORDER_H

#include <QWidget>
#include <QVBoxLayout>
#include "orderitem.h"
#include "customer.h"
#include <QTimer>
namespace Ui {
class Order;
}

class Order : public QWidget
{
    Q_OBJECT

public:
    explicit Order(QWidget *parent = nullptr);
    ~Order();
    QList<OrderItem*> itemList;
    QList<QString> nList;
    QList<int> cList;
    QList<double> pList;
    QList<int> sList;
    Customer *cust;
    void InitOrder();

private:
    Ui::Order *ui;
    QVBoxLayout* pLayout;
    QTimer* tTimer;//支付倒计时
    QTime* timeRecord;
    double sum;

signals:
    void cancelOrder();
    void finishTrade(QString addr,double nMoney);

private slots:
    void on_btn_trade_clicked();
};

#endif // ORDER_H
