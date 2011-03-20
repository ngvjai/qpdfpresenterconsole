cmake_minimum_required(VERSION 2.6)

project(QPdfPresenterConsole)
set(CMAKE_MODULE_PATH
	"${PROJECT_SOURCE_DIR}/cmake"
	"${CMAKE_MODULE_PATH}"
)

set(PROJECT_TARGETBIN "QPdfPresenterConsole")
set(PROJECT_SHORTNAME "qpdfpresenterconsole")
set(PROJECT_VERSION_MAJOR "1")
set(PROJECT_VERSION_MINOR "0")
set(PROJECT_VERSION_PATCH "0")
set(PROJECT_VERSION "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}")
set(PROJECT_DATADIR "${CMAKE_INSTALL_PREFIX}/share/${PROJECT_SHORTNAME}")

set(DEFAULT_OPTION_DURATION 20 CACHE STRING "Default presentation length (min)")
set(DEFAULT_OPTION_EMERGENCY 2 CACHE STRING "Default emergency time (min)")
set(DEFAULT_OPTION_SLIDESWIDTH 55 CACHE STRING "Default main slide width (prcent)")
set(DEFAULT_OPTION_PAGE 1 CACHE STRING "Default first page opened")
set(DEFAULT_OPTION_MAINSCREEN 1 CACHE STRING "Default main screen identifier")
set(DEFAULT_OPTION_PROJECTORSCREEN 2 CACHE STRING "Default secondary screen identifier")

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

if(CMAKE_CROSSCOMPILING)
    message(STATUS "Cross-compiling for " ${CMAKE_SYSTEM_NAME})
endif(CMAKE_CROSSCOMPILING)
if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    set(PROJECT_DATADIR "share")
    set(DEFAULT_OPTION_MAINSCREEN 0)
    set(DEFAULT_OPTION_PROJECTORSCREEN 1)
    set(WINOTHERDEPS "${CMAKE_SOURCE_DIR}/win32-dependencies/")
    set(CPACK_GENERATOR "${CPACK_GENERATOR};NSIS;ZIP")
    set(CPACK_NSIS_URL_INFO_ABOUT "http://gitorious.org/qpdfpresenterconsole")
    set(CPACK_PACKAGE_EXECUTABLES "${PROJECT_TARGETBIN}" "QPdfPresenterConsole")
endif(${CMAKE_SYSTEM_NAME} MATCHES "Windows")

if(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    set(DEFAULT_OPTION_MAINSCREEN 1)
    set(DEFAULT_OPTION_PROJECTORSCREEN 2)
    set(CPACK_GENERATOR "${CPACK_GENERATOR};DEB;RPM")
endif(${CMAKE_SYSTEM_NAME} MATCHES "Linux")

find_package(Qt4 COMPONENTS QtCore QtGui REQUIRED)
find_package(PopplerQt4 REQUIRED)

include(CPack)

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

configure_file(
	${CMAKE_SOURCE_DIR}/app.h.in ${CMAKE_BINARY_DIR}/app.h
)

QT4_WRAP_CPP(MOC ${H_SOURCE_FILES})
QT4_WRAP_UI(UIC ${UIC_SOURCE_FILES})

QT4_ADD_TRANSLATION(QM ${TRANSLATION_FILES})

add_executable(${PROJECT_TARGETBIN}
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

install(FILES ${QM} DESTINATION ${PROJECT_DATADIR})
install(TARGETS ${PROJECT_TARGETBIN} RUNTIME DESTINATION bin)
if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    install(FILES ${PROJECT_WINDEPS} DESTINATION bin)
endif(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
