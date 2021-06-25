#ifndef ADDGOODS_H
#define ADDGOODS_H

#include <QWidget>
#include <QStandardItemModel>
#include "sqlhelp.h"
namespace Ui {
class AddGoods;
}

class AddGoods : public QWidget
{
    Q_OBJECT

public:
    explicit AddGoods(QWidget *parent = nullptr);
    ~AddGoods();

    void InitForm();
    void InitParamete();
    void CreateConnect();
    void InitGoodsInfo();               //商品信息初始化
    QString createGoodsId();

private slots:
    void on_btn_add_clicked();

    void on_btn_save_clicked();

    void on_btn_delete_clicked();

private:
    Ui::AddGoods *ui;
    bool GetGoodsId(QString &id);
    SqlHelp *m_pSqlhelp;
    QStandardItemModel *m_pStandModel;
    int columnCount;
};

#endif // ADDGOODS_H
