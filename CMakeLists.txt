cmake_minimum_required(VERSION 2.6)

project(QPdfPresenterConsole)
set(CMAKE_MODULE_PATH
	"${PROJECT_SOURCE_DIR}/cmake"
	"${CMAKE_MODULE_PATH}"
)

set(PROJECT_TARGETBIN "QPdfPresenterConsole")
set(PROJECT_SHORTNAME "qpdfpresenterconsole")
set(PROJECT_VERSION "1.0.0")

set(DEFAULT_OPTION_DURATION 20 CACHE STRING "Default presentation length (min)")
set(DEFAULT_OPTION_EMERGENCY 2 CACHE STRING "Default emergency time (min)")
set(DEFAULT_OPTION_SLIDESWIDTH 55 CACHE STRING "Default main slide width (prcent)")
set(DEFAULT_OPTION_PAGE 1 CACHE STRING "Default first page opened")
set(DEFAULT_OPTION_MAINSCREEN 1 CACHE STRING "Default main screen identifier")
set(DEFAULT_OPTION_PROJECTORSCREEN 2 CACHE STRING "Default secondary screen identifier")

if(CMAKE_CROSSCOMPILING)
    message(STATUS "Cross-compiling for " ${CMAKE_SYSTEM_NAME})
endif(CMAKE_CROSSCOMPILING)
if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    set(DEFAULT_OPTION_MAINSCREEN 0)
    set(DEFAULT_OPTION_PROJECTORSCREEN 1)
endif(${CMAKE_SYSTEM_NAME} MATCHES "Windows")

find_package(Qt4 COMPONENTS QtCore QtGui REQUIRED)
find_package(PopplerQt4 REQUIRED)

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

configure_file(
	${CMAKE_SOURCE_DIR}/app.h.in ${CMAKE_BINARY_DIR}/app.h
)

QT4_WRAP_CPP(MOC ${H_SOURCE_FILES})

QT4_ADD_TRANSLATION(QM ${TRANSLATION_FILES})

add_executable(${PROJECT_TARGETBIN}
	${CPP_SOURCE_FILES}
	${MOC}
	${QM}
)

target_link_libraries(
        ${PROJECT_TARGETBIN}
	${QT_LIBRARIES}
	${POPPLER_QT4_LIBRARIES}
)

install(FILES ${QM} DESTINATION share/${PROJECT_SHORTNAME}/)
install(TARGETS ${PROJECT_TARGETBIN} RUNTIME DESTINATION bin)
