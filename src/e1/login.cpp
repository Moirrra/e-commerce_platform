#include "login.h"
#include "ui_login.h"
#include <QFile>
#include <QTextStream>
#include <QString>
#include <QMessageBox>
#include <QRadioButton>
#include <QDebug>
#include <QDir>
#include "global.h"

/*
 * @brief 构造函数
 * @param parent 父窗口
 */
login::login(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::login)
{
    ui->setupUi(this);

    this->setWindowTitle("登录");
    //账户类型选择
    TypeGroup = new QButtonGroup(this);
    TypeGroup->addButton(ui->radioButton,0);
    TypeGroup->addButton(ui->radioButton_2,1);

    //输入框限制输入字母/数字/下划线
    QRegExp regx("[a-zA-Z0-9_]+$");
    ui->password->setValidator(new QRegExpValidator(regx,this));
    ui->userName->setValidator(new QRegExpValidator(regx,this));
}

/*
 * @brief 析构函数
 */
login::~login()
{
    delete ui;
}

/*
 * @brief 从控件读取输入信息
 */
void login::readInfo()
{
    userName = ui->userName->text();
    password = ui->password->text();
}

/*
 * @brief 读取消费者账户文件
 * @retval 0 成功
 * @retval -1 失败
 */
int login::readCustomerFile()
{
    QFile file("customer.txt");
    if(!file.open(QIODevice::ReadOnly|QIODevice::Text))
    {
        return -1;
    }
    QTextStream in(&file);
    while (!in.atEnd())//读取账号信息
    {
        QString line=in.readLine();
        customerList.append(line);
    }
    file.close();
    return 0;
}

/*
 * @brief 读取商家账户文件
 * @retval 0 成功
 * @retval -1 失败
 */
int login::readMerchantFile()
{
    QFile file("merchant.txt");
    if(!file.open(QIODevice::ReadOnly|QIODevice::Text))
    {
        return -1;
    }
    QTextStream in(&file);
    while (!in.atEnd())//读取账号信息
    {
        QString line=in.readLine();
        merchantList.append(line);
    }
    file.close();
    return 0;
}

/*
 * @brief 写入消费者账户文件
 * @retval 0 成功
 * @retval -1 失败
 */
int login::appendCustomerFile()
{
    QFile *file = new QFile("customer.txt");
    if(!file->open(QIODevice::WriteOnly|QIODevice::Append|QIODevice::Text))
    {
        return -1;
    }
    //写入文件尾
    QTextStream txtWrite(file);
    txtWrite << userName << " " << password << " 0" << endl;
    qDebug()<<"write successfully"<<endl;
    file->close();
    return 0;
}

/*
 * @brief 写入商家账户文件
 * @retval 0 成功
 * @retval -1 失败
 */
int login::appendMerchantFile()
{
    QFile *file = new QFile("merchant.txt");
    if(!file->open(QIODevice::WriteOnly|QIODevice::Append|QIODevice::Text))
    {
        return -1;
    }
    //写入文件尾
    QTextStream txtWrite(file);
    txtWrite << userName << " " << password << " 0" << endl;
    qDebug()<<"write successfully"<<endl;
    file->close();
    return 0;
}

