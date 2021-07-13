#ifndef RECHARGE_H
#define RECHARGE_H

#include <QDialog>
#include <filehelp.h>
#include <QTimer>
namespace Ui {
class recharge;
}

class recharge : public QDialog
{
    Q_OBJECT

public:
    explicit recharge(QWidget *parent = nullptr);
    ~recharge();

private slots:
    void on_btn_sure_clicked();
    void myTimeOut();
private:
    Ui::recharge *ui;
    double curMoney;
    double addMoney;
    double sum;
    FileHelp *fileHelp;
    QTimer* myTimer;
};

#endif // RECHARGE_H
