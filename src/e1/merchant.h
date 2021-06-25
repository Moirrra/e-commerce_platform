#ifndef MERCHANT_H
#define MERCHANT_H
#include "user.h"

class Merchant:public User
{
private:
    QString userType;
    QString userName;
    QString password;
    double money;
public:
    Merchant();

    virtual QString getUserType();
};

#endif // MERCHANT_H
