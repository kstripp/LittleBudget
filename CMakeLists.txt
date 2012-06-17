#####################################################################
# Setup
#####################################################################
cmake_minimum_required( VERSION 2.6 )
project( lbt )
set (lbt_VERSION_MAJOR 1)
set (lbt_VERSION_MINOR 1)
set ( CMAKE_BUILD_TYPE Release )

list (APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)

configure_file(
	"${PROJECT_SOURCE_DIR}/config.h.in"
    "${PROJECT_BINARY_DIR}/config.h"
	)


#####################################################################
# Compiler flags
#####################################################################
add_definitions ( -Wall )

#TODO- this should probably be done with a config file
# rather than a compile option
add_definitions(-DVERSION="\"${VERSION_MAJOR}"."${VERSION_MINOR}"\")


#####################################################################
# Setup QT4
#####################################################################
find_package ( Qt4 REQUIRED )

include ( ${QT_USE_FILE} )
include_directories (
	${CMAKE_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR}
	${QT_QTCORE_INCLUDE_DIR} ${QT_QTGUI_INCLUDE_DIR} ${QT_QTCORE_INCLUDE_DIR}
	${QT_QTGUI_INCLUDE_DIR} ${QT_QT3SUPPORT_INCLUDE_DIR})

#####################################################################
# Set up QT
#####################################################################
option (USE_QT "Use qt for gui" ON )
if ( USE_QT )
	set (lbt_HDRS ${lbt_HDRS}
		src/gui/choosefile.h
		src/gui/filelistbox.h
		src/gui/mainwindow.h
		)
	set	(lbt_SRCS ${lbt_SRCS}
		src/gui/choosefile.cpp
		src/gui/filelistbox.cpp
		src/gui/main.cpp
		src/gui/mainwindow.cpp
		)
endif( USE_QT )

#####################################################################
# Header files
#####################################################################
set ( lbt_HDRS ${lbt_HDRS}
	src/core/bud.h
	src/core/budget.h
	src/core/lb.h
	src/core/lbrc.h
	src/core/report.h
	
	src/test/tester.h
	)

#####################################################################
# Source files
#####################################################################
set ( lbt_SRCS ${lbt_SRCS}
	src/core/budget.cpp
	src/core/lb.cpp
	src/core/lbrc.cpp
	src/core/main.cpp
	src/core/report.cpp
	src/core/t_bud.cpp
	src/core/t_budget.cpp
	src/core/t_lb.cpp
	src/core/t_report.cpp
	
	src/test/tester.cpp
	src/test/testmain.cpp
	)

#####################################################################
# Compile and Link
#####################################################################
add_executable ( lbt ${lbt_SRCS} )
target_link_libraries ( lbt  ${QT_QTCORE_LIBRARY} ${QT_QTGUI_LIBRARY} ${QT_QTCORE_LIBRARY} ${QT_QTGUI_LIBRARY} )
target_link_libraries (lbt ${gr_link_libs} )
