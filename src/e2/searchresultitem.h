#ifndef SEARCHRESULTITEM_H
#define SEARCHRESULTITEM_H

#include <QWidget>
#include "goods.h"

namespace Ui {
class SearchResultItem;
}

class SearchResultItem : public QWidget
{
    Q_OBJECT

public:
    explicit SearchResultItem(QWidget *parent = nullptr);
    ~SearchResultItem();
    void initInfo(QString GoodsID);
    void mousePressEvent(QMouseEvent* event);

private:
    Ui::SearchResultItem *ui;
    Goods curGoods;
};

#endif // SEARCHRESULTITEM_H
