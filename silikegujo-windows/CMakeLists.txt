# CMakeLists.txt: Build Script
# Copyright 2012-2023 Vincent Damewood
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

add_executable(SilikegujoForWindows WIN32)
target_sources(SilikegujoForWindows
	PRIVATE
		EvalWindow.h
		About.h
		Id.h
		WinMain.cc
		EvalWindow.cc
		EvalWindow.rc
		About.cc
		About.rc
		Icon.rc
)
set_target_properties(SilikegujoForWindows PROPERTIES
	OUTPUT_NAME               Silikegujo
	RUNTIME_OUTPUT_DIRECTORY  "${PROJECT_BINARY_DIR}/products"
	LIBRARY_OUTPUT_DIRECTORY  "${PROJECT_BINARY_DIR}/products"
	ARCHIVE_OUTPUT_DIRECTORY  "${PROJECT_BINARY_DIR}/products"
	PDB_OUTPUT_DIRECTORY      "${PROJECT_BINARY_DIR}/products"
)
set(VSV_DEBUG_FLAG 0)
set(VSV_FILENAME "Silikegujo.exe")
configure_file(
	Version.rc.in
	${CMAKE_CURRENT_BINARY_DIR}/Version.Release.rc
	@ONLY
)
target_sources(SilikegujoForWindows PRIVATE $<$<CONFIG:Release>:${CMAKE_CURRENT_BINARY_DIR}/Version.Release.rc>)

set(VSV_DEBUG_FLAG VS_FF_DEBUG)
set(VSV_FILENAME "Silikegujo.exe")
configure_file(
	Version.rc.in
	${CMAKE_CURRENT_BINARY_DIR}/Version.Debug.rc
	@ONLY
)
target_sources(SilikegujoForWindows PRIVATE $<$<CONFIG:Debug>:${CMAKE_CURRENT_BINARY_DIR}/Version.Debug.rc>)
target_compile_features(SilikegujoForWindows PRIVATE cxx_std_11)
target_link_libraries(SilikegujoForWindows PRIVATE Silikego::Silikego)

install(TARGETS SilikegujoForWindows
RUNTIME
	DESTINATION  ${CMAKE_INSTALL_BINDIR}
	COMPONENT    Silikegujo
)
