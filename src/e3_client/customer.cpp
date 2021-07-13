#include "customer.h"
#include <QMessageBox>
#include <QDebug>
Customer::Customer()
{
    userType = "消费者";
}

QString Customer::getUserType()
{
    return userType;
}



