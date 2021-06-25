#include "goods.h"

Goods::Goods()
{
    ID = "";
    goodsName = "";
    goodsType = "";
    goodsSize = "";
    goodsUnit = "";
    unitPrice = 0;
    count = 0;
    goodsSum = 0;
    remnant = 0;
    seller = "";
    details = "";
    ifDiscount = 0;
    discount = 0;
}



/* getter */
double Goods::getPrice() const
{
    return unitPrice;
}

QString Goods::getID() const
{
    return ID;
}
QString Goods::getName() const
{
    return goodsName;
}

QString Goods::getType() const
{
    return goodsType;
}

QString Goods::getSize() const
{
    return goodsSize;
}

QString Goods::getUnit() const
{
    return goodsUnit;
}

int Goods::getCount() const
{
    return count;
}

double Goods::getSum() const
{
    return goodsSum;
}

int Goods::getRemnant() const
{
    return remnant;
}

QString Goods::getSeller() const
{
    return  seller;
}
QString Goods::getDetails() const
{
    return details;
}
int Goods::getIfDiscount() const
{
    return ifDiscount;
}
double Goods::getDiscount() const
{
    return discount;
}
QPixmap Goods::getImage() const
{
    return pixmap;
}



/* setter */
void Goods::setID(QString id)
{
    ID = id;
}

void Goods::setName(QString name)
{
    goodsName = name;
}

void Goods::setType(QString type)
{
    goodsType = type;
}

void Goods::setSize(QString size)
{
    goodsSize = size;
}
void Goods::setUnit(QString unit)
{
    goodsUnit = unit;
}

void Goods::setPrice(double m)
{
    unitPrice = m;
}

void Goods::setCount(int c)
{
    count = c;
}

void Goods::setSeller(QString s)
{
    seller = s;
}

void Goods::setDiscount(double d)
{
    discount = d;
}

void Goods::setImg(QPixmap img)
{
    pixmap = img;
}

void Goods::setDetails(QString t)
{
    details = t;
}

//具体商品类
Book::Book()
{
    setType("book");
}

Clothes::Clothes()
{
    setType("clothes");
}
Food::Food()
{
    setType("food");
}
