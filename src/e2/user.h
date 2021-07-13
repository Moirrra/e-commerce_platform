#ifndef USER_H
#define USER_H
#include <QString>
#include <QDateTime>
#include "goods.h"

class User
{
private:
    QString userType;
    QString userName;
    QString password;
    double money;

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