//点击登录
void login::on_btn_login_clicked()
{
    readInfo();
    //未完全输入
    if(ui->userName->text().isEmpty())
    {
        QMessageBox::critical(this,"警告","用户名为空！","确认");
        return;
    }
    if(ui->password->text().isEmpty())
    {
        QMessageBox::critical(this,"警告","密码为空！","确认");
        return;
    }
    if(!ui->radioButton->isChecked()&&!ui->radioButton_2->isChecked())
    {
        QMessageBox::critical(this,"警告","请选择用户类型！","确认");
        return;
    }



    //顾客登录信息确认
    if(ui->radioButton->isChecked())
    {
        //读取文件失败
        if(readCustomerFile()==-1)
        {
            QMessageBox::critical(this,"错误","消费者信息读取失败！","确认");
            return;
        }
        for (int i = 0; i < customerList.length(); i++)
        {
            QString line = customerList.at(i);
            line = line.trimmed();
            QStringList linesplit=line.split(" ");
            if(userName==linesplit.at(0) && password==linesplit.at(1))
            {
                money = linesplit.at(2).toDouble();
                global::login_state = 1;
                qDebug()<<userName<<" "<<password<<" "<<money<<" "<<global::login_state;
                break;
            }
        }
    }
    //商家登录信息确认
    if(ui->radioButton_2->isChecked())
    {
        if (readMerchantFile()==-1)
        {
            QMessageBox::critical(this,"错误","商家信息读取失败！","确认");
            return;
        }
        for (int i = 0; i < merchantList.length(); i++)
        {
            QString line = merchantList.at(i);
            line = line.trimmed();
            QStringList linesplit=line.split(" ");
            if(userName==linesplit.at(0) && password==linesplit.at(1))
            {
                money = linesplit.at(2).toDouble();
                if(userName=="admin") global::login_state = 3;
                else global::login_state = 2;
                qDebug()<<userName<<" "<<password<<" "<<money<<" "<<global::login_state;
                break;
            }
        }
    }
    //判断登录状态
    if(global::login_state)
    {
        QMessageBox::information(this,"提示信息","您已成功登录！");
        global::userName = userName;
        emit loginSceneBack();
    }
    else
    {
        QMessageBox::critical(this,"错误","用户名或密码错误，请再次输入！");
    }
    //清空输入框
    ui->userName->clear();
    ui->password->clear();
    //光标定位
    ui->userName->setFocus();
}
//返回首页
void login::on_btn_index_clicked()
{
    emit loginSceneBack();
}
//注册
void login::on_btn_reg_clicked()
{
    readInfo();
    if(ui->userName->text().isEmpty())
    {
        QMessageBox::critical(this,"警告","用户名为空！","确认");
        return;
    }
    if(ui->password->text().isEmpty())
    {
        QMessageBox::critical(this,"警告","密码为空！","确认");
        return;
    }
    if(!ui->radioButton->isChecked()&&!ui->radioButton_2->isChecked())
    {
        QMessageBox::critical(this,"警告","请选择用户类型！","确认");
        return;
    }


    //顾客注册信息确认
    if(ui->radioButton->isChecked())
    {
        if(readCustomerFile()==-1)
        {
            QMessageBox::critical(this,"错误","消费者信息读取失败！","确认");
            return;
        }
        for (int i = 0; i < customerList.length(); i++)
        {
            QString line = customerList.at(i);
            line = line.trimmed();
            QStringList linesplit=line.split(" ");
            if(userName==linesplit.at(0))//重名
            {
                QMessageBox::critical(this,"错误","用户名已存在！","确认");
                return;
            }
        }
        if(appendCustomerFile()==-1)
        {
            QMessageBox::critical(this,"错误","消费者信息写入失败！","确认");
            return;
        }
        global::login_state = 1;
        money = 0;
    }

    //商家注册信息确认
    if(ui->radioButton_2->isChecked())
    {
        if (readMerchantFile()==-1)
        {
            QMessageBox::critical(this,"错误","商家信息读取失败！","确认");
            return;
        }
        for (int i = 0; i < merchantList.length(); i++)
        {
            QString line = merchantList.at(i);
            line = line.trimmed();
            QStringList linesplit=line.split(" ");
            if(userName==linesplit.at(0))//重名
            {
                QMessageBox::critical(this,"错误","用户名已存在！","确认");
                return;
            }
        }
        //新增用户信息
        if(appendMerchantFile()==-1)
        {
            QMessageBox::critical(this,"错误","商家信息写入失败！","确认");
            return;
        }
        if(userName=="admin")
        {
            global::login_state = 3;
            money = 0;
        }
        else
        {
            global::login_state = 2;
            money = 0;

        }
    }

    //注册成功
    if(global::login_state)
    {
        QMessageBox::information(this,"提示信息","您已成功注册！");
        global::userName = userName;
        emit loginSceneBack();
    }
    //清空输入框
    ui->userName->clear();
    ui->password->clear();
    //光标定位
    ui->userName->setFocus();
}

