#ifndef FILEHELP_H
#define FILEHELP_H

#include <QFile>

class FileHelp
{
public:
    FileHelp();
    bool findValue(QString path, QString key, QString &value, int pos);
    int findLogin(QString path, QString name, QString password, QString &value);
    int findReg(QString path, QString name, QString password);
    int writeFile(QString path, QString key, QString value, int pos);
};

#endif // FILEHELP_H
