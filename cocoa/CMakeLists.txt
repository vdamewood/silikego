# CMakeLists.txt: Build Script
# Copyright 2012, 2014, 2015, 2016, 2019 Vincent Damewood
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

set(XIB MainMenu)

set(MACOSX_BUNDLE_GUI_IDENTIFIER "com.vdamewood.SilikegoGui")
set(MACOSX_BUNDLE_BUNDLE_NAME "Silikego")
set(MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION})
set(MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION})
set(MACOSX_BUNDLE_COPYRIGHT ${COPYRIGHT_STATEMENT})
set(MACOSX_BUNDLE_MAIN_NIB_FILE ${XIB})
set(MACOSX_BUNDLE_PRINCIPAL_CLASS "NSApplication")
set(MACOSX_BUNDLE_ICON_FILE silikego.icns)

find_program(IBTOOL ibtool)
if (NOT ${IBTOOL} STREQUAL "IBTOOL-NOTFOUND")
	message("-- Finding ibtool - found")
else()
	message("-- Finding ibtool - not found")
	message(FATAL_ERROR "Can't build GUI. Quitting.")
endif()

add_custom_command(OUTPUT "${XIB}.nib"
	COMMAND ${IBTOOL} --compile ${XIB}.nib ${CMAKE_CURRENT_SOURCE_DIR}/${XIB}.xib
				COMMENT "Compiling ${CMAKE_CURRENT_SOURCE_DIR}/${XIB}.xib")
set_source_files_properties("${XIB}.nib" PROPERTIES MACOSX_PACKAGE_LOCATION Resources)
set_source_files_properties(silikego.icns PROPERTIES MACOSX_PACKAGE_LOCATION Resources)

add_executable(SilikegoGui MACOSX_BUNDLE
	Delegate.h
	Delegate.mm
	Main.mm
	"${XIB}.nib"
	silikego.icns)
target_compile_options(SilikegoGui PRIVATE "-fobjc-arc")
target_compile_features(SilikegoCore PRIVATE cxx_std_11)
target_link_libraries(SilikegoGui PRIVATE Silikego::Core "-framework Cocoa")
set_target_properties(SilikegoGui PROPERTIES
	OUTPUT_NAME "${ONAMEGUI}"
	MACOSX_BUNDLE_INFO_PLIST "${CMAKE_CURRENT_SOURCE_DIR}/Info.plist.in")
install(TARGETS SilikegoGui
	RUNTIME DESTINATION ${INSTBIN}
	BUNDLE  DESTINATION .)

	install(CODE "include(BundleUtilities)")
	install(CODE "fixup_bundle(\"$<TARGET_FILE:SilikegoGui>\" \"\" \"$<TARGET_FILE_DIR:Silikego::Core>\")")
