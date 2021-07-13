#include "filehelp.h"
#include <QDebug>
#include <QTextStream>

FileHelp::FileHelp()
{

}

/*
 * @brief 查找文本文件内容
 * @param path 文件路径
 * @param key 键值
 * @param value 查找结果
 * @param pos 字符串位置
 * @retval true找到 false没找到/文件打开失败
 */
bool FileHelp::findValue(QString path, QString key, QString &value, int pos)
{
    bool flag = false;
    QFile file(path);
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
            flag = true;
            break;
        }
    }
    file.close();
    return flag;
}

/*
 * @brief 登录检验
 * @param path 文件路径
 * @param name 用户名
 * @param password 密码
 * @param value 余额
 * @retval 1成功 0用户名不存在 -1密码错误 -2打开文件错误
 */
int FileHelp::findLogin(QString path, QString name, QString password, QString &value)
{
    int flag = 0;
    QFile file(path);
    if(!file.open(QIODevice::ReadOnly|QIODevice::Text))
    {
        qDebug()<<"findLogin open error";
        return -2;
    }
    QTextStream rstream(&file);
    while (!rstream.atEnd())//读取账号信息
    {
        QString line= rstream.readLine(); //读取一行，不包括换行符
        line = line.trimmed();
        QStringList linesplit = line.split(" ");//将一行的内容用空格分隔成元素
        if(name==linesplit.at(0))//找到用户名
        {
            if(password==linesplit.at(1));
            {
                value = linesplit.at(2);
                flag = 1;
                break;
            }
            flag = -1;
            break;
        }
    }
    file.close();
    return flag;
}

/*
 * @brief 注册检验
 * @param path 文件路径
 * @param name 用户名
 * @param password 密码
 * @retval 1成功 0重名 -1文件打开失败
 */
int FileHelp::findReg(QString path, QString name, QString password)
{
    int flag = 1;
    QFile file(path);
    //read
    if(!file.open(QIODevice::ReadOnly|QIODevice::Text))
    {
        qDebug()<<"findLogin open error";
        return -1;
    }
    QTextStream rstream(&file);
    while (!rstream.atEnd())//读取账号信息
    {
        QString line= rstream.readLine(); //读取一行，不包括换行符
        line = line.trimmed();
        QStringList linesplit = line.split(" ");//将一行的内容用空格分隔成元素
        if(name==linesplit.at(0))//重名
        {
            flag = 0;
            break;
        }
    }
    file.close();
    //append
    if(!file.open(QIODevice::WriteOnly|QIODevice::Append|QIODevice::Text))
    {
        return -1;
    }
    //写入文件尾
    QTextStream txtWrite(&file);
    txtWrite << name << " " << password << " 0" << endl;
    qDebug()<<"write successfully"<<endl;
    file.close();
    return flag;
}

/*
 * @brief 修改文本内容
 * @param path 文件路径
 * @param key 键值
 * @param value 要修改的原值
 * @param pos 字符串位置
 * @retval 1成功 0失败
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


