#include "discount.h"
#include "ui_discount.h"

/*
 * @brief 构造函数
 * @param parent 父窗口
 * @param ui ui界面
 */
Discount::Discount(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Discount)
{
    ui->setupUi(this);
    m_pSqlhelp = new SqlHelp();
    m_pSqlhelp->BindDataToCombox("GoodsType","TypeName",ui->type_comboBox);
}

/*
 * @brief 析构函数
 */
Discount::~Discount()
{
    delete ui;
}

/*
 * @brief 从父窗口获得字符串
 * @param str 商品名称
 */
void Discount::fromParent(QString str)
{
    ui->label_name->setText(str);
}

/*
 * @brief 确定打折
 */
void Discount::on_btn_sure_clicked()
{
    emit sendData_1(ui->spinBox->value());
    this->close();
}

/*
 * @brief 确定打折
 */
void Discount::on_btn_sure_2_clicked()
{
    emit sendData_2(ui->spinBox_2->value(),ui->type_comboBox->currentText());
    this->close();
}

/*
 * @brief 取消折扣
 */
void Discount::on_btn_cancel_clicked()
{
    emit cancel_1();
    this->close();
}

/*
 * @brief 取消折扣
 */
void Discount::on_btn_cancel_3_clicked()
{
    emit cancel_2(ui->type_comboBox->currentText());
    this->close();
}
