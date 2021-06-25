#include "customer.h"

Customer::Customer()
{
    userType = "消费者";
}

QString Customer::getUserType()
{
    return userType;
}

