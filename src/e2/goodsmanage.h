#ifndef GOODSMANAGE_H
#define GOODSMANAGE_H

#include <QMainWindow>
#include <QGridLayout>
#include <QWidget>
#include <QPushButton>
#include <QStackedWidget>
#include <QListWidget>
#include "addgoods.h"
#include "stock.h"
#include "setgoods.h"

namespace Ui {
class GoodsManage;
}

class GoodsManage : public QMainWindow
{
    Q_OBJECT

public:
    explicit GoodsManage(QWidget *parent = nullptr);
    ~GoodsManage();

public slots:
    void changePage(QListWidgetItem *current, QListWidgetItem *previous);

private:
    Ui::GoodsManage *ui;

    QListWidget *contentWidget;
    QStackedWidget *pagesWidget;

    //设置QStackedWidget页面
    QStackedWidget *m_pStackedWidget;
    AddGoods *addGoodsPage;
    Stock *stockPage;
    SetGoods *setGoodsPage;

    void createIcons();
};

#endif // GOODSMANAGE_H
