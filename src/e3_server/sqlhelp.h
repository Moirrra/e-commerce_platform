#ifndef SQLHELP_H
#define SQLHELP_H

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>

class SqlHelp
{
public:
    SqlHelp();
    ~SqlHelp();
    void open();
    void close();
    bool FindValue(const QString &columnName,const QString &tableName,const QString &value);
    bool getGoodsList(QStringList *list);
    bool getGoodsInfo(QStringList *list, const QString &name);
    bool addToChart(const QString& uName, const QString& gName, const int num);
    bool showChart(const QString& uName, QList<QString> *nList, QList<int> *cList,
                   QList<double> *pList, QList<int> *sList);
    bool deleteChart(const QString& uName, QStringList *gList);
    bool createOrder(const QString& uName, QList<QString> *nList, QList<int> *cList);
    bool cancelOrder(const QString& ID);
    bool finishOrder(const QString& ID, const QString& addr,QString &uName, double &uTotal,
                     QList<QString> *sList, QList<double> *tList);
    bool showOrder(const QString& uName,QList<QString> *nList,QList<int> *cList,
                   QList<double> *pList,QList<int> *stList,QList<QString> *tList);
    QString tempID;
private:
    QSqlDatabase db;
    QString createOrderID();
};

#endif // SQLHELP_H
