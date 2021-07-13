#ifndef SQLHELP_H
#define SQLHELP_H
#include <QString>
#include <QComboBox>
#include <QPushButton>
#include <QTableView>
#include <QLabel>
#include <QtSql/QSqlQueryModel>

class SqlHelp:public QObject
{
public:
    SqlHelp();
    //执行某条sql语句
    bool SqlExec(const QString sql);
    //返回某一个表中的某一列的值
    QStringList GetTableColumnValues(const QString &tableName,
                                const QString &columName,
                                    QString where = "");
    QString GetTableColumnValue(const QString &tableName,
                                         const QString &columnName,
                                         const QString where);
    //将数据绑定到下拉框中
    void BindDataToCombox(const QString &tableName,
                          const QString &tableColumnName,
                          QComboBox *cbox);

    //数据绑定并显示，带查询条件
    void BindTableWhere(QString sql, QTableView *tableview);
    void BindData(QString sql);     //数据绑定

    bool FindValue(const QString &columnName,
                       const QString &tableName,
                       const QString &id);

    //删除数据库记录
    bool DeleteWhere(const QString &tableName,const QString where);

private:
    QTableView *TableView;          //显示数据的表格对象
};

#endif // SQLHELP_H
