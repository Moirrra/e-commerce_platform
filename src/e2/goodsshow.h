#ifndef GOODSSHOW_H
#define GOODSSHOW_H

#include <QWidget>
#include "appsearchline.h"
#include "goods.h"
#include <QPixmap>
#include "login.h"
#include "user_center.h"
#include "goods.h"
#include "sqlhelp.h"
#include "customer.h"
#include "merchant.h"
namespace Ui {
class GoodsShow;
}

class GoodsShow : public QWidget
{
    Q_OBJECT

public:
    explicit GoodsShow(QWidget *parent = nullptr);
    ~GoodsShow();

    void setShowPage(QString ID);
    void InitUser(int loginState, QString name);

private:
    Ui::GoodsShow *ui;
    SqlHelp *sqlHelp;
    Goods curGoods;//当前商品
    int login_state;
    Customer *cust;

    bool initGoodsInfo();
    void InitData();
    void saveChart();

private slots:
    void on_btn_chart_clicked();
    void on_btn_addChart_clicked();

    void on_btn_buy_clicked();
};

#endif // GOODSSHOW_H
