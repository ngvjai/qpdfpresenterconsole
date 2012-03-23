#-------------------------------------------------
#
# Project created by QtCreator 2011-03-06T10:45:41
#
#-------------------------------------------------

QT       += core
QT       += gui
QT       += dbus
QT       += webkit

TARGET = QPdfPresenterConsole
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

INCLUDEPATH += /usr/include/poppler/qt4
INCLUDEPATH += /usr/include
LIBS        += -lpoppler-qt4
LIBS        += -lvlc

TRANSLATIONS += \
    qpdfpresenterconsole_fr.ts
    qpdfpresenterconsole_es.ts
    qpdfpresenterconsole_de.ts
    qpdfpresenterconsole_it.ts

SOURCES += main.cpp \
    mainscreenpdfview.cpp \
    presenterpdf.cpp \
    pdfmodel.cpp \
    parameters.cpp \
    presentationtimer.cpp \
    qcommandline.cpp \
    optionsdialog.cpp \
    textannot.cpp \
    screensaverinhibit.cpp \
    screensaverinhibiters.cpp \
    freedesktopinhibiter.cpp \
    xdginhibiter.cpp \
    windowsinhibiter.cpp \
    slidewidget.cpp \
    mousemoveeventeater.cpp \
    mediaplayer.cpp \
    helpdialog.cpp \
    osxinhibiter.cpp \
    presenter.cpp

HEADERS += \
    mainscreenpdfview.h \
    presenterpdf.h \
    pdfmodel.h \
    app.h.in \
    parameters.h \
    presentationtimer.h \
    qcommandline.h \
    optionsdialog.h \
    textannot.h \
    screensaverinhibit.h \
    screensaverinhibiters.h \
    freedesktopinhibiter.h \
    xdginhibiter.h \
    windowsinhibiter.h \
    slidewidget.h \
    mousemoveeventeater.h \
    mediaplayer.h \
    helpdialog.h \
    osxinhibiter.h \
    presenter.h

QMAKE_EXTRA_COMPILERS += lrelease
lrelease.input         = TRANSLATIONS
lrelease.output        = ${QMAKE_FILE_BASE}.qm
lrelease.commands      = $$[QT_INSTALL_BINS]/lrelease ${QMAKE_FILE_IN} -qm ${QMAKE_FILE_BASE}.qm
lrelease.CONFIG       += no_link target_predeps

RESOURCES +=

OTHER_FILES += \
    CMakeLists.txt \
    cmake/FindPopplerQt4.cmake \
    cmake/mingw32-cross.cmake \
    cmake/mingw64-cross.cmake \
    README.txt \
    LICENSE.txt \
    .gitignore \
    cmake/FindAsciidoc.cmake \
    qpdfpresenterconsole.txt \
    QPdfPresenterConsole.png \
    QPdfPresenterConsole.desktop \
    cmake/FindLIBVLC.cmake

FORMS += \
    optionsdialog.ui
