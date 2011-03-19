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

TRANSLATIONS += \
    qpdfpresenterconsole_fr.ts

SOURCES += main.cpp \
    mainscreenpdfview.cpp \
    presenterpdf.cpp \
    pdfmodel.cpp \
    parameters.cpp \
    presentationtimer.cpp \
    qcommandline.cpp

HEADERS += \
    mainscreenpdfview.h \
    presenterpdf.h \
    pdfmodel.h \
    app.h.in \
    parameters.h \
    presentationtimer.h \
    qcommandline.h

QMAKE_EXTRA_COMPILERS += lrelease
lrelease.input         = TRANSLATIONS
lrelease.output        = ${QMAKE_FILE_BASE}.qm
lrelease.commands      = $$[QT_INSTALL_BINS]/lrelease ${QMAKE_FILE_IN} -qm ${QMAKE_FILE_BASE}.qm
lrelease.CONFIG       += no_link target_predeps

RESOURCES +=

OTHER_FILES += \
    CMakeLists.txt \
    cmake/FindPopplerQt4.cmake
