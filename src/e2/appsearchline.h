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
    void initCompleter(QString where="");
signals:
    void cliked_search(QString name);

};
#endif // APPSEARCHLINE_H
