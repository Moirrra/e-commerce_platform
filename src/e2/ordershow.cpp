#include "ordershow.h"
#include "ui_ordershow.h"
#include "global.h"

OrderShow::OrderShow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::OrderShow)
{
    ui->setupUi(this);
    this->setWindowTitle("历史订单");
    RowsCountAll = 0; //总记录数
    TableEditor();
    updateInfo();
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);//设置无法编辑
}

OrderShow::~OrderShow()
{
    delete ui;
}

void OrderShow::TableEditor()
{
    model = new QSqlTableModel(this);
    model->setTable("Orders");
    model->setHeaderData(0, Qt::Horizontal, tr("订单编码"));
    model->setHeaderData(1, Qt::Horizontal, tr("用户名"));
    model->setHeaderData(2, Qt::Horizontal, tr("商品编码"));
    model->setHeaderData(3, Qt::Horizontal, tr("商品名称"));
    model->setHeaderData(4, Qt::Horizontal, tr("规格"));
    model->setHeaderData(5, Qt::Horizontal, tr("单价"));
    model->setHeaderData(6, Qt::Horizontal, tr("数量"));
    model->setHeaderData(7, Qt::Horizontal, tr("总价"));
    model->setHeaderData(8, Qt::Horizontal, tr("商家"));
    model->setHeaderData(9, Qt::Horizontal, tr("状态"));
    model->setHeaderData(10, Qt::Horizontal, tr("交易时间"));
    model->setHeaderData(11, Qt::Horizontal, tr("收货地址"));
    ui->tableView->setModel(model);
    if(global::login_state!=3)
        model->setFilter(QString("UserName = '%1'").arg(global::userName));
    model->select();
}

void OrderShow::updateInfo()
{
    RowsCountAll = model->rowCount();
    ui->label_info->setText(QString("共有%1条数据").arg(RowsCountAll));
}

//刷新
void OrderShow::on_btn_refresh_clicked()
{
    if(global::login_state!=3)
        model->setFilter(QString("UserName = '%1'").arg(global::userName));
    model->select();
    RowsCountAll = model->rowCount();
    ui->label_info->setText(QString("共有%1条数据").arg(RowsCountAll));
}
