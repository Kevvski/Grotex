QT       += core gui sql printsupport

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
    addannexdialog.cpp \
    addfreedialog.cpp \
    addfreewidget.cpp \
    addjobdialog.cpp \
    addjobwidget.cpp \
    adduserdialog.cpp \
    appstate.cpp \
    cadrewidget.cpp \
    cashwidget.cpp \
    checkcontractsdialog.cpp \
    choosefromdialog.cpp \
    companydialog.cpp \
    companywidget.cpp \
    customerswidget.cpp \
    databasewidget.cpp \
    datatabledialog.cpp \
    dbmanager.cpp \
    dbviewdialog.cpp \
    doclistdialog.cpp \
    employeedatawidget.cpp \
    employeeinfodialog.cpp \
    financedialog.cpp \
    imagebutton.cpp \
    main.cpp \
    mainwindow.cpp \
    menuappwidget.cpp \
    orderbilldialog.cpp \
    pagedrawer.cpp \
    printdocuments.cpp \
    printtext.cpp \
    searchsickdialog.cpp \
    settlementsdialog.cpp

HEADERS += \
    Data.h \
    addannexdialog.h \
    addfreedialog.h \
    addfreewidget.h \
    addjobdialog.h \
    addjobwidget.h \
    adduserdialog.h \
    appstate.h \
    cadrewidget.h \
    cashwidget.h \
    checkcontractsdialog.h \
    choosefromdialog.h \
    companydialog.h \
    companywidget.h \
    customerswidget.h \
    databasewidget.h \
    datatabledialog.h \
    dbmanager.h \
    dbviewdialog.h \
    doclistdialog.h \
    employeedatawidget.h \
    employeeinfodialog.h \
    financedialog.h \
    imagebutton.h \
    mainwindow.h \
    menuappwidget.h \
    orderbilldialog.h \
    pagedrawer.h \
    printdocuments.h \
    printtext.h \
    searchsickdialog.h \
    settlementsdialog.h


# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    Resources.qrc
