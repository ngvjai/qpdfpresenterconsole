# vim: set et ts=4 sw=4:

cmake_minimum_required(VERSION 2.6)

if(NOT CMAKE_INSTALL_PREFIX)
    set(NO_INSTALL_PREFIX ON)
endif(NOT CMAKE_INSTALL_PREFIX)

project(QPdfPresenterConsole)
set(CMAKE_MODULE_PATH
    "${PROJECT_SOURCE_DIR}/cmake"
    "${CMAKE_MODULE_PATH}"
)

set(PROJECT_TARGETBIN "qpdfpresenterconsole")
set(PROJECT_SHORTNAME "qpdfpresenterconsole")
set(PROJECT_FULLNAME "QPdfPresenterConsole")
set(PROJECT_VERSION_MAJOR "2")
set(PROJECT_VERSION_MINOR "5")
set(PROJECT_VERSION_PATCH "11")
set(PROJECT_VERSION "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}")
set(PROJECT_DATADIR "${CMAKE_INSTALL_PREFIX}/share/${PROJECT_SHORTNAME}")
set(inputdoc "${CMAKE_SOURCE_DIR}/${PROJECT_TARGETBIN}.txt")
set(htmldoc "${PROJECT_TARGETBIN}.html")
set(htmlcss "docbook-xsl.css")
set(mandoc "${PROJECT_TARGETBIN}.1")

option(PROJECT_DEBUG "Compile in Debug profile" OFF)

include(CMakeDependentOption)

CMAKE_DEPENDENT_OPTION(PROJECT_RELEASE "Compile for distribution" ON "NOT PROJECT_DEBUG" OFF)
CMAKE_DEPENDENT_OPTION(PROJECT_STRIP_BIN "Strip Windows binaries" ON "NOT PROJECT_DEBUG" OFF)
CMAKE_DEPENDENT_OPTION(PROJECT_PACK_BIN "Compress Windows binaries" ON "NOT PROJECT_DEBUG" OFF)

if(PROJECT_DEBUG)
    set(HAVE_DEBUG 1)
    set(CMAKE_BUILD_TYPE "Debug")
endif(PROJECT_DEBUG)

if(DEBUG_ZONES)
    set(HAVE_DEBUG_AREA 1)
endif(DEBUG_ZONES)

if(PROJECT_RELEASE)
    set(CMAKE_BUILD_TYPE "MinRelSize")
endif(PROJECT_RELEASE)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release")
endif(NOT CMAKE_BUILD_TYPE)

