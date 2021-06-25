#include "addgoods.h"
#include "ui_addgoods.h"
#include <QMessageBox>
#include <QString>
#include <QSqlQuery>
#include <QDebug>
#include <QTime>
#include <QDate>
#include "global.h"
/*
 * @brief 构造函数
 * @param parent 父窗口
 * @param ui ui界面
 */
AddGoods::AddGoods(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AddGoods)
{
    ui->setupUi(this);
    columnCount = 0;
    InitForm();
    InitGoodsInfo();
    InitParamete();

}

/*
 * @brief 析构函数
 */
AddGoods::~AddGoods()
{
    delete ui;
}


/*
 * @brief 初始化成员
 */
void AddGoods::InitForm()
{
    m_pSqlhelp = new SqlHelp();
    m_pStandModel = new QStandardItemModel(this);
}

/*
 * @brief ui控件初始化
 */
void AddGoods::InitParamete()
{
    m_pSqlhelp->BindDataToCombox("GoodsType","TypeName",ui->type_comboBox);
    m_pSqlhelp->BindDataToCombox("GoodsUnit","UnitName",ui->unit_comboBox);
    m_pSqlhelp->BindDataToCombox("GoodsSize","Size",ui->size_comboBox);
    ui->btn_delete->setEnabled(false); //删除按钮不可用
    ui->btn_save->setEnabled(false); //保存按钮不可用
}

/*
 * @brief 初始化商品信息栏
 */
void AddGoods::InitGoodsInfo()
{
    m_pStandModel->setHorizontalHeaderItem(0,new QStandardItem("商品编号"));
    m_pStandModel->setHorizontalHeaderItem(1,new QStandardItem("商品名称"));
    m_pStandModel->setHorizontalHeaderItem(2,new QStandardItem("商品类别"));
    m_pStandModel->setHorizontalHeaderItem(3,new QStandardItem("规格"));
    m_pStandModel->setHorizontalHeaderItem(4,new QStandardItem("单价"));
    m_pStandModel->setHorizontalHeaderItem(5,new QStandardItem("数量"));
    m_pStandModel->setHorizontalHeaderItem(6,new QStandardItem("总价"));
    m_pStandModel->setHorizontalHeaderItem(7,new QStandardItem("单位"));
    m_pStandModel->setHorizontalHeaderItem(8,new QStandardItem("商家"));
    m_pStandModel->setHorizontalHeaderItem(9,new QStandardItem("是否打折"));
    m_pStandModel->setHorizontalHeaderItem(10,new QStandardItem("折扣"));
    ui->tableView->setModel(m_pStandModel);
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);//设置只能选中整行
    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);//设置只能选择一行
    //隐藏列
    ui->tableView->setColumnHidden(9,true);
    ui->tableView->setColumnHidden(10,true);
}

/*
 * @brief 创建商品ID
 * @return GoodsID
 */
QString AddGoods::createGoodsId()
{
    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));//设置随机数种子
    QDate date(QDate::currentDate());
    int year = date.year();
    int month = date.month();
    int day = date.day();
    QString id1 = QString::number(year) + QString("%1").arg(month,2,10,QLatin1Char('0')) + QString("%1").arg(day,2,10,QLatin1Char('0'));
    int n = qrand() % 10000;  //产生10000以内的随机数
    QString id2 = QString("%1").arg(n,4,10,QLatin1Char('0'));
    QString id = id1 + id2;

    //查重
    while(m_pSqlhelp->FindValue("GoodsId","Goods",id))
    {
        int _n = qrand() % 10000;
        QString _id2 = QString("%1").arg(_n,4,10,QLatin1Char('0'));
        id = id1 + _id2;  //更新id
    }
    return id;
}

/*
 * @brief 添加商品信息至表项
 */
