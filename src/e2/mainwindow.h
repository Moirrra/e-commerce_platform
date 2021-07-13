#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "login.h"
#include "user_center.h"
#include "customer.h"
#include "merchant.h"
#include "appsearchline.h"
#include "ImageView.h"
#include "sortindex.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    ImageView *adv;
    AppSearchLine *searchLine;

private slots:
    void on_btn_logout_clicked();

    void on_btn_login_clicked();

    void on_btn_userCenter_clicked();

    void on_btn_chart_clicked();

    void on_listWidget_itemClicked(QListWidgetItem *item);

    void chooseImg(int index);


signals:

};
#endif // MAINWINDOW_H
