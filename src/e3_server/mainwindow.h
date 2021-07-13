#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtNetwork>
#include <QTcpServer>
#include <QTcpSocket>
#include "sqlhelp.h"
#include "filehelp.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void sendMsg(const QString &ins);

    //收到消息分类
    void loginMsg(const QString &buffer);
    void regMsg(const QString &buffer);
    void getGLMsg();
    void goodsMsg(const QString &buffer);
    void addChart(const QString &buffer);
    void showChart(const QString &buffer);
    void deleteChart(const QString &buffer);
    void createOrder(const QString &buffer);
    void cancelOrder(const QString &buffer);
    void finishOrder(const QString &buffer);
    void showOrder(const QString &buffer);
public slots:
    void newConnect();
    void readMsg();
    void disConnect();

private:
    Ui::MainWindow *ui;
    QTcpServer* myServer;
    QTcpSocket* mySocket;
    SqlHelp* sqlHelp;
    FileHelp* fileHelp;
};
#endif // MAINWINDOW_H
