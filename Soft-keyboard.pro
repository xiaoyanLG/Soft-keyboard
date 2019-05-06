#-------------------------------------------------
#
# Project created by QtCreator 2017-11-03T09:42:03
#
#-------------------------------------------------

QT       += core gui sql xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

INCLUDEPATH += chineseInput
QMAKE_CXXFLAGS += -std=c++11

SOURCES  += \
    xyvirtualkeyboard.cpp \
    xypushbutton.cpp \
    xydragablewidget.cpp \
    chineseInput/xydatabaseoperation.cpp \
    chineseInput/xytranslateitem.cpp \
    chineseInput/xyinputsearchinterface.cpp \
    xyhdragabletranslateview.cpp \
    xyvdragabletranslateview.cpp \
    xyqstringview.cpp \
    xyskin.cpp \
    xytempwindows.cpp

HEADERS  += \
    xyvirtualkeyboard.h \
    xypushbutton.h \
    xydragablewidget.h \
    chineseInput/xydatabaseoperation.h \
    chineseInput/xytranslateitem.h \
    chineseInput/xyinputsearchinterface.h \
    xyhdragabletranslateview.h \
    xyvdragabletranslateview.h \
    xyqstringview.h \
    xyskin.h \
    xytempwindows.h

# 这里区分当前编译类型
#DEFINES += THIS_IS_PLUGIN
if(contains(DEFINES,THIS_IS_PLUGIN)){
TEMPLATE = lib
QT += gui-private
CONFIG += plugin

SOURCES  += \
    xyplatforminputcontext.cpp \
    xyinputplugin.cpp

HEADERS  += \
    xyplatforminputcontext.h \
    xyinputplugin.h

} else {
TEMPLATE = app
SOURCES  += \
    main.cpp
}

msvc {
greaterThan(QT_MAJOR_VERSION, 4): RC_FILE = ico.rc
}

RESOURCES += \
    images/images.qrc \
    data/files.qrc

include($$PWD/libgooglepinyin/googlepinyin.pri)
