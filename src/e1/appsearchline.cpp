#include "appsearchline.h"
#include <QHBoxLayout>
#include <QPushButton>
#include <QCompleter>
#include "sqlhelp.h"
#include <QStringListModel>

/*
 * @brief 构造函数
 * @param parent 父类
 */
AppSearchLine::AppSearchLine(QWidget *parent)
    :QLineEdit(parent)
{
    QHBoxLayout *mainLayout = new QHBoxLayout;
    searchBtn = new QPushButton;
    searchBtn->setFixedSize(20,20);
    searchBtn->setCursor(Qt::PointingHandCursor);
    searchBtn->setToolTip(tr("搜索"));
    searchBtn->setStyleSheet("QPushButton{border-image:url(:/img/icon/chazhao.png);"
                             "background:transparent;}");
    setPlaceholderText(tr("搜索"));
    mainLayout->addWidget(searchBtn);
    mainLayout->addStretch();
    mainLayout->setContentsMargins(8,0,0,0);
    setTextMargins(20+8+2,0,0,0);
    setContentsMargins(0,0,0,0);
    setLayout(mainLayout);
    setStyleSheet("height:32px;border:1px solid #eaeaea;border-radius:12px;");



}

void AppSearchLine::initCompleter(QString where)
{
    QCompleter *completer = new QCompleter();
    completer->setFilterMode(Qt::MatchContains);//包含
    completer->setCompletionMode(QCompleter::PopupCompletion);//弹出
    completer->setCaseSensitivity(Qt::CaseInsensitive);//不区分大小写
    SqlHelp *myHelp = new SqlHelp();
    QStringList goods_list = myHelp->GetTableColumnValues("Goods","GoodsName",where);
    goods_list = goods_list.toSet().toList();//去重名
    QStringListModel *listModel;
    listModel = new QStringListModel(goods_list, this);
    completer->setModel(listModel);
    this->setCompleter(completer);
}
