#include "ordershow.h"
#include "ui_ordershow.h"

OrderShow::OrderShow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::OrderShow)
{
    ui->setupUi(this);
    this->setWindowTitle("历史订单");
}

OrderShow::~OrderShow()
{
    delete ui;
}

//初始化历史订单展示
void OrderShow::InitData()
{
    ui->tableWidget->clear();
    QStringList headerText;
    headerText<<"交易时间"<<"商品名称"<<"单价"<<"数量"<<"总价"<<"订单状态";  //表头标题用QStringList来表示
    ui->tableWidget->setColumnCount(headerText.count());//列数设置为与 headerText的行数相等
    ui->tableWidget->setRowCount(rowCount);//总行数
    ui->tableWidget->setHorizontalHeaderLabels(headerText);

    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);//取消单元格的点击编辑
    ui->tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);//单选
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);//只能选中行

    int rowIndex =0;
    // 设置表格内每一行的数据
    for(int i = 0; i < rowCount; i++)
    {
        QTableWidgetItem *item_time = new QTableWidgetItem (tList[i]);
        QTableWidgetItem *item_gname = new QTableWidgetItem (nList[i]);
        QTableWidgetItem *item_price = new QTableWidgetItem (QString::number(pList[i],'f',2));
        QTableWidgetItem *item_count = new QTableWidgetItem (QString::number(cList[i]));
        QTableWidgetItem *item_sum = new QTableWidgetItem (QString::number(pList[i]*cList[i],'f',2));
        QTableWidgetItem *item_state = new QTableWidgetItem (QString::number(stList[i]));
        ui->tableWidget->setItem(rowIndex,0,item_time);
        ui->tableWidget->setItem(rowIndex,1,item_gname);
        ui->tableWidget->setItem(rowIndex,2,item_price);
        ui->tableWidget->setItem(rowIndex,3,item_count);
        ui->tableWidget->setItem(rowIndex,4,item_sum);
        ui->tableWidget->setItem(rowIndex,5,item_state);
        rowIndex++;
    }

}

