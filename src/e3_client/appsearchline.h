#ifndef APPSEARCHLINE_H
#define APPSEARCHLINE_H
#include <QLineEdit>
#include <QPushButton>
class AppSearchLine : public QLineEdit
{
    Q_OBJECT
public:
    AppSearchLine(QWidget *parent = 0);
    QPushButton *searchBtn;
    void initCompleter(QStringList goods_list);
signals:
    void cliked_search(QStringList goods_list);

};
#endif // APPSEARCHLINE_H
