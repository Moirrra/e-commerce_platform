#include "sqlhelp.h"
#include <QtSql/QSqlQuery>
#include <QHeaderView>
#include <QDebug>
#include <QPushButton>
#include<QSqlError>
SqlHelp::SqlHelp()
{

}

bool SqlHelp::SqlExec(const QString sql)
{
    QSqlQuery query;
    if(!query.exec(sql))
    {
        return false;
        qDebug()<<query.lastError();
    }
    return true;
}

/*
 * @brief 获得指定表指定列的数据
 * @param tableName 表名
 * @param columnName 列名
 * @retval list 存储列内容
 */
QStringList SqlHelp::GetTableColumnValues(const QString &tableName, const QString &columnName,
                                          QString where)
{
    QSqlQuery query;
    QStringList list;
    QString sql = QString("SELECT %1 FROM %2 ").arg(columnName).arg(tableName) + where;
    query.exec(sql);
    while (query.next())
        list.append(query.value(0).toString());
    return list;
}

/*
 * @brief 获得指定数据
 * @param tableName 表名
 * @param columnName 列名
 * @param where 筛选条件
 * @retval result 结果字符串
 */
QString SqlHelp::GetTableColumnValue(const QString &tableName, const QString &columnName, const QString where)
{
    QSqlQuery query;
    QString result;
    QString sql = QString("SELECT %1 FROM %2 ").arg(columnName).arg(tableName)+where;
    query.exec(sql);
    if(query.first())
        result = query.value(0).toString();
    return result;
}


/*
 * @brief 绑定数据至下拉框
 * @param tableName 表名
 * @param columnName 列名
 * @param cbox 控件名称
 */
void SqlHelp::BindDataToCombox(const QString &tableName, const QString &tableColumnName,
                               QComboBox *cbox)
{
    QSqlQuery query;
    query.exec("SELECT "+tableColumnName+" FROM "+tableName+" ORDER BY "+tableColumnName+" DESC");
    while(query.next())
    {
        cbox->addItem(query.value(0).toString());
    }
}


/*
 * @brief 带查询条件绑定数据至tableview
 * @param sql sql语句
 * @param tableName 表名
 */
void SqlHelp::BindTableWhere(QString sql,QTableView *tableview)
{
    TableView = tableview;
    QSqlQueryModel *model = new QSqlQueryModel;
    model->setQuery(sql);
    TableView->setModel(model);

    TableView->horizontalHeader()->setHighlightSections(false);//点击表头时不对表头光亮

    TableView->setSelectionMode(QAbstractItemView::SingleSelection);//选中模式单行选中
    TableView->setSelectionBehavior(QAbstractItemView::SelectRows);//选中整行
    TableView->setStyleSheet( "QTableView::item:hover{background-color:rgb(98,188,214,60)}"
                              "QTableView::item:selected{background-color:#0997C0}");
}

/*
 * @brief
 * @param columnName 列名
 * @param tableName 表名
 * @param value 查找值
 * @retval true 查找成功
 * @retval false 查找失败
 */
bool SqlHelp::FindValue(const QString &columnName,const QString &tableName,const QString &value)
{
    QSqlQuery query;
    QString sql = QString("SELECT %1 FROM %2").arg(tableName).arg(columnName);
    while(query.exec())
    {
        if(query.value(0).toString()==value)
            return true;
    }
    return false;
}

/*
 * @brief 条件删除
 * @param tableName 表名
 * @param where 筛选条件
 * @retval result 结果字符串
 */
bool SqlHelp::DeleteWhere(const QString &tableName, const QString where)
{
    QSqlQuery query;
    QString sql = QString("DELETE FROM %1 ").arg(tableName)+where;
    if(!query.exec(sql))
        return false;
    return true;
}




