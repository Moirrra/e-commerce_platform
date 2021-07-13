#ifndef CUSTOMER_H
#define CUSTOMER_H
#include "user.h"

class Customer:public User
{
private:
    QString userType;
    QString userName;
    QString password;
    double money;
    QList<Goods> ChartList;

public:
    Customer();
    virtual QString getUserType();
    bool addToChartList(Goods newGoods);
    QList<Goods> getChartList();
    void clearChartList();
    bool updateChartInfo();
    bool changeChartItemCount(QString id,int c);
    bool deleteChartItem(QString id);
};

#endif // CUSTOMER_H
