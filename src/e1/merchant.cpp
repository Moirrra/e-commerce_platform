#include "merchant.h"

Merchant::Merchant()
{
    userType = "商家";
}

QString Merchant::getUserType()
{
    return userType;
}
