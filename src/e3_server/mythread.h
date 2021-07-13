#ifndef MYTHREAD_H
#define MYTHREAD_H

#include <QObject>
#include <QThread>
#include <QTcpSocket>
#include "mysocket.h"
#include "sqlhelp.h"
#include "filehelp.h"
class MyThread : public QThread
{
    Q_OBJECT
public:
    explicit MyThread(qintptr p);
    ~MyThread();
    bool checkLogin(QString type, QString name, QString password);
    bool checkReg(QString type, QString name, QString password);

protected:
    virtual void run();

private:
    qintptr ptr;
    MySocket* socket;
    QString myName;//该线程对应用户名
    SqlHelp* sqlHelp;
    FileHelp* fileHelp;

private slots:
    void onReadyRead();
    void onDisconnect();

signals:

};

#endif // MYTHREAD_H
