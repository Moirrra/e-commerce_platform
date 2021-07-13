#include "mysocket.h"

MySocket::MySocket(qintptr p)
{
    this->setSocketDescriptor(p);
}
