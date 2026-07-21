QT += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

SOURCES += \
    chatwidget.cpp \
    clientmanager.cpp \
    main.cpp \
    mainwindow.cpp \
    protocol.cpp \
    server.cpp \
    textchat.cpp

HEADERS += \
    chatwidget.h \
    clientmanager.h \
    mainwindow.h \
    networkutils.h \
    protocol.h \
    server.h \
    textchat.h

FORMS += \
    chatwidget.ui \
    mainwindow.ui \
    textchat.ui

qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
