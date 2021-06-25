#ifndef USER_H
#define USER_H
#include <QString>
#include <QDateTime>
#include "goods.h"
typedef struct tradeList
{
    double account;//花费
    QString goodName; //商品名
    unsigned int amount; //商品数量
    QDateTime tradeTime; //交易时间
}tradeList;

class User
{
private:
    QString userType;
    QString userName;
    QString password;
    double money;
    tradeList myTrade;

public:
    User();
    /* getter */
    virtual QString getUserType()=0;
    QString getUserName();
    QString getUserPwd();
    double getBalance();


    /* setter */
    void setUserName(QString name);
    void setUserPwd(QString psw);
    void setBalance(double m);

};

#endif // USER_H
