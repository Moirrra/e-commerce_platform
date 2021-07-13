#ifndef GOODSSHOW_H
#define GOODSSHOW_H

#include <QWidget>
#include "goods.h"
namespace Ui {
class GoodsShow;
}

class GoodsShow : public QWidget
{
    Q_OBJECT

public:
    explicit GoodsShow(QWidget *parent = nullptr);
    ~GoodsShow();
    Goods curGoods;
    void InitData();
    int userType;

private slots:
    void on_btn_add_clicked();


private:
    Ui::GoodsShow *ui;

signals:
    void addChart(QString name, int num);

};

#endif // GOODSSHOW_H
