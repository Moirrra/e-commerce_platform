#ifndef ORDERSHOW_H
#define ORDERSHOW_H

#include <QWidget>
#include <QStandardItemModel>
#include <QSqlTableModel>

namespace Ui {
class OrderShow;
}

class OrderShow : public QWidget
{
    Q_OBJECT

public:
    explicit OrderShow(QWidget *parent = nullptr);
    ~OrderShow();


private slots:
    void on_btn_refresh_clicked();

private:
    Ui::OrderShow *ui;
    QSqlTableModel *model;
    int RowsCountAll;

    void TableEditor();
    void updateInfo();
};

#endif // ORDERSHOW_H
