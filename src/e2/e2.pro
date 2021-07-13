QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    ImageView.cpp \
    addgoods.cpp \
    appsearchline.cpp \
    changepwd.cpp \
    chart.cpp \
    chartitem.cpp \
    customer.cpp \
    discount.cpp \
    filehelp.cpp \
    global.cpp \
    goods.cpp \
    goodsmanage.cpp \
    goodsshow.cpp \
    login.cpp \
    main.cpp \
    mainwindow.cpp \
    merchant.cpp \
    myhelper.cpp \
    order.cpp \
    orderitem.cpp \
    ordershow.cpp \
    recharge.cpp \
    searchresult.cpp \
    searchresultitem.cpp \
    selfinfo.cpp \
    setgoods.cpp \
    sortindex.cpp \
    sqlhelp.cpp \
    stock.cpp \
    user.cpp \
    user_center.cpp

HEADERS += \
    ImageView.h \
    addgoods.h \
    appsearchline.h \
    changepwd.h \
    chart.h \
    chartitem.h \
    customer.h \
    discount.h \
    filehelp.h \
    global.h \
    goods.h \
    goodsmanage.h \
    goodsshow.h \
    login.h \
    mainwindow.h \
    merchant.h \
    myhelper.h \
    order.h \
    orderitem.h \
    ordershow.h \
    recharge.h \
    searchresult.h \
    searchresultitem.h \
    selfinfo.h \
    setgoods.h \
    sortindex.h \
    sqlhelp.h \
    stock.h \
    tableDelegate.h \
    user.h \
    user_center.h

FORMS += \
    addgoods.ui \
    changepwd.ui \
    chart.ui \
    chartitem.ui \
    discount.ui \
    goodsmanage.ui \
    goodsshow.ui \
    login.ui \
    mainwindow.ui \
    order.ui \
    orderitem.ui \
    ordershow.ui \
    recharge.ui \
    searchresult.ui \
    searchresultitem.ui \
    selfinfo.ui \
    setgoods.ui \
    sortindex.ui \
    stock.ui \
    user_center.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    res.qrc

QMAKE_CXXFLAGS +=  -Wno-unused-parameter
