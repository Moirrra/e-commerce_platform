#ifndef STOCK_H
#define STOCK_H

#include <QWidget>
#include <QStandardItemModel>
#include <QSqlTableModel>
#include "tableDelegate.h"
#include "discount.h"

namespace Ui {
class Stock;
}

class Stock : public QWidget
{
    Q_OBJECT

public:
    explicit Stock(QWidget *parent = nullptr);
    ~Stock();

private:
    Ui::Stock *ui;

    QSqlTableModel *model;
    int RowsCountAll;
    ReadOnlyDelegate *readOnlyDelegate;
    CountDelegate *countDelegate;
    PriceDelegate *priceDelegate;
    TypeDelegate *typeDelegate;
    SizeDelegate *sizeDelegate;
    UnitDelegate *unitDelegate;
    Discount *discountOpt;
    QString merch_opt;

    void TableEditor();
    void updateInfo();
    void setDiscount(int d, QString type);
    void setDiscount(int d);
    void cancelDisc();
    void cancelDisc(QString type);

private slots:
    void on_btn_save_clicked();
    void on_btn_revert_clicked();
    void on_btn_refresh_clicked();
    void on_btn_delete_clicked();
    void on_btn_edit_clicked();
    void on_btn_discount_clicked();
};

#endif // STOCK_H
