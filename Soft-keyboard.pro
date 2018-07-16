#-------------------------------------------------
#
# Project created by QtCreator 2017-11-03T09:42:03
#
#-------------------------------------------------

QT       += core gui sql xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = XYInputMethod
TEMPLATE = app

INCLUDEPATH += chineseInput

SOURCES += main.cpp \
    xykeyboardfilter.cpp \
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
    xykeyboardfilter.h \
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

QMAKE_CXXFLAGS += -std=c++11

greaterThan(QT_MAJOR_VERSION, 4): RC_FILE = ico.rc

RESOURCES += \
    images/images.qrc \
    data/files.qrc

include($$PWD/libgooglepinyin/googlepinyin.pri)
