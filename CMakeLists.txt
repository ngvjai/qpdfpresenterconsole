cmake_minimum_required(VERSION 2.6)

project(QPdfPresenterConsole)
set(PROJECT_VERSION "1.0.0")
set(CMAKE_MODULE_PATH
	"${PROJECT_SOURCE_DIR}/cmake"
	"${CMAKE_MODULE_PATH}"
)
set(TARGETBIN "QPdfPresenterConsole")

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

add_executable(${TARGETBIN}
	${CPP_SOURCE_FILES}
	${MOC}
	${QM}
)

target_link_libraries(
	${TARGETBIN}
	${QT_LIBRARIES}
	${POPPLER_QT4_LIBRARIES}
)

install(FILES ${QM} DESTINATION share/${PROJECT_NAME}/)
install(TARGETS ${TARGETBIN} RUNTIME DESTINATION bin)
