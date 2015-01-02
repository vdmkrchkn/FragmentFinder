cmake_minimum_required(VERSION 2.8.11)
#
project(findRectObjects)
#
find_package(Qt5 REQUIRED Widgets LinguistTools)
find_package(OpenCV REQUIRED)
#
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(
	CPP_FILES
	main.cpp	
	mainwindow.cpp
	worker-thread.cpp)
set(
	H_FILES
	mainwindow.h
	worker-thread.h)
set(
	TS_FILES
	translations/findRectObjects_ru_RU.ts)	
set(UI_FILES mainwindow.ui)
# Translations
list(
	APPEND TRANSLATE_FILES
	${CPP_FILES}
	${H_FILES}
	${UI_FILES})	
#	
option(UPDATE_TRANSLATIONS_WITH_DANGER
	"Update source translation (files translations/*.ts). "
	"WARNING: ’make clean’ will delete them, turn off ASAP, danger!")
#
if(UPDATE_TRANSLATIONS_WITH_DANGER)
	message(
		WARNING
		"UPDATE_TRANSLATIONS_WITH_DANGER option is set, turn it off "
		"after generating translation files as soon as possible!")
	foreach(src_name IN LISTS TRANSLATE_FILES)
		get_filename_component(src_path ${src_name} ABSOLUTE)
		list(APPEND TRANSLATE_PATHS ${src_path})
	endforeach()
	qt5_create_translation(QM_FILES ${TRANSLATE_PATHS} ${TS_FILES})	
else()
	qt5_add_translation(QM_FILES ${TS_FILES})
endif()
#
add_custom_target(translations DEPENDS ${QM_FILES})
#
qt5_wrap_cpp(MOC_WRAPPERS ${H_FILES})
qt5_wrap_ui(UIC_WRAPPERS ${UI_FILES})
#  
qt5_add_resources(
	QRC_WRAPPERS
	findRectObjects.qrc
	OPTIONS -no-compress)  
#
if(WIN32)
  set(gui_type WIN32)
endif()
#
add_executable(
  findRectObjects ${gui_type}
  ${H_FILES}
  ${CPP_FILES}
  ${MOC_WRAPPERS}
  ${UIC_WRAPPERS}
  ${QRC_WRAPPERS})
#
add_dependencies(findRectObjects translations)
target_link_libraries(findRectObjects Qt5::Widgets ${OpenCV_LIBS})
# Installation
install(TARGETS findRectObjects RUNTIME DESTINATION bin)
install(FILES ${QM_FILES} DESTINATION translations)
#
include(DeployQt4)
get_property(EXE_PATH TARGET findRectObjects PROPERTY LOCATION)
#
if(EXE_PATH)
	get_filename_component(EXE_NAME ${EXE_PATH} NAME)	
	install_qt4_executable(bin/${EXE_NAME})
endif()