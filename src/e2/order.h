#ifndef ORDER_H
#define ORDER_H

#include <QWidget>
#include "orderitem.h"
#include <QVBoxLayout>
#include <customer.h>
#include <QTimer>
#include "sqlhelp.h"
namespace Ui {
class Order;
}

class Order : public QWidget
{
    Q_OBJECT

public:
    explicit Order(QWidget *parent = nullptr);
    ~Order();
    void InitOrder(QList<int> list);

private slots:
    void on_lineEdit_editingFinished();

    void on_pushButton_clicked();

private:
    Ui::Order *ui;
    QVBoxLayout* pLayout;
    QList<OrderItem*> itemList;
    Customer *cust;
    double sum;//总金额
    QTimer* tTimer;//支付倒计时
    QTime* timeRecord;
    SqlHelp* mSqlhelp;
    QString address;
    QString orderID;
    QString tradeTime;
    QString createID();
    void orderDelete();
};

#endif // ORDER_H