void AddGoods::on_btn_add_clicked()
{

    QString goodsId = createGoodsId();
    QString goodsName = ui->name_lineEdit->text();
    QString goodsType = ui->type_comboBox->currentText();
    QString goodsUnit = ui->unit_comboBox->currentText();
    QString goodsSize = ui->size_comboBox->currentText();
    QString goodsSeller = global::userName;
    double unitPrice = ui->doubleSpinBox->value();
    int count = ui->spinBox->value();
    double sum = unitPrice * count;

    if(goodsName.isEmpty())
    {
        QMessageBox::critical(this,"错误","请填写商品信息！");
    }
    else
    {
        for (int i = 0; i < m_pStandModel->rowCount(); i++)
        {
            if(m_pStandModel->index(i,0).data().toString()==goodsId)//ID重复
                goodsId = createGoodsId();
        }
        m_pStandModel->setItem(columnCount,0,new QStandardItem(goodsId));
        m_pStandModel->setItem(columnCount,1,new QStandardItem(goodsName));
        m_pStandModel->setItem(columnCount,2,new QStandardItem(goodsType));
        m_pStandModel->setItem(columnCount,3,new QStandardItem(goodsSize));
        m_pStandModel->setItem(columnCount,4,new QStandardItem(QString::number(unitPrice,'f',2)));
        m_pStandModel->setItem(columnCount,5,new QStandardItem(QString::number(count)));
        m_pStandModel->setItem(columnCount,6,new QStandardItem(QString::number(sum,'f',2)));
        m_pStandModel->setItem(columnCount,7,new QStandardItem(goodsUnit));
        m_pStandModel->setItem(columnCount,8,new QStandardItem(goodsSeller));
        m_pStandModel->setItem(columnCount,9,new QStandardItem("0"));
        m_pStandModel->setItem(columnCount,10,new QStandardItem("1"));
        columnCount++;
        ui->btn_delete->setEnabled(true);
        ui->btn_save->setEnabled(true);
    }
}


/*
 * @brief 保存列表中的表项至数据库
 */
void AddGoods::on_btn_save_clicked()
{
    if (m_pStandModel->rowCount() < 1)
    {
        QMessageBox::critical(this,"错误","保存失败，缺少商品信息！");
    }
    else
    {
        int column = m_pStandModel->columnCount();
        int row = m_pStandModel->rowCount();

        qDebug() <<"row:"<<row<<"column:"<<column;

        for (int i = 0; i < row; i++)
        {
            QVariantList idList,nameList,typeList,sizeList,unitPriceList,countList,
                    sumList,unitList,sellerList,ifDscList,discountList;

            idList << m_pStandModel->index(i,0).data().toString();
            nameList << m_pStandModel->index(i,1).data().toString();
            typeList << m_pStandModel->index(i,2).data().toString();
            sizeList << m_pStandModel->index(i,3).data().toString();
            unitPriceList << m_pStandModel->index(i,4).data().toDouble();
            countList << m_pStandModel->index(i,5).data().toInt();
            sumList << m_pStandModel->index(i,6).data().toDouble();
            unitList << m_pStandModel->index(i,7).data().toString();
            sellerList << m_pStandModel->index(i,8).data().toString();
            ifDscList << m_pStandModel->index(i,9).data().toInt();
            discountList << m_pStandModel->index(i,10).data().toDouble();
            QSqlQuery query;
            QString sql ="INSERT INTO Goods(GoodsId,GoodsName,GoodsCategory,GoodsSize,";
            sql += "GoodsUnitPrice,GoodsNum,GoodsSum,GoodsUnit,GoodsSeller,IfDiscount,Discount)";
            sql += " VALUES(?,?,?,?,?,?,?,?,?,?,?)";

            query.prepare(sql);
            query.addBindValue(idList);
            query.addBindValue(nameList);
            query.addBindValue(typeList);
            query.addBindValue(sizeList);
            query.addBindValue(unitPriceList);
            query.addBindValue(countList);
            query.addBindValue(sumList);
            query.addBindValue(unitList);
            query.addBindValue(sellerList);
            query.addBindValue(ifDscList);
            query.addBindValue(discountList);
            if (!query.execBatch())
            {
                QMessageBox::critical(this,"错误","商品入库失败！");
                return;
            }
        }
        ui->btn_delete->setEnabled(false); //删除按钮不可用
        ui->btn_save->setEnabled(false); //保存按钮不可用

        m_pStandModel->clear();
        ui->name_lineEdit->clear();
        ui->type_comboBox->clear();
        ui->unit_comboBox->clear();
        ui->size_comboBox->clear();
        ui->doubleSpinBox->clear();
        ui->spinBox->clear();

        QMessageBox::information(this,"成功保存","成功添加商品入库！");
    }
}

/*
 * @brief 删除所选表项
 */
void AddGoods::on_btn_delete_clicked()
{
    m_pStandModel->removeRow(ui->tableView->currentIndex().row());
    columnCount = m_pStandModel->rowCount();
}
