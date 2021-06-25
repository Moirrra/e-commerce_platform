#include "filehelp.h"
#include <QFile>
#include <QMessageBox>
#include <QDebug>
#include <QTextStream>
FileHelp::FileHelp()
{

}


/*
 * @brief 修改文件
 * @param path 文件路径
 * @retval 1 成功
 * @retval 0 失败
 */
int FileHelp::writeFile(QString path, QString key, QString value, int pos)
{
    QFile file(path);
    int flag = 0;
    if(!file.open(QIODevice::ReadOnly|QIODevice::Text))
    {
        qDebug()<<"writeFile open error";
        return flag;
    }
    QTextStream rstream(&file);
    QStringList strList;
    while (!rstream.atEnd())//读取账号信息
    {
        QString line=rstream.readLine(); //读取一行，不包括换行符
        strList.append(line);
    }
    file.close();

    if(!file.open(QIODevice::WriteOnly|QIODevice::Text))
    {
        qDebug()<<"writeFile open error";
        return flag;
    }
    QTextStream wstream(&file);
    for(int i = 0; i < strList.length(); i++)
    {
        QString line = strList.at(i);
        line = line.trimmed();
        QStringList linesplit = line.split(" ");//将一行的内容用空格分隔成元素
        if(key==linesplit.at(0))//找到用户名
        {
            qDebug()<<"find";
            linesplit.replace(pos,value);//替换为新值
            wstream<<linesplit.at(0)<<" "<<linesplit.at(1)<<" "<<linesplit.at(2)<<"\n";
            flag = 1;
        }
        //非替换行
        else
        {
            wstream << strList.at(i) << endl;
        }
    }
    file.close();
    return flag;
}

int FileHelp::findValue(QString path, QString key, QString &value, int pos)
{
    QFile file(path);
    int flag = 0;
    if(!file.open(QIODevice::ReadOnly|QIODevice::Text))
    {
        qDebug()<<"findValue open error";
        return flag;
    }
    QTextStream rstream(&file);
    while (!rstream.atEnd())//读取账号信息
    {
        QString line= rstream.readLine(); //读取一行，不包括换行符
        line = line.trimmed();
        QStringList linesplit = line.split(" ");//将一行的内容用空格分隔成元素
        if(key==linesplit.at(0))//找到用户名
        {
            value = linesplit.at(pos);
            flag = 1;
            break;
        }
    }
    file.close();
    return flag;
}
