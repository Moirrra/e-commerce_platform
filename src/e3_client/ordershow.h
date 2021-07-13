#ifndef ORDERSHOW_H
#define ORDERSHOW_H

#include <QWidget>

namespace Ui {
class OrderShow;
}

class OrderShow : public QWidget
{
    Q_OBJECT

public:
    explicit OrderShow(QWidget *parent = nullptr);
    ~OrderShow();
    QList<QString> nList;
    QList<int> cList;
    QList<double> pList;
    QList<int> stList;
    QList<QString> tList;
    void InitData();
    int rowCount;

private:
    Ui::OrderShow *ui;

signals:
    void refresh();
};

#endif // ORDERSHOW_H
