QT += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

INCLUDEPATH += include

SOURCES += \
    src/chatwidget.cpp \
    src/clientmanager.cpp \
    src/main.cpp \
    src/mainwindow.cpp \
    src/protocol.cpp \
    src/server.cpp \
    src/settingdialog.cpp \
    src/textchat.cpp

HEADERS += \
    include/chatwidget.h \
    include/clientmanager.h \
    include/mainwindow.h \
    include/networkutils.h \
    include/protocol.h \
    include/server.h \
    include/settingdialog.h \
    include/textchat.h

FORMS += \
    forms/chatwidget.ui \
    forms/mainwindow.ui \
    forms/settingdialog.ui \
    forms/textchat.ui

qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    qrc.qrc
