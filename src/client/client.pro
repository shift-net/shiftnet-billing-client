TARGET = ShiftNetBillingClient
TEMPLATE = app
DESTDIR = $$PWD/../../dist
QT = core gui widgets websockets
RC_FILE = client.rc

SOURCES += \
    main.cpp \
    screenlocker.cpp \
    adminloginwidget.cpp \
    settingswidget.cpp \
    guestloginwidget.cpp \
    memberloginwidget.cpp \
    application.cpp \
    connection.cpp \
    billingdialog.cpp \
    topupdialog.cpp \
    maintenancedialog.cpp

HEADERS  += \
    screenlocker.h \
    adminloginwidget.h \
    settingswidget.h \
    global.h \
    guestloginwidget.h \
    memberloginwidget.h \
    application.h \
    connection.h \
    billingdialog.h \
    topupdialog.h \
    maintenancedialog.h

RESOURCES += \
    main.qrc

FORMS += \
    adminloginwidget.ui \
    settingswidget.ui \
    guestloginwidget.ui \
    memberloginwidget.ui \
    billingdialog.ui \
    topupdialog.ui \
    maintenancedialog.ui

DISTFILES +=
