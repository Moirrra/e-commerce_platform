#include "stock.h"
#include "ui_stock.h"
#include <QDebug>
#include <QMessageBox>
#include <QSqlError>
#include <QSpinBox>
#include <QSqlRecord>
#include "global.h"
/*
 * @brief 构造函数
 * @param parent 父窗口
 */
Stock::Stock(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Stock)
{
    ui->setupUi(this);
    RowsCountAll = 0; //总记录数
    merch_opt = QObject::tr("GoodsSeller='%1'").arg(global::userName);
    TableEditor();
    updateInfo();
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);//选中整行
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);//设置无法编辑
    ui->tableView->setColumnHidden(11,true);
    ui->tableView->setColumnHidden(12,true);
}

/*
 * @brief 析构函数
 */
Stock::~Stock()
{
    delete ui;
}

/*
 * @brief 显示数据至表格
 */
void Stock::TableEditor()
{
    model = new QSqlTableModel(this);
    model->setTable("Goods");
    model->setEditStrategy(QSqlTableModel::OnManualSubmit);

    model->setHeaderData(0, Qt::Horizontal, tr("商品编码"));
    model->setHeaderData(1, Qt::Horizontal, tr("商品名称"));
    model->setHeaderData(2, Qt::Horizontal, tr("商品类别"));
    model->setHeaderData(3, Qt::Horizontal, tr("规格"));
    model->setHeaderData(4, Qt::Horizontal, tr("单价"));
    model->setHeaderData(5, Qt::Horizontal, tr("数量"));
    model->setHeaderData(6, Qt::Horizontal, tr("总价"));
    model->setHeaderData(7, Qt::Horizontal, tr("单位"));
    model->setHeaderData(8, Qt::Horizontal, tr("商家"));
    model->setHeaderData(9, Qt::Horizontal, tr("是否打折"));
    model->setHeaderData(10, Qt::Horizontal, tr("折扣"));
    model->setHeaderData(11, Qt::Horizontal, tr("图片数据"));
    model->setHeaderData(12, Qt::Horizontal, tr("商品描述"));
    ui->tableView->setModel(model);
    if(global::userName!="admin")
        model->setFilter(merch_opt); //根据商家姓名进行筛选
    model->select(); //显示结果

}

/*
 * @brief 更新数据条目信息
 */
void Stock::updateInfo()
{
    RowsCountAll = model->rowCount();
    ui->labInfo->setText(QString("共有%1条数据").arg(RowsCountAll));
}

/*
 * @brief 设置打折
 * @param d 折扣
 */
void Stock::setDiscount(int d)
{
    int curRow = ui->tableView->currentIndex().row();
    double dsc = 0.01*d;
    double price = model->record(curRow).value(4).toDouble()*dsc;
    double sum = model->record(curRow).value(6).toDouble()*dsc;

    model->setData(model->index(curRow,4),price);
    model->setData(model->index(curRow,6),sum);
    model->setData(model->index(curRow,9),1);
    model->setData(model->index(curRow,10),dsc);
    if (model->submitAll())
    {
        model->database().commit();//提交
        QMessageBox::information(this,tr("打折成功"),tr("成功提交数据库,返回打折结果！"));
    }
    else
    {
        model->database().rollback();//回滚
        QMessageBox::warning(this,tr("折扣"),tr("数据库出错: %1")
                             .arg(model->lastError().text()));
    }
    updateInfo();
}

/*
 * @brief 设置打折
 * @param d 折扣
 * @param type 商品品类
 */
void Stock::setDiscount(int d, QString type)
{
    if(global::userName!="admin")
        model->setFilter(merch_opt + QObject::tr(" AND GoodsCategory='%1'").arg(type));
    else
        model->setFilter(QObject::tr("GoodsCategory='%1'").arg(type));
    model->select();
    int recordCount = model->rowCount();
    double dsc = 0.01*d;
    qDebug()<<"dsc="<<dsc;
    for(int i = 0; i < recordCount; i++)
    {
        double price = model->record(i).value(4).toDouble() * dsc;
        double sum = model->record(i).value(6).toDouble() * dsc;

        model->setData(model->index(i,4),price);
        model->setData(model->index(i,6),sum);
        model->setData(model->index(i,9),1);
        model->setData(model->index(i,10),dsc);
    }
    if (model->submitAll())
    {
        model->database().commit();//提交
        QMessageBox::information(this,tr("打折成功"),tr("成功提交数据库,返回打折结果！"));
    }
    else
    {
        model->database().rollback();//回滚
        QMessageBox::warning(this,tr("折扣"),tr("数据库出错: %1")
                                                .arg(model->lastError().text()));
    }
}

/*
 * @brief 取消折扣
 */
void Stock::cancelDisc()
{
    int curRow = ui->tableView->currentIndex().row();
    double dsc = model->record(curRow).value(10).toDouble();
    double price = model->record(curRow).value(4).toDouble()/dsc;
    double sum = model->record(curRow).value(6).toDouble()/dsc;

    model->setData(model->index(curRow,4),price);
    model->setData(model->index(curRow,6),sum);
    model->setData(model->index(curRow,9),0);
    model->setData(model->index(curRow,10),1);
    if (model->submitAll())
    {
        model->database().commit();//提交
        QMessageBox::information(this,tr("取消折扣成功"),tr("成功提交数据库！"));
    }
    else
    {
        model->database().rollback();//回滚
        QMessageBox::warning(this,tr("取消折扣"),tr("数据库出错: %1")
                                                .arg(model->lastError().text()));
    }
    updateInfo();

}

