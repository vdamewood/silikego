# CMakeLists.txt: Build Script
# Copyright 2012, 2014, 2015, 2016, 2017, 2018 Vincent Damewood
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

find_package(PkgConfig REQUIRED)
pkg_check_modules(Gtkmm3 REQUIRED gtkmm-3.0 IMPORTED_TARGET)

find_program(GRESOURCE glib-compile-resources)
if(NOT GRESOURCE)
	message(FATAL_ERROR "Can't find glib-compile-resources.")
endif()

add_custom_command(
	OUTPUT gresource.h
	COMMAND "${GRESOURCE}"
		--generate-header
		--target=${CMAKE_CURRENT_BINARY_DIR}/gresource.h
		silikego.gresource.xml
	DEPENDS silikego.gresource.xml
	WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

add_custom_command(
	OUTPUT gresource.cc
	COMMAND "${GRESOURCE}"
		--generate-source
		--target=${CMAKE_CURRENT_BINARY_DIR}/gresource.cc
		silikego.gresource.xml
	DEPENDS silikego.gresource.xml
	WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(SilikegoGui)
target_sources(SilikegoGui
	PRIVATE
		Main.cc
		EvalWindow.cc
		gresource.cc
		EvalWindow.h
		gresource.h
)
target_compile_features(SilikegoCore PRIVATE cxx_std_11)
set_target_properties(SilikegoGui PROPERTIES
	OUTPUT_NAME ${ONAMEGUI})
target_link_libraries(SilikegoGui PRIVATE Silikego::Core PkgConfig::Gtkmm3)

install(TARGETS SilikegoGui
	RUNTIME    DESTINATION ${INSTBIN})

install(
	FILES silikego.desktop
	DESTINATION "${INSTSHR}/applications"
)

install(
	FILES ${CMAKE_SOURCE_DIR}/icon/silikego-48.png
	DESTINATION "${INSTSHR}/icons/hicolor/48x48/apps"
	RENAME silikego.png
)
