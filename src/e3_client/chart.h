#ifndef CHART_H
#define CHART_H

#include <QWidget>
#include <QVBoxLayout>
#include "chartitem.h"
#include "customer.h"
#include "order.h"
namespace Ui {
class Chart;
}

class Chart : public QWidget
{
    Q_OBJECT

public:
    explicit Chart(QWidget *parent = nullptr);
    ~Chart();
    QList<ChartItem*> itemList;
    QList<QString> nList;
    QList<int> cList;
    QList<double> pList;
    QList<int> sList;
    QList<QString> dList;
    Customer *cust;
    Order* orderPage;
    void InitChart();
    void showOrder();

private slots:
    void on_btn_all_clicked();

    void on_btn_delete_clicked();

    void on_btn_order_clicked();


private:
    Ui::Chart *ui;
    QVBoxLayout* pLayout;
    void clearLayout();

signals:
    void deleteItems(QList<QString> *dList);
    void createOrder(QList<QString> *nList,QList<int> *cList);
};

#endif // CHART_H
