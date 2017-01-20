TARGET = shiftnet-billing-client
TEMPLATE = app
DESTDIR = $$PWD/../dist
QT = core gui widgets network

SOURCES += \
    main.cpp \
    screenlocker.cpp \
    adminloginwidget.cpp \
    settingswidget.cpp \
    connection.cpp \
    loginwidget.cpp

HEADERS  += \
    screenlocker.h \
    adminloginwidget.h \
    settingswidget.h \
    global.h \
    connection.h \
    loginwidget.h

RESOURCES += \
    main.qrc

FORMS += \
    adminloginwidget.ui \
    settingswidget.ui \
    loginwidget.ui
