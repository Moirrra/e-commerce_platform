#include "chart.h"
#include "ui_chart.h"
#include "order.h"
#include "orderitem.h"
#include <QMessageBox>
#include <QDebug>
Chart::Chart(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Chart)
{
    ui->setupUi(this);
    this->setWindowTitle("我的购物车");
    cust = new Customer();
    orderPage = new Order();

}

Chart::~Chart()
{
    delete ui;
    delete orderPage;
}

/*
 * @brief 初始化购物车
 */
void Chart::InitChart()
{
    delete ui->scrollAreaWidgetContents->layout();
    pLayout = new QVBoxLayout();
    clearLayout();
    qDebug()<<"init chart";
    itemList.clear();
    for(int i = 0; i < nList.size(); i++)
    {
        ChartItem* item = new ChartItem();
        item->InitItem(nList[i],cList[i],pList[i],sList[i]);
        itemList.append(item);
    }
    for(int i = 0; i < itemList.size(); i++)
    {
        pLayout->addWidget(itemList[i]);
    }
    if(!itemList.isEmpty())
    {
        ui->scrollAreaWidgetContents->setLayout(pLayout);
        pLayout->setContentsMargins(0, 0, 0, 0);
    }
}


/*
 * @brief 全选
 */
void Chart::on_btn_all_clicked()
{
    for(int i = 0; i < itemList.size(); i++)
    {
        itemList[i]->setChecked();
    }
}

/*
 * @brief 删除
 */
void Chart::on_btn_delete_clicked()
{

    for(int i = 0; i < itemList.size(); i++)
    {
        if(itemList[i]->isChecked)
        {
            pLayout->removeWidget(itemList[i]);
            dList.append(itemList[i]->curGoods.getName());
            itemList.removeAt(i);
            nList.removeAt(i);
            cList.removeAt(i);
            pList.removeAt(i);
            sList.removeAt(i);
            i--;//删除后序号会改变
        }
    }
    emit deleteItems(&dList);
}

/*
 * @brief 结算功能
 */
void Chart::on_btn_order_clicked()
{
    //购物车空
    if(itemList.isEmpty())
    {
        QMessageBox::critical(this,"错误","您的购物车是空的！");
        return;
    }
    //选中商品加入订单
    int flag = 0;
    orderPage->nList.clear();
    orderPage->cList.clear();
    orderPage->pList.clear();
    orderPage->sList.clear();
    for(int i = 0; i < itemList.size(); i++)
    {
        if(itemList[i]->isChecked)
        {
            orderPage->nList.append(itemList[i]->curGoods.getName());
            orderPage->cList.append(itemList[i]->curGoods.getCount());
            orderPage->pList.append(itemList[i]->curGoods.getPrice());
            orderPage->sList.append(itemList[i]->stock);
            flag  = 1;
        }
    }
    if(!flag)
    {//没有选中
        QMessageBox::critical(this,"错误","您未选择任何商品！");
        return;
    }
    orderPage->InitOrder();
    emit createOrder(&orderPage->nList,&orderPage->cList);
}

/*
 * @brief 清空布局
 */
void Chart::clearLayout()
{
    while(pLayout->layout()->count())
    {
        QWidget *pWidget=pLayout->layout()->itemAt(0)->widget();
        if (pWidget)
        {
            pWidget->setParent (NULL);
            pLayout->removeWidget(pWidget);
            delete pWidget;
        }
        else
        {
            QLayout *rLayout=pLayout->itemAt(0)->layout();
            if (rLayout)
            {
                while(rLayout->count())
                {
                    QWidget *pTempWidget=rLayout->itemAt(0)->widget();
                    if (pTempWidget)
                    {
                        pTempWidget->setParent (NULL);
                        rLayout->removeWidget(pTempWidget);
                        delete pTempWidget;
                    }
                    else
                    {
                        rLayout->removeItem(rLayout->itemAt(0));
                    }
                }
            }
            pLayout->removeItem(pLayout->itemAt(0));
        }
    }
}

/*
 * @brief 弹出订单界面
 */
void Chart::showOrder()
{
    orderPage->cust = this->cust;
    orderPage->show();
}

