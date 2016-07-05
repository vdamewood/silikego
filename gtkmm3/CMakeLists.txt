# CMakeLists.txt: Build Script
# Copyright 2012, 2014, 2015, 2016 Vincent Damewood
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program. If not, see <http://www.gnu.org/licenses/>.

set(TARGET SilikegoGui)
set(HEADERS EvalWindow.hh)
set(SOURCES Main.cc EvalWindow.cc)
set(LAYOUTS EvalWindow.glade)

set(DEPS SilikegoLib)
message("-- Finding GTKmm 2")
find_package(GTK2 QUIET COMPONENTS gtk gtkmm)
if(GTK2_FOUND)
  message("-- Finding GTKmm 2 - found")
  list(APPEND DEPS ${GTK2_LIBRARIES})
  include_directories(${GTK2_INCLUDE_DIRS})
else()
  message("-- Finding GTKmm 2 - not found")
endif()

find_package(PkgConfig REQUIRED)
pkg_check_modules(GTKMM3 REQUIRED gtkmm-3.0)
list(APPEND LIBS ${GTKMM3_LIBRARIES})
include_directories(${GTKMM3_INCLUDE_DIRS})
link_directories(${GTKMM3_LIBRARY_DIRS})
add_definitions(${GTKMM3_CFLAGS_OTHER})

add_definitions("-DGLADEDIR=\"${CMAKE_INSTALL_PREFIX}/${INSTDAT}\"")

add_executable(${TARGET} ${SOURCES} ${HEADERS})
set_target_properties(${TARGET} PROPERTIES
	OUTPUT_NAME ${ONAMEGUI})
target_link_libraries(${TARGET} ${DEPS})

install(TARGETS ${TARGET}
  RUNTIME    DESTINATION ${INSTBIN})

install(
  FILES ${LAYOUTS}
  DESTINATION ${INSTDAT})