/*
 * @brief 取消折扣
 * @param type 商品品类
 */
void Stock::cancelDisc(QString type)
{
    if(global::userName!="admin")
        model->setFilter(merch_opt + QObject::tr(" AND GoodsCategory='%1'").arg(type));
    else
        model->setFilter(QObject::tr("GoodsCategory='%1'").arg(type));
    model->select();
    int recordCount = model->rowCount();

    for(int i = 0; i < recordCount; i++)
    {
        double dsc = model->record(i).value(10).toDouble();
        double price = model->record(i).value(4).toDouble() / dsc;
        double sum = model->record(i).value(6).toDouble() / dsc;

        model->setData(model->index(i,4),price);
        model->setData(model->index(i,6),sum);
        model->setData(model->index(i,9),0);
        model->setData(model->index(i,10),1);
    }
    if (model->submitAll())
    {
        model->database().commit();//提交
        QMessageBox::information(this,tr("取消折扣成功"),tr("成功提交数据库！"));
    }
    else
    {
        model->database().rollback();//回滚
        QMessageBox::warning(this,tr("取消折扣"),tr("数据库出错: %1")
                                                .arg(model->lastError().text()));
    }
    updateInfo();
}


/* 按钮 */
//保存修改结果至数据库
void Stock::on_btn_save_clicked()
{
    int recordCount = model->rowCount();
    for(int i = 0; i < recordCount; i++)
    {
        double price = model->record(i).value(4).toDouble();
        int count = model->record(i).value(5).toInt();
        double sum = price * count;

        model->setData(model->index(i,4),price);
        model->setData(model->index(i,6),sum);
    }
    model->database().transaction();//开始事务操作
    if (model->submitAll())
    {
        model->database().commit();//提交
        QMessageBox::information(this,tr("修改成功"),tr("成功提交数据库！"));
    }
    else
    {
        model->database().rollback();//回滚
        QMessageBox::warning(this,tr("库存调整"),tr("数据库出错: %1")
                                                .arg(model->lastError().text()));
    }
}
//撤销未保存的操作
void Stock::on_btn_revert_clicked()
{
    model->revertAll();
}
//刷新表格，返回初始状态
void Stock::on_btn_refresh_clicked()
{
    if(global::userName!="admin")
        model->setFilter(merch_opt); //根据商家姓名进行筛选
    model->select(); //显示结果
}
//删除当前选择记录
void Stock::on_btn_delete_clicked()
{
    int curRow = ui->tableView->currentIndex().row();
    //获取选中的行
    model->removeRow(curRow);
    //删除该行
    int ok = QMessageBox::warning(this,tr("删除当前行!"),tr("确定删除当前行吗？"),QMessageBox::Yes,QMessageBox::No);
    if(ok == QMessageBox::No)
    {
        model->revertAll();//如果不删除，则撤销
    }
    else model->submitAll(); //否则提交，在数据库中删除该行
}
//开始编辑
void Stock::on_btn_edit_clicked()
{
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectItems);//选择单元格
    ui->tableView->setEditTriggers(QAbstractItemView::QAbstractItemView::CurrentChanged);//设置编辑
    readOnlyDelegate = new ReadOnlyDelegate();
    countDelegate = new CountDelegate();
    priceDelegate = new PriceDelegate();
    typeDelegate = new TypeDelegate();
    sizeDelegate = new SizeDelegate();
    unitDelegate = new UnitDelegate();

    ui->tableView->setItemDelegateForColumn(0,readOnlyDelegate);//ID
    ui->tableView->setItemDelegateForColumn(2,typeDelegate);//种类
    ui->tableView->setItemDelegateForColumn(3,sizeDelegate);//规格
    ui->tableView->setItemDelegateForColumn(4,priceDelegate);//单价
    ui->tableView->setItemDelegateForColumn(5,countDelegate);//数量
    ui->tableView->setItemDelegateForColumn(6,readOnlyDelegate);//总价
    ui->tableView->setItemDelegateForColumn(7,unitDelegate);//单位
    ui->tableView->setItemDelegateForColumn(8,readOnlyDelegate); //商家
    ui->tableView->setItemDelegateForColumn(9,readOnlyDelegate);//是否打折
    ui->tableView->setItemDelegateForColumn(10,readOnlyDelegate);//折扣
    ui->tableView->setItemDelegateForColumn(11,readOnlyDelegate);//图片数据
    ui->tableView->setItemDelegateForColumn(12,readOnlyDelegate);//商品描述

}
//折扣优惠
void Stock::on_btn_discount_clicked()
{
    int curRow = ui->tableView->currentIndex().row();
    discountOpt = new Discount(this);
    discountOpt->show();
    QString gName = model->record(curRow).value(1).toString();
    discountOpt->fromParent(gName);
    //当前商品打折
    connect(discountOpt,&Discount::sendData_1,this,[=](int d){
       qDebug()<<"setDiscount1";
       setDiscount(d);
    });
    //选择同一品类商品打折
    connect(discountOpt,&Discount::sendData_2,this,[=](int d, QString type){
       qDebug()<<"setDiscount2";
       setDiscount(d,type);
    });

    connect(discountOpt,&Discount::cancel_1,this,[=](){
       qDebug()<<"cancel1";
       cancelDisc();
    });
    connect(discountOpt,&Discount::cancel_2,this,[=](QString type){
       qDebug()<<"cancel2"<<type;
       cancelDisc(type);
    });
}


