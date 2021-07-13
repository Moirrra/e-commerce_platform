#include "mythread.h"
#include <QDebug>
MyThread::MyThread(qintptr p):ptr(p)
{
    qDebug()<<"创建新线程 p:"<<p;
    socket = new MySocket(this->ptr);
    myName = "";
    connect(socket,SIGNAL(disconnected()),this,SLOT(onDisconnect()));
    connect(socket,SIGNAL(readyRead()),this,SLOT(onReadyRead()));
}

MyThread::~MyThread()
{
    socket->abort();
    delete socket;
}

bool MyThread::checkLogin(QString type, QString name, QString password)
{

}

bool MyThread::checkReg(QString type, QString name, QString password)
{

}

void MyThread::onReadyRead()
{
    qDebug()<<"线程"<<ptr<<"收到消息";

}


