#include "myserver.h"
#include "mythread.h"
#include "mysocket.h"
#include <QDebug>
MyServer::MyServer()
{


}

void MyServer::incomingConnection(qintptr socketDescriptor)
{
    qDebug()<<"有新连接";
    MyThread* thread = new MyThread(socketDescriptor);
    thread->start();
}

