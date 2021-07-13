#ifndef GLOBAL_H
#define GLOBAL_H
#include <QString>

class global
{
public:
    global();
    static int login_state;//未登录0 消费者1 商家2
    static QString userName;
    static double money;
};

#endif // GLOBAL_H
