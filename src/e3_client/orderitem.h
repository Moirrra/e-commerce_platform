#ifndef ORDERITEM_H
#define ORDERITEM_H

#include <QWidget>
#include "goods.h"

namespace Ui {
class OrderItem;
}

class OrderItem : public QWidget
{
    Q_OBJECT

public:
    explicit OrderItem(QWidget *parent = nullptr);
    ~OrderItem();

    void InitItem(QString n, int c, double p, int s);
    Goods curGoods;
    double total;
    int stock;
private:
    Ui::OrderItem *ui;
};

#endif // ORDERITEM_H
