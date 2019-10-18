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

find_program(IBTOOL ibtool)
if (NOT IBTOOL)
	message(FATAL_ERROR "Command ibtool not found. Can't build GUI. Quitting.")
endif()

add_custom_command(OUTPUT MainMenu.nib
	COMMAND "${IBTOOL}" --compile MainMenu.nib "${CMAKE_CURRENT_SOURCE_DIR}/MainMenu.xib"
	COMMENT "Compiling ${CMAKE_CURRENT_SOURCE_DIR}/MainMenu.xib"
)
set_source_files_properties(MainMenu.nib PROPERTIES MACOSX_PACKAGE_LOCATION Resources)
set_source_files_properties(silikego.icns PROPERTIES MACOSX_PACKAGE_LOCATION Resources)

add_executable(SilikegujoForMacOS MACOSX_BUNDLE)
target_sources(SilikegujoForMacOS
	PRIVATE
		Delegate.h
		Delegate.mm
		Main.mm
		MainMenu.nib
		silikego.icns
)
target_compile_options(SilikegujoForMacOS PRIVATE "-fobjc-arc")
target_compile_features(SilikegujoForMacOS PRIVATE cxx_std_11)
target_link_libraries(SilikegujoForMacOS PRIVATE Silikego::Core "-framework Cocoa")
set_target_properties(SilikegujoForMacOS PROPERTIES
	OUTPUT_NAME                         Silikegujo
	MACOSX_BUNDLE_GUI_IDENTIFIER        "com.vdamewood.SilikegoGui"
	MACOSX_BUNDLE_BUNDLE_NAME           "Silikego"
	MACOSX_BUNDLE_SHORT_VERSION_STRING  ${PROJECT_VERSION}
	MACOSX_BUNDLE_BUNDLE_VERSION        ${PROJECT_VERSION}
	MACOSX_BUNDLE_COPYRIGHT             ${COPYRIGHT_STATEMENT}
	MACOSX_BUNDLE_ICON_FILE             silikego.icns
	MACOSX_BUNDLE_INFO_PLIST            "${CMAKE_CURRENT_SOURCE_DIR}/Info.plist.in"
)
install(TARGETS SilikegujoForMacOS
	RUNTIME DESTINATION ${INSTBIN}
	BUNDLE  DESTINATION .)

install(CODE "include(BundleUtilities)")
install(CODE "fixup_bundle(\"$<TARGET_FILE:SilikegujoForMacOS>\" \"\" \"$<TARGET_FILE_DIR:Silikego::Core>\")")
