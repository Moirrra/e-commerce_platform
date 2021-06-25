#ifndef CUSTOMER_H
#define CUSTOMER_H
#include "user.h"

class Customer:public User
{
private:
    QString userType;
    QString userName;
    QString password;
    double money;

public:
    Customer();

    virtual QString getUserType();

};

#endif // CUSTOMER_H
