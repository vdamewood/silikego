# CMakeLists.txt: Build Script
# Copyright 2012, 2014, 2015, 2016, 2018, 2019 Vincent Damewood
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

add_executable(SilikegoGui WIN32
	EvalWindow.h
	About.h Id.h
	WinMain.cc
	EvalWindow.cc
	EvalWindow.rc
	About.cc
	About.rc
	Icon.rc
)
target_compile_features(SilikegoCore PRIVATE cxx_std_11)
set_target_properties(SilikegoGui PROPERTIES
	OUTPUT_NAME ${ONAMEGUI})
target_link_libraries(SilikegoGui PRIVATE Silikego::Core)

add_custom_command(
	TARGET SilikegoGui POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_if_different
	$<TARGET_FILE:Silikego::Core>
	$<$<CONFIG:Debug>:$<TARGET_PDB_FILE:Silikego::Core>>
	${CMAKE_CURRENT_BINARY_DIR})

install(TARGETS SilikegoGui
	RUNTIME DESTINATION ${INSTBIN})
