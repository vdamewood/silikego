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
set(HEADERS EvalWindow.h About.h Id.h)
set(SOURCES App.cc EvalWindow.cc EvalWindow.rc About.cc About.rc Icon.rc)
set(DEPS ${LIB})

message("-- Finding MFC Framework")
find_package(MFC QUIET)
if(MFC_FOUND)
  message("-- Finding MFC Framework - found")
  ADD_DEFINITIONS(-D_AFXDLL -D_WIN32_WINNT=NTDDI_WIN7)
  SET(CMAKE_MFC_FLAG 2)
else()
  message("-- Finding MFC Framework - not found")
endif()

if(NOT MFC_FOUND)
  message(FATAL_ERROR "Can't build GUI. Quitting.")
endif()

add_executable(${TARGET} WIN32 ${HEADERS} ${SOURCES})
set_target_properties(${TARGET} PROPERTIES
	OUTPUT_NAME ${ONAMEGUI})
target_link_libraries(${TARGET} ${DEPS})

add_custom_command(
  TARGET ${TARGET} POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_if_different
  $<TARGET_FILE:SilikegoDLib>
  ${CMAKE_CURRENT_BINARY_DIR})
add_custom_command(
  TARGET ${TARGET} POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_if_different
  $<TARGET_PDB_FILE:SilikegoDLib>
  ${CMAKE_CURRENT_BINARY_DIR})

install(TARGETS ${TARGET}
  RUNTIME DESTINATION ${INSTBIN})
