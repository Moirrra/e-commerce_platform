#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QPushButton>
#include <QMessageBox>
#include <QListWidgetItem>
#include <QCompleter>
#include <QStringListModel>
#include <QtSql/QSqlQuery>
#include "goodsshow.h"
#include "searchresult.h"
#include "global.h"
/*
 * @brief 构造函数
 * @param parent 父窗口
 */
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{   
    ui->setupUi(this);

    ui->btn_login->setVisible(1);
    ui->btn_logout->setVisible(0);
    //登录后隐藏按钮
    if(global::login_state)
    {
        ui->btn_login->setVisible(0);
        ui->btn_logout->setVisible(1);
    }

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
        this->hide();

    });

    /*轮播图*/
    adv = new ImageView(ui->widget);
    adv->addImage(":/img/pic/default.jpg");
    adv->addImage(":/img/pic/c++primerplus.jpg");
    adv->addImage(":/img/pic/TB183NQapLM8KJjSZFBSutJHVXa.jpg");
    adv->addImage(":/img/pic/chouzhi.jpg");
    adv->startPlay();
    connect(adv,SIGNAL(clickImg(int)),this,SLOT(chooseImg(int)));

    /* 分类列表 */
    QStringList  strList;
    strList<<QString("数码/家电")<<QString("图书/音像")<<QString("零食/生鲜")
          <<QString("男装/女装")<<QString("日化/医药")<<QString("鞋子/运动");
    ui->listWidget->addItems(strList);
    ui->listWidget->setStyleSheet( "QListWidget::item:hover{background-color:rgb(98,188,214,60)}"
                                   "QListWidget::item:selected{background-color:#0997C0}");
}

/*
 * @brief 析构函数
 */
MainWindow::~MainWindow()
{
    delete ui;
}

/*
 * @brief 响应点击“退出账号”事件
 */
void MainWindow::on_btn_logout_clicked()
{
    if(global::login_state)
    {
        global::userName = "";
        global::login_state = 0;
        ui->btn_login->setVisible(1);
        ui->btn_logout->setVisible(0);
    }
    else
        QMessageBox::critical(this,"错误","您未登录！");
}

/*
 * @brief 响应点击“登录/注册”事件
 */
void MainWindow::on_btn_login_clicked()
{
    login *loginScene = new login();
    loginScene->show();
    this->hide();
    //监听返回信号
    connect(loginScene,&login::loginSceneBack,this,[=](){
        qDebug()<<"log in: back";
        if(global::login_state)
        {
            ui->btn_login->setVisible(0);
            ui->btn_logout->setVisible(1);
        }
        this->show();
        loginScene->hide();
    });
}

/*
 * @brief 响应点击“用户中心”事件
 */
void MainWindow::on_btn_userCenter_clicked()
{
    if(global::login_state==0)
    {
        QMessageBox::critical(this,"错误","请先登录！");
        return;
    }
    else
    {
        user_center *centerScene = new user_center();
        this->hide();
        centerScene->show();
        //监听返回信号
        connect(centerScene,&user_center::centerBack,this,[=](){
            if(global::login_state)
            {
                ui->btn_login->setVisible(0);
                ui->btn_logout->setVisible(1);
            }
            centerScene->close();
            this->show();
        });
    }
}

/*
 * @brief 响应点击“购物车”事件
 */
void MainWindow::on_btn_chart_clicked()
{
    QMessageBox::information(this,"提示","请期待2.0版本!");
}

/*
 * @brief 点击商品分类列表
 * @param item 点击的列表项指针
 */
void MainWindow::on_listWidget_itemClicked(QListWidgetItem *item)
{
    int curRow = ui->listWidget->currentRow();
    qDebug()<<curRow;
    ui->listWidget->currentItem()->setSelected(false);
    SortIndex *sortIndex = new SortIndex();
    switch (curRow) {
    case 0:
        sortIndex->setGoodsList("数码");
        break;
    case 1:
        sortIndex->setGoodsList("图书");
        break;
    case 2:
        sortIndex->setGoodsList("食品");
        break;
    case 3:
        sortIndex->setGoodsList("服饰");
        break;
    case 4:
        sortIndex->setGoodsList("日化");
        break;
    case 5:
        sortIndex->setGoodsList("鞋子");
        break;
    default:
        break;
    }
    sortIndex->show();
    this->hide();
    /* 监听返回首页信号 */
    connect(sortIndex,&SortIndex::backIndex,this,[=](){
        sortIndex->hide();
        this->show();
    });
}

/*
 * @brief 响应点击轮播图事件
 * @param index 图片序号
 */
void MainWindow::chooseImg(int index)
{
    SearchResult* resultPage = new SearchResult(this);
    qDebug()<<"index="<<index;
    switch(index)
    {
    case 0:
        break;
    case 1:
        resultPage->findResult("C++ Primer");
        resultPage->show();
        break;
    case 2:
        break;
    case 3:
        resultPage->findResult("清风抽纸");
        resultPage->show();
        break;
    }
}



