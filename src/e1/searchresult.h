#ifndef SEARCHRESULT_H
#define SEARCHRESULT_H

#include <QWidget>
#include <QVBoxLayout>
#include "searchresultitem.h"
#include "customer.h"
#include "merchant.h"
#include "sqlhelp.h"
namespace Ui {
class SearchResult;
}

class SearchResult : public QWidget
{
    Q_OBJECT

public:
    explicit SearchResult(QWidget *parent = nullptr);
    ~SearchResult();
    void findResult(QString name);
    void updateResult();

private:
    Ui::SearchResult *ui;
    QVBoxLayout* rlayout;
    SqlHelp *sqlHelp;
    QString searchName;
    QString clikeID;
    QVector<SearchResultItem*> itemList;
    QStringList idList;//存储查找结果的ID

};

#endif // SERACHRESULT_H
