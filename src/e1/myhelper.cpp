#include "myhelper.h"
#include <QString>
#include <QFile>
#include <QApplication>
#include <QLineEdit>
myHelper::myHelper()
{

}

/*
 * @brief 加载样式表
 * @param sheetName 样式表名称
 */
void myHelper::loadStyleSheet(const QString &sheetName)
{

    QFile file(QString(":/%1.qss").arg(sheetName));
    file.open(QFile::ReadOnly);
    if (file.isOpen())
    {
        QString qss = QLatin1String(file.readAll());
        qApp->setStyleSheet(qss);
    }
}
