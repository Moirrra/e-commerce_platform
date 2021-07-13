#include "searchresult.h"
#include "ui_searchresult.h"
#include <QSqlQuery>
#include <QVBoxLayout>
#include <QMessageBox>
#include "login.h"
#include <QDebug>
#include "sqlhelp.h"
#include "goodsshow.h"
#include "user_center.h"
SearchResult::SearchResult(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SearchResult)
{
    ui->setupUi(this);
    rlayout = new QVBoxLayout();
    this->setWindowTitle("查找结果");
}

SearchResult::~SearchResult()
{
    delete ui;
}

//根据搜索框的名字查找GoodsID
void SearchResult::findResult(QString name)
{
    searchName = name;
    SqlHelp *sqlHelp = new SqlHelp();
    QString where = QString("WHERE GoodsName = '%1'").arg(name);
    idList.clear();
    idList = sqlHelp->GetTableColumnValues("Goods","GoodsId",where);
    if(!idList.isEmpty())
        updateResult();
    else
    {
        QMessageBox::warning(this,"查找失败","不存在该商品，请重新查找！");
    }
}

void SearchResult::updateResult()
{
    itemList.clear();
    for(int i = 0; i < idList.size(); i++)
    {
        SearchResultItem* tempItem = new SearchResultItem();
        tempItem ->initInfo(idList[i]);
        itemList.append(tempItem);
        rlayout->addWidget(itemList[i]);
        qDebug()<<idList[i];
    }
    ui->scrollAreaWidgetContents->setLayout(rlayout);

}


