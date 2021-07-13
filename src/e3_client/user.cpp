#include "user.h"

User::User()
{

}

/* getter*/
QString User::getUserType()
{
    return userType;
}
QString User::getUserName()
{
    return userName;
}

QString User::getUserPwd()
{
    return password;
}
double User::getBalance()
{
    return money;
}

/*setter*/
void User::setUserName(QString name)
{
    userName = name;
}
void User::setUserPwd(QString pwd)
{
    password = pwd;
}
void User::setBalance(double m)
{
    money = m;
}
