QT       += core gui
QT       += quick
QT       += widgets
QT += core websockets
QT += webenginewidgets
QT += network




greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17
CONFIG += utf8

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    line1.cpp \
    main.cpp \
    editor.cpp \
    plus.cpp \
    star.cpp \
    textbox.cpp

HEADERS += \
    color.h \
    color2.h \
    color3.h \
    editor.h \
    line1.h \
    plus.h \
    star.h \
    textbox.h

FORMS += \
    editor.ui \
    line1.ui \
    star.ui \
    textbox.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    icons.qrc
RC_FILE=res.rc

msvc{
    QMAKE_CXXFLAGS += /source-charset:utf-8 /execution-charset:utf-8
}
