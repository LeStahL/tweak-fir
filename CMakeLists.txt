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

find_package(Qt5 COMPONENTS Core Widgets Gui)

set(tweak_SRC
    tweak.cpp
    MainWindow.cpp
)

add_executable(tweak-fir ${tweak_SRC})
target_link_libraries(tweak-fir Qt5::Widgets Qt5::Core Qt5::Gui)
