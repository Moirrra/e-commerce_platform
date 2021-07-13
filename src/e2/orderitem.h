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
    void InitItem(Goods newGoods);
    Goods goods;
    double total;

private:
    Ui::OrderItem *ui;


};

#endif // ORDERITEM_H
