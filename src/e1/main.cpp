#include "mainwindow.h"
#include "login.h"
#include <QApplication>
#include <QSqlError>
#include <QtSql/QSqlQuery>
#include <QMessageBox>
#include <QDebug>
#include "myhelper.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("e_goods.db");
    if (!db.open())
    {
        qDebug() << "Error: Failed to connect database." << db.lastError();
    }
    qDebug()<<"Successfully concect to database.";
    myHelper::loadStyleSheet("new1");
    MainWindow w;
    w.show();
    return a.exec();
}
