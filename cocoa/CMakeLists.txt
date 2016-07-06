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
set(HEADERS Delegate.hh)
set(XIB MainMenu)
set(SOURCES Delegate.mm Main.mm "${XIB}.nib")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fobjc-arc")
set(MACOSX_BUNDLE_GUI_IDENTIFIER "com.vdamewood.${TARGET}")
set(MACOSX_BUNDLE_BUNDLE_NAME "Silikego")
set(MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION})
set(MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION})
set(MACOSX_BUNDLE_COPYRIGHT ${COPYRIGHT_STATEMENT})
set(MACOSX_BUNDLE_MAIN_NIB_FILE ${XIB})
set(MACOSX_BUNDLE_PRINCIPAL_CLASS "NSApplication")

set(DEPS SilikegoLib)
message("-- Finding Cocoa")
find_library(COCOA Cocoa)
if(NOT ${COCOA} STREQUAL "COCOA-NOTFOUND")
  message("-- Finding Cocoa - found")
  list(APPEND DEPS ${COCOA})
  set(COCOA_FOUND Yes)
else()
  message("-- Finding Cocoa - not found")
  set(COCOA_FOUND No)
endif()

find_program(IBTOOL ibtool)
if (NOT ${IBTOOL} STREQUAL "IBTOOL-NOTFOUND")
  message("-- Finding ibtool - found")
  set(IBTOOL_FOUND Yes)
else()
  message("-- Finding ibtool - not found")
  set(IBTOOL_FOUND No)
endif()

if(NOT COCOA_FOUND OR NOT IBTOOL_FOUND)
  message(FATAL_ERROR "Can't build GUI. Quitting.")
endif()

add_custom_command(OUTPUT "${XIB}.nib"
	COMMAND ${IBTOOL} --compile ${XIB}.nib ${CMAKE_CURRENT_SOURCE_DIR}/${XIB}.xib
        COMMENT "Compiling ${CMAKE_CURRENT_SOURCE_DIR}/${XIB}.xib")
set_source_files_properties("${XIB}.nib" PROPERTIES MACOSX_PACKAGE_LOCATION Resources)

add_executable(${TARGET} MACOSX_BUNDLE ${HEADERS} ${SOURCES})
set_target_properties(${TARGET} PROPERTIES
  OUTPUT_NAME "${ONAMEGUI}"
  MACOSX_BUNDLE_INFO_PLIST "${CMAKE_CURRENT_SOURCE_DIR}/Info.plist.in")
target_link_libraries(${TARGET} ${DEPS})

install(TARGETS "${TARGET}"
  RUNTIME DESTINATION ${INSTBIN}
  BUNDLE  DESTINATION .)

install(CODE "include(BundleUtilities)")
install(CODE "message(\"Installing Libs from ${LIB_BINARY_DIR}\")")
install(CODE "fixup_bundle(\"\${CMAKE_INSTALL_PREFIX}/${ONAMEGUI}.app\" \"\" \"${LIB_BINARY_DIR}\")")
