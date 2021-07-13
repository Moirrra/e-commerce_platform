#ifndef CHARTITEM_H
#define CHARTITEM_H

#include <QWidget>
#include "goods.h"
namespace Ui {
class ChartItem;
}

class ChartItem : public QWidget
{
    Q_OBJECT

public:
    explicit ChartItem(QWidget *parent = nullptr);
    ~ChartItem();
    void InitItem(QString n, int c, double p, int s);
    void setChecked();
    Goods curGoods;
    int isChecked;
    double total;
    int stock;
private slots:
    void on_radioButton_toggled(bool checked);

    void on_spinBox_valueChanged(int arg1);

private:
    Ui::ChartItem *ui;


};

#endif // CHARTITEM_H
