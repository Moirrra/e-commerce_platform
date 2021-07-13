#ifndef MYSERVER_H
#define MYSERVER_H

#include <QObject>
#include<QTcpServer>

class MyServer : public TcpServer
{
    Q_OBJECT
public:
    MyServer();

signals:

private:
    virtual void incomingConnection(qintptr socketDescriptor);
};

#endif // MYSERVER_H
