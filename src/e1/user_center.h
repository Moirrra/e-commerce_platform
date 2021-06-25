#ifndef USER_CENTER_H
#define USER_CENTER_H

#include <QMainWindow>
#include "customer.h"
#include "merchant.h"
#include "changepwd.h"
#include "recharge.h"
#include "goodsmanage.h"
#include "selfinfo.h"
namespace Ui {
class user_center;
}

class user_center : public QMainWindow
{
    Q_OBJECT

public:
    explicit user_center(QWidget *parent = nullptr);
    ~user_center();

private:
    Ui::user_center *ui;
    changePwd *change_pwd;
    recharge *rech;
    GoodsManage *gManage;
    SelfInfo *myInfo;

signals:
    void centerBack();

private slots:
    void on_btn_index_clicked();

    void on_btn_changePwd_clicked();

    void on_btn_recharge_clicked();

    void on_btn_mback_clicked();

    void on_btn_info_clicked();


    void on_btn_chart_clicked();
    void on_pushButton_2_clicked();
};

#endif // USER_CENTER_H
