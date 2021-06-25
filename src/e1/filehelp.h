#ifndef FILEHELP_H
#define FILEHELP_H

#include <QString>
class FileHelp
{
public:
    FileHelp();

    int writeFile(QString path, QString key, QString value, int pos);
    int findValue(QString path, QString key, QString &value, int pos);
};

#endif // FILEHELP_H
