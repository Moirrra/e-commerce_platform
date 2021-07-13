#ifndef MYSOCKET_H
#define MYSOCKET_H

#include <QObject>
#include<QTcpSocket>

class MySocket : public QTcpSocket
{
    Q_OBJECT
public:
    explicit MySocket(qintptr p);

signals:

};

#endif // MYSOCKET_H
