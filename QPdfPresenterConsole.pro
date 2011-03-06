#-------------------------------------------------
#
# Project created by QtCreator 2011-03-06T10:45:41
#
#-------------------------------------------------

QT       += core
QT       += gui

TARGET = QPdfPresenterConsole
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

INCLUDEPATH += /usr/include/poppler/qt4
LIBS        += -lpoppler-qt4

SOURCES += main.cpp \
    mainscreenpdfview.cpp \
    presenterpdf.cpp \
    pdfmodel.cpp

HEADERS += \
    mainscreenpdfview.h \
    presenterpdf.h \
    pdfmodel.h \
    app.h
