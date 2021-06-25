#include "sortindex.h"
#include "ui_sortindex.h"
#include <QDebug>
#include <QPushButton>
#include <QMessageBox>
#include <QCompleter>
#include "sqlhelp.h"
#include "searchresult.h"
#include <QStringListModel>

/*
 * @brief 构造函数
 * @param parent 父窗口
 */
SortIndex::SortIndex(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SortIndex)
{
    ui->setupUi(this);
    listHelp = new SqlHelp();
    /*搜索框*/
    searchLine = new AppSearchLine(ui->widget_search);
    searchLine->initCompleter();
    searchLine->setFixedWidth(580);
    //搜索
    connect(searchLine->searchBtn,&QPushButton::clicked,[=](){
        QString name = searchLine->text();
        searchLine->clear();
        SearchResult* resultPage = new SearchResult();
        resultPage->findResult(name);
        resultPage->show();
    });

    /*轮播图*/
    adv = new ImageView(ui->widget);
    adv->addImage(":/img/pic/default.jpg");
    adv->addImage(":/img/pic/c++primerplus.jpg");
    adv->startPlay();

    ui->listWidget->setStyleSheet( "QListWidget::item:hover{background-color:rgb(98,188,214,60)}"
                                   "QListWidget::item:selected{background-color:#0997C0}"
                                   "QListWidget::item:ioutline:0px");

}

/*
 * @brief 析构函数
 */
SortIndex::~SortIndex()
{
    delete ui;
}

void SortIndex::setGoodsList(QString goodsSort)
{
    this->setWindowTitle(goodsSort);
    QString where = QString("WHERE GoodsCategory='%1'").arg(goodsSort);
    QStringList goods_list = listHelp->GetTableColumnValues("Goods","GoodsName",where);
    goods_list = goods_list.toSet().toList();
    for(int i = 0; i< goods_list.size(); i++)
    {
        qDebug()<<"!!!";
        QListWidgetItem* item = new QListWidgetItem();
        item->setText(goods_list[i]);
        ui->listWidget->addItem(item);
    }
}

//返回首页
void SortIndex::on_btn_index_clicked()
{
    emit backIndex();
}

void SortIndex::on_listWidget_itemClicked(QListWidgetItem *item)
{
    QString name = item->text();
    ui->listWidget->currentItem()->setSelected(false);
    SearchResult* resultPage = new SearchResult();
    resultPage->findResult(name);
    resultPage->show();
}