set(DEFAULT_OPTION_DURATION 20 CACHE STRING "Default presentation length (min)")
set(DEFAULT_OPTION_EMERGENCY 2 CACHE STRING "Default emergency time (min)")
set(DEFAULT_OPTION_SLIDESWIDTH 55 CACHE STRING "Default main slide width (prcent)")
set(DEFAULT_OPTION_PAGE 1 CACHE STRING "Default first page opened")
set(DEFAULT_OPTION_MAINSCREEN 1 CACHE STRING "Default main screen identifier")
set(DEFAULT_OPTION_PROJECTORSCREEN 2 CACHE STRING "Default secondary screen identifier")
set(DEFAULT_OPTION_BEAMERNOTES false CACHE STRING "Enable Beamer Notes support")
set(DEFAULT_OPTION_BEAMERNOTESPART \"right\" CACHE STRING "Part of the slides containing the Beamer Notes (right/left)")
set(DEFAULT_OPTION_TEXTANNOT false CACHE STRING "Enable text file annotations support")
set(DEFAULT_OPTION_CHECKMULTIDISPLAY true CACHE STRING "Enable checking the presence of multiple screen at runtime")
set(DEFAULT_OPTION_CONFIGFILE \"\" CACHE STRING "Config file to use to load parameters from")

# ---- #
set(CPACK_PACKAGE_CONTACT "Alexandre Lissy <alexandre.lissy@etu.univ-tours.fr>")
set(CPACK_PACKAGE_VENDOR "Alexandre Lissy")
set(CPACK_PACKAGE_VERSION_MAJOR ${PROJECT_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${PROJECT_VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${PROJECT_VERSION_PATCH})
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY
    "${PROJECT_NAME} is a Beamer-oriented presenting tool, allowing the speaker to have a main screen with informations, and presentation on the second screen.")
set(CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_SOURCE_DIR}/README.txt")
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_SOURCE_DIR}/LICENSE.txt")

# Packages-specific
set(CPACK_RPM_PACKAGE_LICENSE "MIT")
set(CPACK_RPM_PACKAGE_GROUP "Graphical desktop/Other")
set(CPACK_RPM_PACKAGE_URL "http://gitorious.org/qpdfpresenterconsole")

# Distrib-specific
set(CPACK_DEBIAN_PACKAGE_HOMEPAGE "http://gitorious.org/qpdfpresenterconsole")
set(CPACK_DEBIAN_PACKAGE_SECTION "misc")
set(CPACK_DEBIAN_PACKAGE_SHLIBDEPS ON)
set(CPACK_DEBIAN_PACKAGE_DESCRIPTION
    "${PROJECT_NAME} is a Beamer-oriented presenting tool.
 QPdfPresenterConsole is a software inspired by both Apple's Keynote and
 Pdf Presenter Console [http://westhoffswelt.de/projects/pdf_presenter_console.html]
 written using Qt4. Thus, it is easy to use, extend and port.
 Currently, it runs smoothly on any Linux which has XRandR capabilities, and
 Windows 7 at least.")

# Source release
set(PROJECT_SOURCE_BASENAME "${PROJECT_SHORTNAME}")
set(PROJECT_SOURCE_VERSION "${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}.${CPACK_PACKAGE_VERSION_PATCH}")
if (PROJECT_SOURCE_TAG)
    set(PROJECT_SOURCE_TAG "${PROJECT_SOURCE_TAG}")
    set(PROJECT_SOURCE_ORIG "${PROJECT_SOURCE_TAG}")
else (PROJECT_SOURCE_TAG)
    set(PROJECT_SOURCE_TAG "v${PROJECT_SOURCE_VERSION}")
    set(PROJECT_SOURCE_ORIG "${PROJECT_SOURCE_VERSION}")
endif (PROJECT_SOURCE_TAG)
set(PROJECT_SOURCE_PREFIX "${PROJECT_SOURCE_BASENAME}-${PROJECT_SOURCE_TAG}")
set(PROJECT_SOURCE_TARGETNAME "${PROJECT_SOURCE_BASENAME}-${PROJECT_SOURCE_TAG}.tar.bz2")
set(PROJECT_SOURCE_DEBORIG "${PROJECT_SOURCE_BASENAME}_${PROJECT_SOURCE_ORIG}.orig.tar.bz2")

message(STATUS "Compilation profile: " ${CMAKE_BUILD_TYPE})

if(CMAKE_CROSSCOMPILING)
    message(STATUS "Cross-compiling for " ${CMAKE_SYSTEM_NAME})
endif(CMAKE_CROSSCOMPILING)
if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    set(PROJECT_DATADIR "share")
    set(GUI_TYPE WIN32)
    set(DEFAULT_OPTION_MAINSCREEN 0)
    set(DEFAULT_OPTION_PROJECTORSCREEN 1)
    set(WINOTHERDEPS "${CMAKE_SOURCE_DIR}/win32-dependencies/")
    set(CPACK_GENERATOR "${CPACK_GENERATOR};NSIS;ZIP")
    set(CPACK_NSIS_URL_INFO_ABOUT "http://gitorious.org/qpdfpresenterconsole")
    set(CPACK_PACKAGE_EXECUTABLES "${PROJECT_TARGETBIN}" "QPdfPresenterConsole")
    set(CPACK_NSIS_CREATE_ICONS_EXTRA
        "CreateShortCut \\\"$SMPROGRAMS\\\\$STARTMENU_FOLDER\\\\Documentation.lnk\\\" \\\"$INSTDIR\\\\share\\\\${htmldoc}\\\"
        CreateShortCut \\\"$SMPROGRAMS\\\\$STARTMENU_FOLDER\\\\BeamerNotes.lnk\\\" \\\"$INSTDIR\\\\bin\\\\${PROJECT_TARGETBIN}.exe\\\" \\\"-n 1 -l left\\\" \\\"$INSTDIR\\\\share\\\\examples\\\\beamernotes\\\\beamer.left.pdf\\\"
        CreateShortCut \\\"$SMPROGRAMS\\\\$STARTMENU_FOLDER\\\\TextAnnot.lnk\\\" \\\"$INSTDIR\\\\bin\\\\${PROJECT_TARGETBIN}.exe\\\" \\\"-t\\\" \\\"$INSTDIR\\\\share\\\\examples\\\\textannot\\\\beamer.pdf\\\"
    ")
    set(CPACK_NSIS_DELETE_ICONS_EXTRA
        "!insertmacro MUI_STARTMENU_GETFOLDER Application $MUI_TEMP
    Delete \\\"$SMPROGRAMS\\\\$MUI_TEMP\\\\Documentation.lnk\\\"
        Delete \\\"$SMPROGRAMS\\\\$MUI_TEMP\\\\BeamerNotes.lnk\\\"
        Delete \\\"$SMPROGRAMS\\\\$MUI_TEMP\\\\TextAnnot.lnk\\\"
    ")
endif(${CMAKE_SYSTEM_NAME} MATCHES "Windows")

if(UNIX AND NOT APPLE)
    if(NO_INSTALL_PREFIX)
        message(WARNING "\n!!!\nYou did not set any CMAKE_INSTALL_PREFIX for your Unix binaries. Your installation will be done with CMAKE_INSTALL_PREFIX=${CMAKE_INSTALL_PREFIX}\n!!!")
    endif(NO_INSTALL_PREFIX)
    set(DEFAULT_OPTION_MAINSCREEN 1)
    set(DEFAULT_OPTION_PROJECTORSCREEN 2)
    set(CPACK_STRIP_FILES "${PROJECT_TARGETBIN}")
    set(CPACK_SYSTEM_NAME "${CMAKE_SYSTEM_NAME}-${CMAKE_SYSTEM_PROCESSOR}")
endif(UNIX AND NOT APPLE)

if(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    set(CPACK_GENERATOR "${CPACK_GENERATOR};DEB;RPM")
endif(${CMAKE_SYSTEM_NAME} MATCHES "Linux")

if(APPLE)
    if(NOT VLC_PATH)
        message(FATAL_ERROR "No VLC_PATH given, cannot continue. Please -DVLC_PATH=/path/to/VLC.app")
    endif(NOT VLC_PATH)
    set(VLC_DIR "${VLC_PATH}/Contents/MacOS")
    set(LIBVLC_INCLUDE_DIR "${VLC_DIR}/include/")
    set(LIBVLC_LIBRARY "${VLC_DIR}/lib/libvlc.5.dylib")
    set(LIBVLCCORE_LIBRARY "${VLC_DIR}/lib/libvlccore.5.dylib")
    set(PROJECT_TARGETBIN "${PROJECT_FULLNAME}")
    set(EXECUTABLE "${PROJECT_TARGETBIN}.app")
    set(PROJECT_CONTENTS "${EXECUTABLE}/Contents/")
    set(PROJECT_RESOURCES "${PROJECT_CONTENTS}/Resources/")
    set(PROJECT_DATADIR "${PROJECT_RESOURCES}")
    set(MACOSX_ICON "${PROJECT_FULLNAME}.icns")
    set(MACOSX_ICON_FILE "${MACOSX_ICON}")
    set(CMAKE_OSX_ARCHITECTURES "x86_64")
    set(OSX_BUNDLE MACOSX_BUNDLE)
    set(DEFAULT_OPTION_MAINSCREEN 0)
    set(DEFAULT_OPTION_PROJECTORSCREEN 1)
    set(CPACK_GENERATOR "${CPACK_GENERATOR};DragNDrop")
    set(CPACK_PACKAGE_ICON "${CMAKE_SOURCE_DIR}/${MACOSX_ICON_FILE}")
    set(MACOSX_BUNDLE_EXECUTABLE_NAME "${PROJECT_TARGETBIN}")
    set(MACOSX_BUNDLE_BUNDLE_NAME "${PROJECT_FULLNAME}")
    set(MACOSX_BUNDLE_COPYRIGHT "MIT License")
    set(MACOSX_BUNDLE_INFO_STRING "${PROJECT_FULLNAME} ${PROJECT_VERSION}")
    set(MACOSX_BUNDLE_ICON_FILE "${MACOSX_ICON}")
    set(MACOSX_BUNDLE_BUNDLE_VERSION "${PROJECT_VERSION}")
    set(MACOSX_BUNDLE_SHORT_VERSION_STRING "${PROJECT_VERSION}")
    set(MACOSX_BUNDLE_LONG_VERSION_STRING "${PROJECT_VERSION} ${CMAKE_SYSTEM_NAME} ${CMAKE_BUILD_TYPE}")
    set(MACOSX_BUNDLE_GUI_IDENTIFIER "com.qpdfpresenterconsole.qpdfpresenterconsole")
    set_source_files_properties(${MACOSX_ICON_FILE} PROPERTIES MACOSX_PACKAGE_LOCATION Resources)
endif(APPLE)

find_package(Qt4 COMPONENTS QtCore QtGui QtWebKit REQUIRED)
find_package(Qt4 COMPONENTS QtDBus)
find_package(PopplerQt4 REQUIRED)
find_package(Asciidoc REQUIRED)
find_package(LIBVLC REQUIRED)

if(APPLE)
    if(NOT QT_MAC_USE_COCOA)
        message(WARNING "Qt does not use Cocoa. Unknown results, you have been warned.")
    endif(NOT QT_MAC_USE_COCOA)
    # CoreServices framework is needed for OSX's Screen Saver Inhibiter
    find_package(OSXCoreServices REQUIRED)
endif(APPLE)

include(CPack)
include(InstallRequiredSystemLibraries)

if(ASCIIDOC_FOUND)
    message(STATUS "Found asciidoc: " ${ASCIIDOC_EXECUTABLE})
    message(STATUS "Found a2x: " ${ASCIIDOC_A2X_EXECUTABLE})
endif(ASCIIDOC_FOUND)

if(QT_QTDBUS_FOUND)
    set(HAVE_DBUS 1)
    message(STATUS "QtDBus ready platform, using.")
else(QT_QTDBUS_FOUND)
    message(STATUS "Not QtDBus ready platform, disabling.")
endif(QT_QTDBUS_FOUND)

if(PROJECT_STRIP_BIN)
    message(STATUS "Using Stripper: " ${CMAKE_STRIP})
endif(PROJECT_STRIP_BIN)

if(PROJECT_PACK_BIN)
    include(FindSelfPackers)
    message(STATUS "Using Packer for binaries: " ${SELF_PACKER_FOR_EXECUTABLE})
    message(STATUS "Using Packer for librairies: " ${SELF_PACKER_FOR_SHARED_LIB})
endif(PROJECT_PACK_BIN)

if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    set(PROJECT_BASE_WINDEPS
        "${WINQTDIR}/bin/mingwm10.dll"
        "${WINQTDIR}/bin/libgcc_s_dw2-1.dll"
    )
    set(PROJECT_QT4_WINDEPS
        "${WINQTDIR}/bin/QtCore4.dll"
        "${WINQTDIR}/bin/QtGui4.dll"
        "${WINQTDIR}/bin/QtSvg4.dll"
        "${WINQTDIR}/bin/QtWebKit4.dll"
        "${WINQTDIR}/bin/QtNetwork4.dll"
        "${WINQTDIR}/bin/phonon4.dll"
    )
    set(PROJECT_POPPLER_QT4_WINDEPS
        "${WINQTDIR}/bin/QtXml4.dll"
        "${WINPOPPLERDIR}/bin/libpoppler.dll"
        "${WINPOPPLERDIR}/bin/libpoppler-cpp.dll"
        "${WINPOPPLERDIR}/bin/libpoppler-qt4.dll"
        "${WINOTHERDEPS}/libtiff3.dll"
# for libpoppler-qt4.dll 0.18.1 from winkde, extracted from mingw32 gcc 4.4.0 fedora 11 package
        "${WINOTHERDEPS}/libgcc_s_sjlj-1.dll"
    )
    set(PROJECT_POPPLER_QT4_DEPS_WINDEPS
        "${WINOTHERDEPS}/libfreetype.dll"
        "${WINOTHERDEPS}/libiconv.dll"
        "${WINOTHERDEPS}/libjpeg.dll"
        "${WINOTHERDEPS}/liblcms-1.dll"
        "${WINOTHERDEPS}/libopenjpeg.dll"
        "${WINOTHERDEPS}/libpng14.dll"
        "${WINOTHERDEPS}/libxml2.dll"
        "${WINOTHERDEPS}/libzlib1.dll"
# libz.dll == libzlib1.dll
        "${WINOTHERDEPS}/libz.dll"
    )
    set(PROJECT_LIBVLC_WINDEPS
        "${WINLIBVLCDIR}/libvlc.dll"
        "${WINLIBVLCDIR}/libvlccore.dll"
    )
    set(PROJECT_LIBVLC_PLUGINS_WINDEPS
        "${WINLIBVLCDIR}/plugins/"
    )
    set(PROJECT_WINDEPS
        ${PROJECT_BASE_WINDEPS}
        ${PROJECT_QT4_WINDEPS}
        ${PROJECT_POPPLER_QT4_WINDEPS}
        ${PROJECT_POPPLER_QT4_DEPS_WINDEPS}
        ${PROJECT_LIBVLC_WINDEPS}
    )
endif(${CMAKE_SYSTEM_NAME} MATCHES "Windows")

if(APPLE)
    set(PROJECT_QT_MENU_NIB_APPLEDEPS
        "${QT_LIBRARY_DIR}/Resources/qt_menu.nib/"
    )
    set(PROJECT_LIBVLC_APPLEDEPS
        "${LIBVLC_LIBRARY}"
    )
    set(PROJECT_LIBVLCCORE_APPLEDEPS
        "${LIBVLCCORE_LIBRARY}"
    )
    set(PROJECT_LIBVLC_PLUGINS_APPLEDEPS
        "${VLC_DIR}/plugins/"
    )
##    set(BUNDLE_SEARCH_PATH
##         "/opt/local/lib"
##    )
endif(APPLE)

include(${QT_USE_FILE})

include_directories(
    ${QT_INCLUDE_DIR}
    ${POPPLER_QT4_INCLUDE_DIR}
    ${LIBVLC_INCLUDE_DIR}
    ${CMAKE_BINARY_DIR}
)

file(   GLOB
        CPP_SOURCE_FILES
        *.cpp
)

file(   GLOB
        H_SOURCE_FILES
        *.h
)

file(   GLOB
        TRANSLATION_FILES
        *.ts
)

file(   GLOB
        UIC_SOURCE_FILES
        *.ui
)

file(   GLOB
        BEAMERNOTES
        beamer.tex
        beamer.left.pdf
        beamer.right.pdf
)

file(   GLOB
        TEXTANNOT
        beamer.pdf
        beamer.annot
)

file(   GLOB
        DESKTOP
        ${PROJECT_FULLNAME}.desktop
)

file(   GLOB
        ICON
        ${PROJECT_FULLNAME}.png
)

set(    DOCUMENTATION_FILES
        ${CMAKE_BINARY_DIR}/${htmldoc}
        ${CMAKE_BINARY_DIR}/${htmlcss}
)

configure_file(
    ${CMAKE_SOURCE_DIR}/app.h.in ${CMAKE_BINARY_DIR}/app.h
)

if(APPLE)
    configure_file(${CMAKE_SOURCE_DIR}/MacOSXBundleInfo.plist.in ${CMAKE_BINARY_DIR}/Info.plist)
endif(APPLE)

# Define platform-specific files
if(NOT APPLE)
    file(GLOB OSX_H_SOURCE_FILES "osxinhibiter.h")
    file(GLOB OSX_CPP_SOURCE_FILES "osxinhibiter.cpp")
endif(NOT APPLE)

if(NOT WIN32)
    file(GLOB WINDOWS_H_SOURCE_FILES "windowsinhibiter.h")
    file(GLOB WINDOWS_CPP_SOURCE_FILES "windowsinhibiter.cpp")
endif(NOT WIN32)

if((NOT HAVE_DBUS) OR APPLE)
    file(GLOB FREEDESKTOP_H_SOURCE_FILES "freedesktopinhibiter.h")
    file(GLOB FREEDESKTOP_CPP_SOURCE_FILES "freedesktopinhibiter.cpp")
endif((NOT HAVE_DBUS) OR APPLE)

if((NOT UNIX) OR APPLE)
    file(GLOB XDG_H_SOURCE_FILES "xdginhibiter.h")
    file(GLOB XDG_CPP_SOURCE_FILES "xdginhibiter.cpp")
endif((NOT UNIX) OR APPLE)

list(APPEND H_SOURCE_FILES_TO_REMOVE
    ${OSX_H_SOURCE_FILES}
    ${WINDOWS_H_SOURCE_FILES}
    ${FREEDESKTOP_H_SOURCE_FILES}
    ${XDG_H_SOURCE_FILES}
)
list(APPEND CPP_SOURCE_FILES_TO_REMOVE
    ${OSX_CPP_SOURCE_FILES}
    ${WINDOWS_CPP_SOURCE_FILES}
    ${FREEDESKTOP_CPP_SOURCE_FILES}
    ${XDG_CPP_SOURCE_FILES}
)

message(STATUS "Removed the following h files: ${H_SOURCE_FILES_TO_REMOVE}")
message(STATUS "Removed the following cpp files: ${CPP_SOURCE_FILES_TO_REMOVE}")

# Remove platform-specific stuff
list(REMOVE_ITEM H_SOURCE_FILES ${H_SOURCE_FILES_TO_REMOVE})
list(REMOVE_ITEM CPP_SOURCE_FILES ${CPP_SOURCE_FILES_TO_REMOVE})

QT4_WRAP_CPP(MOC ${H_SOURCE_FILES})
QT4_WRAP_UI(UIC ${UIC_SOURCE_FILES})

QT4_ADD_TRANSLATION(QM ${TRANSLATION_FILES})

add_executable(${PROJECT_TARGETBIN} ${GUI_TYPE} ${OSX_BUNDLE}
    ${CPP_SOURCE_FILES}
    ${MOC}
    ${UIC}
    ${QM}
    ${MACOSX_ICON_FILE}
)

target_link_libraries(
    ${PROJECT_TARGETBIN}
    ${QT_LIBRARIES}
    ${POPPLER_QT4_LIBRARIES}
    ${LIBVLC_LIBRARY}
    ${OSX_CORE_SERVICES_LIBRARY}
)

add_custom_target(dist
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
    COMMAND git archive --format=tar --prefix=${PROJECT_SOURCE_PREFIX}/ ${PROJECT_SOURCE_TAG} | bzip2 > ${CMAKE_BINARY_DIR}/${PROJECT_SOURCE_TARGETNAME}
)

add_custom_target(deborig
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
    COMMAND git archive --format=tar --prefix=${PROJECT_SOURCE_PREFIX}/ ${PROJECT_SOURCE_TAG} | bzip2 > ${CMAKE_BINARY_DIR}/${PROJECT_SOURCE_DEBORIG}
)

add_custom_target(prepare_debuild
    DEPENDS deborig
    COMMAND cd ${CMAKE_BINARY_DIR} && tar xf ${PROJECT_SOURCE_DEBORIG}
)

add_custom_target(debuild
    DEPENDS prepare_debuild
    COMMAND cd ${CMAKE_BINARY_DIR}/${PROJECT_SOURCE_PREFIX} && debuild
)

if(ASCIIDOC_FOUND)
    add_custom_target(${htmldoc} COMMAND ${ASCIIDOC_A2X_EXECUTABLE} -d manpage -f xhtml --destination-dir=${CMAKE_BINARY_DIR} ${inputdoc})
    add_custom_target(${mandoc} COMMAND ${ASCIIDOC_A2X_EXECUTABLE} -d manpage -f manpage --destination-dir=${CMAKE_BINARY_DIR} ${inputdoc})
    add_dependencies(${PROJECT_TARGETBIN} ${htmldoc} ${mandoc})
endif(ASCIIDOC_FOUND)

install(TARGETS ${PROJECT_TARGETBIN} RUNTIME DESTINATION bin BUNDLE DESTINATION . )
install(FILES ${QM} ${DOCUMENTATION_FILES} DESTINATION ${PROJECT_DATADIR})
install(FILES ${BEAMERNOTES} DESTINATION ${PROJECT_DATADIR}/examples/beamernotes/)
install(FILES ${TEXTANNOT} DESTINATION ${PROJECT_DATADIR}/examples/textannot/)
if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    install(FILES ${PROJECT_WINDEPS} DESTINATION bin)
    install(DIRECTORY ${PROJECT_LIBVLC_PLUGINS_WINDEPS} DESTINATION bin/plugins/)
endif(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
if(APPLE)
    install(FILES ${MACOSX_ICON_FILE} DESTINATION ${PROJECT_RESOURCES})
    install(FILES ${CMAKE_BINARY_DIR}/Info.plist DESTINATION ${PROJECT_CONTENTS})
    install(FILES ${PROJECT_LIBVLC_APPLEDEPS} ${PROJECT_LIBVLCCORE_APPLEDEPS} DESTINATION ${PROJECT_CONTENTS}/MacOS)
    install(DIRECTORY ${PROJECT_QT_MENU_NIB_APPLEDEPS} DESTINATION ${PROJECT_RESOURCES}/qt_menu.nib)

    # install a second time, libvlc plugins are not fixed up
    install(FILES ${PROJECT_LIBVLC_APPLEDEPS} ${PROJECT_LIBVLCCORE_APPLEDEPS} DESTINATION ${PROJECT_CONTENTS}/lib)
    install(DIRECTORY ${PROJECT_LIBVLC_PLUGINS_APPLEDEPS} DESTINATION ${PROJECT_CONTENTS}/plugins)

    include(DeployQt4)
    install_qt4_executable("${EXECUTABLE}")
    # needed for libvlc fixup
    install(CODE "
        include(BundleUtilities)
        fixup_bundle(\"\$ENV{DESTDIR}\${CMAKE_INSTALL_PREFIX}/${EXECUTABLE}\" \"\" \"${BUNDLE_SEARCH_PATH}\")
")
endif(APPLE)
if(UNIX AND NOT APPLE)
    install(FILES ${CMAKE_BINARY_DIR}/${mandoc} DESTINATION share/man/man1)
    install(FILES ${DESKTOP} DESTINATION share/applications)
    install(FILES ${ICON} DESTINATION share/pixmaps)
endif(UNIX AND NOT APPLE)

if(PROJECT_STRIP_BIN)
    if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
        install(CODE
"file(GLOB FILES_TO_STRIP \"\$ENV{DESTDIR}\${CMAKE_INSTALL_PREFIX}/bin/*.exe\" \"\$ENV{DESTDIR}\${CMAKE_INSTALL_PREFIX}/bin/*.dll\")
message(STATUS \"Stripping with ${CMAKE_STRIP}\")
execute_process(COMMAND ${CMAKE_STRIP} \${FILES_TO_STRIP})")
    endif(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
endif(PROJECT_STRIP_BIN)

if(PROJECT_PACK_BIN)
    if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
        install(CODE
"file(GLOB BINS_TO_PACK \"\$ENV{DESTDIR}\${CMAKE_INSTALL_PREFIX}/bin/*.exe\")
file(GLOB DEPS_TO_PACK \"\$ENV{DESTDIR}\${CMAKE_INSTALL_PREFIX}/bin/*.dll\")
file(GLOB DEPS_NO_PACK \"\$ENV{DESTDIR}\${CMAKE_INSTALL_PREFIX}/bin/libgcc_s_dw2-1.dll\")
message(STATUS \"NOT packed libraries: \${DEPS_NO_PACK}\")
list(REMOVE_ITEM DEPS_TO_PACK \${DEPS_NO_PACK})
message(STATUS \"Packing binaries with ${SELF_PACKER_FOR_EXECUTABLE}\")
execute_process(COMMAND ${SELF_PACKER_FOR_EXECUTABLE} ${SELF_PACKER_FOR_EXECUTABLE_FLAGS} -qq \${BINS_TO_PACK})
message(STATUS \"Packing libraries with ${SELF_PACKER_FOR_SHARED_LIB}\")
execute_process(COMMAND ${SELF_PACKER_FOR_SHARED_LIB} ${SELF_PACKER_FOR_SHARED_LIB_FLAGS} -qq \${DEPS_TO_PACK})
")
    endif(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
endif(PROJECT_PACK_BIN)
