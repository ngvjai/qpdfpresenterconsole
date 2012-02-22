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
set(PROJECT_VERSION_MINOR "3")
set(PROJECT_VERSION_PATCH "0")
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
    set(CMAKE_BUILD_TYPE "Debug")
endif(PROJECT_DEBUG)

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

if(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    if(NO_INSTALL_PREFIX)
        message(WARNING "\n!!!\nYou did not set any CMAKE_INSTALL_PREFIX for your Linux binaries. Your installation will be done with CMAKE_INSTALL_PREFIX=${CMAKE_INSTALL_PREFIX}\n!!!")
    endif(NO_INSTALL_PREFIX)
    set(DEFAULT_OPTION_MAINSCREEN 1)
    set(DEFAULT_OPTION_PROJECTORSCREEN 2)
    set(CPACK_SYSTEM_NAME "${CMAKE_SYSTEM_NAME}-${CMAKE_SYSTEM_PROCESSOR}")
    set(CPACK_GENERATOR "${CPACK_GENERATOR};DEB;RPM")
    set(CPACK_STRIP_FILES "${PROJECT_TARGETBIN}")
endif(${CMAKE_SYSTEM_NAME} MATCHES "Linux")

find_package(Qt4 COMPONENTS QtCore QtGui REQUIRED)
find_package(Qt4 COMPONENTS QtDBus)
find_package(PopplerQt4 REQUIRED)
find_package(Asciidoc REQUIRED)

include(CPack)

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
    )
    set(PROJECT_POPPLER_QT4_WINDEPS
        "${WINQTDIR}/bin/QtXml4.dll"
        "${WINPOPPLERDIR}/bin/libpoppler.dll"
        "${WINPOPPLERDIR}/bin/libpoppler-cpp.dll"
        "${WINPOPPLERDIR}/bin/libpoppler-qt4.dll"
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
    )
    set(PROJECT_WINDEPS
        ${PROJECT_BASE_WINDEPS}
        ${PROJECT_QT4_WINDEPS}
        ${PROJECT_POPPLER_QT4_WINDEPS}
        ${PROJECT_POPPLER_QT4_DEPS_WINDEPS}
    )
endif(${CMAKE_SYSTEM_NAME} MATCHES "Windows")

include(${QT_USE_FILE})

include_directories(
	${QT_INCLUDE_DIR}
	${POPPLER_QT4_INCLUDE_DIR}
	${CMAKE_BINARY_DIR}
)

file(	GLOB
	CPP_SOURCE_FILES
	*.cpp
)

file(	GLOB
	H_SOURCE_FILES
	*.h
)

file(	GLOB
	TRANSLATION_FILES
	*.ts
)

file(	GLOB
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

file(	GLOB
	DESKTOP
	${PROJECT_FULLNAME}.desktop
)

file(	GLOB
	ICON
	${PROJECT_FULLNAME}.png
)

configure_file(
	${CMAKE_SOURCE_DIR}/app.h.in ${CMAKE_BINARY_DIR}/app.h
)

QT4_WRAP_CPP(MOC ${H_SOURCE_FILES})
QT4_WRAP_UI(UIC ${UIC_SOURCE_FILES})

QT4_ADD_TRANSLATION(QM ${TRANSLATION_FILES})

add_executable(${PROJECT_TARGETBIN} ${GUI_TYPE}
	${CPP_SOURCE_FILES}
	${MOC}
        ${UIC}
        ${QM}
)

target_link_libraries(
        ${PROJECT_TARGETBIN}
	${QT_LIBRARIES}
        ${POPPLER_QT4_LIBRARIES}
)

if(ASCIIDOC_FOUND)
    add_custom_target(${htmldoc} COMMAND ${ASCIIDOC_A2X_EXECUTABLE} -d manpage -f xhtml --destination-dir=${CMAKE_BINARY_DIR} ${inputdoc})
    add_custom_target(${mandoc} COMMAND ${ASCIIDOC_A2X_EXECUTABLE} -d manpage -f manpage --destination-dir=${CMAKE_BINARY_DIR} ${inputdoc})
    add_dependencies(${PROJECT_TARGETBIN} ${htmldoc} ${mandoc})
endif(ASCIIDOC_FOUND)

install(FILES ${QM} DESTINATION ${PROJECT_DATADIR})
install(FILES ${BEAMERNOTES} DESTINATION ${PROJECT_DATADIR}/examples/beamernotes/)
install(FILES ${TEXTANNOT} DESTINATION ${PROJECT_DATADIR}/examples/textannot/)
install(TARGETS ${PROJECT_TARGETBIN} RUNTIME DESTINATION bin)
if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    install(FILES ${PROJECT_WINDEPS} DESTINATION bin)
    install(FILES ${CMAKE_BINARY_DIR}/${htmldoc} DESTINATION ${PROJECT_DATADIR})
    install(FILES ${CMAKE_BINARY_DIR}/${htmlcss} DESTINATION ${PROJECT_DATADIR})
else(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    install(FILES ${CMAKE_BINARY_DIR}/${mandoc} DESTINATION share/man/man1)
    install(FILES ${DESKTOP} DESTINATION share/applications)
    install(FILES ${ICON} DESTINATION share/pixmaps)
endif(${CMAKE_SYSTEM_NAME} MATCHES "Windows")

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
