#ifndef SORTINDEX_H
#define SORTINDEX_H

#include <QWidget>
#include "login.h"
#include "user_center.h"
#include "customer.h"
#include "merchant.h"
#include "appsearchline.h"
#include "ImageView.h"
#include "stock.h"
#include "sqlhelp.h"
#include "goodsshow.h"

namespace Ui {
class SortIndex;
}

class SortIndex : public QWidget
{
    Q_OBJECT

public:
    explicit SortIndex(QWidget *parent = nullptr);
    ~SortIndex();

    void setGoodsList(QString goodsSort);

private:
    Ui::SortIndex *ui;
    ImageView *adv;
    AppSearchLine *searchLine;
    SqlHelp *listHelp;
    GoodsShow *showPage;

private slots:

    void on_btn_index_clicked();

    void on_listWidget_itemClicked(QListWidgetItem *item);

signals:
    void backIndex();


};

#endif // SORTINDEX_H
