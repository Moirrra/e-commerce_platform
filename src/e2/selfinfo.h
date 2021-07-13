#ifndef SELFINFO_H
#define SELFINFO_H

#include <QWidget>
#include <QTimer>

namespace Ui {
class SelfInfo;
}

class SelfInfo : public QWidget
{
    Q_OBJECT

public:
    explicit SelfInfo(QWidget *parent = nullptr);
    ~SelfInfo();
    void InitInfo(int loginState, QString name, QString pwd, double m);

private:
    Ui::SelfInfo *ui;
    int login_state;
    QString userName;
    QString password;
    QString money;
    void upDateInfo();
    QTimer* myTimer;

private slots:
    void myTimeOut();
};

#endif // SELFINFO_H
