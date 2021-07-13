#include "chartitem.h"
#include "ui_chartitem.h"

ChartItem::ChartItem(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ChartItem)
{
    ui->setupUi(this);
    isChecked = 0;
    this->setStyleSheet("QWidget{background-color:white;border-radius:5px;}"
                            "QSpinBox{background-color:#F7F7F7;}");
}

ChartItem::~ChartItem()
{
    delete ui;

}

/*
 * @brief 初始化购物车物品信息
 */
void ChartItem::InitItem(QString n, int c, double p, int s)
{
    curGoods.setName(n);
    curGoods.setCount(c);
    curGoods.setPrice(p);
    stock = s;
    ui->label_name->setText(n);
    ui->label_price->setText(QString::number(p,'f',2));
    ui->spinBox->setValue(c);
    ui->spinBox->setRange(1,s);
    total = curGoods.getSum();
    ui->label_total->setText(QString::number(total));
}

/*
 * @brief 购物车物品选中设置
 */
void ChartItem::setChecked()
{
    ui->radioButton->setChecked(true);
    isChecked = 1;
}

/*
 * @brief 购物车物品选中状态变更
 * @param checked 是否选中
 */
void ChartItem::on_radioButton_toggled(bool checked)
{
    if(checked==true)
        isChecked = 1;
    else isChecked = 0;
}

/*
 * @brief 购物车物品数量变更
 * @param arg1 数量
 */
void ChartItem::on_spinBox_valueChanged(int arg1)
{
    curGoods.setCount(arg1);
    total = curGoods.getSum();
    ui->label_total->setText(QString::number(total));
}
