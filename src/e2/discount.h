#ifndef DISCOUNT_H
#define DISCOUNT_H

#include <QDialog>
#include "sqlhelp.h"
namespace Ui {
class Discount;
}

class Discount : public QDialog
{
    Q_OBJECT

public:
    explicit Discount(QWidget *parent = nullptr);
    ~Discount();

    void fromParent(QString str);

private:
    Ui::Discount *ui;
    SqlHelp *m_pSqlhelp;
    QString goodsName();

signals:
    void sendData_1(int d);
    void sendData_2(int d, QString type);
    void cancel_1();
    void cancel_2(QString type);

private slots:
    void on_btn_sure_clicked();
    void on_btn_sure_2_clicked();
    void on_btn_cancel_clicked();
    void on_btn_cancel_3_clicked();
};

#endif // DISCOUNT_H
