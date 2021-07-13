#include "setgoods.h"
#include "ui_setgoods.h"
#include <QFileDialog>
#include <QFileInfo>
#include <QScreen>
#include <QGuiApplication>
#include <QDir>
#include <QDebug>
#include <QSqlQuery>
#include <QMessageBox>
#include "global.h"

/*
 * @brief 构造函数
 * @param parent 父窗口
 */
SetGoods::SetGoods(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SetGoods)
{
    ui->setupUi(this);
    InitData();
}

/*
 * @brief 析构函数
 */
SetGoods::~SetGoods()
{
    delete ui;
    InitData();
}

void SetGoods::InitData()
{

    /*商品列表*/
    listHelp = new SqlHelp();
    QString sql;
    if(global::userName=="admin")
    {
        sql = QObject::tr("select GoodsName from Goods;");
    }
    else
    {
        sql = QObject::tr("select GoodsName from Goods where GoodsSeller='%1';")
            .arg(global::userName);
    }
    listHelp->BindTableWhere(sql,ui->tableView);
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    ui->tableView->horizontalHeader()->hide();
    ui->tableView->setColumnWidth(0, 260);


    /*搜索框*/
    searchLine = new AppSearchLine(ui->widget_search);
    searchLine->initCompleter(QString(" WHERE GoodsSeller = '%1' ").arg(global::userName));
    searchLine->setFixedWidth(250);
    //搜索
    connect(searchLine->searchBtn,&QPushButton::clicked,[=](){
        QString name = searchLine->text();
        searchLine->clear();
        int rowcnt = ui->tableView->model()->rowCount();
        for(int i=0;i<rowcnt;i++)
        {
            QModelIndex index = ui->tableView->model()->index(i,0);
            if (index.data() == name)
            {
                ui->tableView->setCurrentIndex(index);
                gname = index.data().toString();
                ui->label_name->setText(gname);
                /*原始数据*/
                QSqlQuery query;
                QString sql = QString("SELECT * FROM Goods WHERE GoodsName= '%1' AND GoodsSeller = '%2' ").arg(gname).arg(global::userName);
                if(query.exec(sql))
                {
                    if(query.first())
                    {
                        //图片
                        QPixmap pixmap;
                        QByteArray outByteArray = query.value(11).toByteArray();
                        pixmap.loadFromData(outByteArray);
                        QSize imgSize = ui->label_img->size();
                        ui->label_img->setPixmap(pixmap.scaled(imgSize,Qt::KeepAspectRatio, Qt::SmoothTransformation));
                        //描述
                        QString text = query.value(12).toString();
                        ui->plainTextEdit->setPlainText(text);
                    }
                }
                break;
            }
            if(i==rowcnt-1)
            {
                qDebug()<<"item not found";
                QMessageBox::critical(this,"错误","该商品不存在！");
            }

        }
    });
}

/*
 * @brief 点击表格项
 * @param index 指向选中表项序号的指针
 */
void SetGoods::on_tableView_clicked(const QModelIndex &index)
{
    gname = index.data().toString();
    ui->label_name->setText(gname);

    /*原始数据*/
    QSqlQuery query;
    QString sql = QString("SELECT * FROM Goods WHERE GoodsName= '%1'").arg(gname);
    if(query.exec(sql))
    {
        if(query.first())
        {
            //图片
            QPixmap pixmap;
            QByteArray outByteArray = query.value(11).toByteArray();
            pixmap.loadFromData(outByteArray);
            QSize imgSize = ui->label_img->size();
            ui->label_img->setPixmap(pixmap.scaled(imgSize,Qt::KeepAspectRatio, Qt::SmoothTransformation));
            //描述
            QString text = query.value(12).toString();
            ui->plainTextEdit->setPlainText(text);
        }
    }
}

/*
 * @brief 点击选择文件按钮
 */
void SetGoods::on_btn_choose_clicked()
{
    //打开文件夹中的图片文件
    OpenFile = QFileDialog::getOpenFileName(this,"Please choose an image file",
                                            "","Image Files(*.jpg *.png *.bmp *.pgm *.pbm);;All(*.*)");
    if( OpenFile != "" )
    {
        if( image.load(OpenFile) )
        {
            QSize imgSize = ui->label_img->size();
            QPixmap pixmap = QPixmap::fromImage(image);
            ui->label_img->setPixmap(pixmap.scaled(imgSize,
                Qt::KeepAspectRatio, Qt::SmoothTransformation));
        }
    }

    //显示所示图片的路径
    QFileInfo OpenFileInfo;
    OpenFileInfo = QFileInfo(OpenFile);
    QString FileName = OpenFileInfo.fileName();
    ui->label_path->setText(OpenFile);
}

/*
 * @brief 点击保存按钮
 */
void SetGoods::on_btn_save_clicked()
{
    int flag = 0;
    QSqlQuery query;

    //保存图片到数据库
    QFile file(OpenFile);
    if(file.open(QIODevice::ReadOnly))
    {
        inByteArray = file.readAll();
        file.close();
        query.prepare("UPDATE Goods SET Image = :imgData WHERE GoodsName = :gName");
        query.bindValue(":imgData",inByteArray);
        query.bindValue(":gName",gname);
        if(!query.exec())
        {
            qDebug()<<"Update imgData error";
        }
        else flag=1;
    }

    //保存商品描述到数据库
    QString text = ui->plainTextEdit->toPlainText();
    if(!text.isEmpty())
    {
        query.prepare("UPDATE Goods SET Details = :text WHERE GoodsName = :gName");
        query.bindValue(":text",text);
        query.bindValue(":gName",gname);
        if(!query.exec())
        {
            qDebug()<<"Update Details error";
        }
        else flag=1;
    }

    if(flag)
        {
         QMessageBox::information(this,"保存成功","商品信息保存成功！");
         ui->plainTextEdit->clear();
         ui->label_img->clear();
    }
}

void SetGoods::on_btn_refresh_clicked()
{
    InitData();
}
