#ifndef CHANGEPWD_H
#define CHANGEPWD_H

#include <QDialog>
#include "filehelp.h"
namespace Ui {
class changePwd;
}

class changePwd : public QDialog
{
    Q_OBJECT

public:
    explicit changePwd(QWidget *parent = nullptr);
    ~changePwd();

private slots:
    void on_pushButton_clicked();

private:
    Ui::changePwd *ui;
    QString pwd;
    QString r_pwd;
    FileHelp *fileHelp;
};

#endif // CHANGEPWD_H
