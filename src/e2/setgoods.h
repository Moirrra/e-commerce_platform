#ifndef SETGOODS_H
#define SETGOODS_H

#include <QWidget>
#include "appsearchline.h"
#include "sqlhelp.h"
#include <QFile>
namespace Ui {
class SetGoods;
}

class SetGoods : public QWidget
{
    Q_OBJECT

public:
    explicit SetGoods(QWidget *parent = nullptr);
    ~SetGoods();

private:
    Ui::SetGoods *ui;
    AppSearchLine *searchLine;
    SqlHelp *listHelp;

    QString sellerName;
    QString OpenFile;//打开的文件路径
    QString OpenFilePath;
    QImage image;
    QString savePath;

    QString gname;
    QByteArray inByteArray;

    void InitData();

signals:
    void clicked(const QModelIndex &);

private slots:
    void on_tableView_clicked(const QModelIndex &index);
    void on_btn_choose_clicked();
    void on_btn_save_clicked();
    void on_btn_refresh_clicked();
};

#endif // SETGOODS_H
