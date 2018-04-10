# tweak-fir - QT tool to tweak shader filter parameters for sound rendering
#
# Copyright (C) 2018  Alexander Kraus <nr4@z10.info>
# 
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
# 
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.
#

cmake_minimum_required(VERSION 2.8)

project(tweak-fir)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)

if(WIN32)
	include_directories("C:\\Qt\\5.10.1\\msvc2017_64\\include")
	add_custom_target(uic 
		COMMAND "C:\\Qt\\5.10.1\\msvc2017_64\\bin\\uic.exe" ${PROJECT_SOURCE_DIR}/MainWindow.ui -o ${PROJECT_BINARY_DIR}/ui_MainWindow.h 
		COMMAND "C:\\Qt\\5.10.1\\msvc2017_64\\bin\\uic.exe" ${PROJECT_SOURCE_DIR}/ParamEdit.ui -o ${PROJECT_BINARY_DIR}/ui_ParamEdit.h
	)
	find_library(QTCORE_LIB NAMES Qt5Core.lib HINTS "C:\\Qt\\5.10.1\\msvc2017_64\\lib\\")
	find_library(QTWIDGETS_LIB NAMES Qt5Widgets.lib HINTS "C:\\Qt\\5.10.1\\msvc2017_64\\lib\\")
	find_library(QTGUI_LIB NAMES Qt5Gui.lib HINTS "C:\\Qt\\5.10.1\\msvc2017_64\\lib\\")
	set(QT_LIBS 
		${QTCORE_LIB} 
		${QTWIDGETS_LIB} 
		${QTGUI_LIB}
	)
	add_custom_target(moc
		COMMAND "C:\\Qt\\5.10.1\\msvc2017_64\\bin\\moc.exe" ${PROJECT_SOURCE_DIR}/MainWindow.h -o ${PROJECT_BINARY_DIR}/moc_MainWindow.cpp
		COMMAND "C:\\Qt\\5.10.1\\msvc2017_64\\bin\\moc.exe" ${PROJECT_SOURCE_DIR}/ParamEdit.h -o ${PROJECT_BINARY_DIR}/moc_ParamEdit.cpp
	)
	set(MOC_SRC
		${PROJECT_BINARY_DIR}/moc_MainWindow.cpp
		${PROJECT_BINARY_DIR}/moc_ParamEdit.cpp
	)
elseif(UNIX)
	find_package(Qt5 COMPONENTS Core Widgets Gui)
endif()

set(tweak_SRC
    tweak.cpp
    MainWindow.cpp
    ParamEdit.cpp
#     RenderSoundWidget.cpp
)

if(WIN32)
	add_executable(tweak-fir ${tweak_SRC} ${MOC_SRC})
	add_dependencies(tweak-fir uic moc)
	target_link_libraries(tweak-fir ${QT_LIBS})
elseif(UNIX)
	add_executable(tweak-fir ${tweak_SRC})
	target_link_libraries(tweak-fir Qt5::Widgets Qt5::Core Qt5::Gui)
endif()

