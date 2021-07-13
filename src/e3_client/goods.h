#ifndef GOODS_H
#define GOODS_H

#include <QString>
#include <QPixmap>
class Goods
{
public:
    Goods();

    /* getter */
    virtual double getPrice() const;
    QString getID() const;
    QString getName() const;
    virtual QString getType() const;
    QString getSize() const;
    QString getUnit() const;
    int getCount() const;
    double getSum() const;
    QString getSeller() const;
    QString getDetails() const;
    int getIfDiscount() const;
    double getDiscount() const;
    QPixmap getImage() const;

    /* setter */
    void setID(QString id);
    void setName(QString name);
    void setType(QString type);
    void setSize(QString size);
    void setUnit(QString unit);
    void setPrice(double m);
    void setCount(int c);
    void setSeller(QString s);
    void setDiscount(double d);
    void setImg(QPixmap img);
    void setDetails(QString t);

private:
    QString ID;
    QString goodsName;
    QString goodsType;
    QString goodsSize;
    QString goodsUnit;
    double unitPrice;
    int count;
    QString seller;
    QString details;
    int ifDiscount;
    double discount;
    QPixmap pixmap;

};

class Book:public Goods
{
public:
    Book();
private:

};

class Clothes:public Goods
{
public:
    Clothes();

};

class Shoes:public Goods
{
public:
    Shoes();

};

class Shuma:public Goods
{
public:
    Shuma();

};

class Rihua:public Goods
{
public:
    Rihua();

};

class Food:public Goods
{
public:
    Food();

};



#endif // GOODS_H
