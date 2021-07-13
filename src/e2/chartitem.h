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
    void InitItem(Goods newGoods);
    int isChecked;//商品是否被选中
    int isValid;
    void setChecked();//选中该商品
    QString getGoodsID();//返回商品ID
    int getGoodsCount();
private slots:
    void on_radioButton_toggled(bool checked);

    void on_spinBox_valueChanged(int arg1);

private:
    Ui::ChartItem *ui;
    Goods goods;
};

#endif // CHARTITEM_H
