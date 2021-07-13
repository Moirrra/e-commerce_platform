#ifndef CHART_H
#define CHART_H

#include <QWidget>
#include <QVBoxLayout>
#include "sqlhelp.h"
#include "goods.h"
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
    QList<int> selectList;//选中商品

private slots:
    void on_pushButton_clicked();

    void on_btn_selectAll_clicked();

    void on_btn_delete_clicked();

private:
    Ui::Chart *ui;
    QVBoxLayout* pLayout;
    QList<ChartItem*> itemList;
    Customer *cust;
    Order* order;
};

#endif // CHART_H
