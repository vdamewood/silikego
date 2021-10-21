# CMakeLists.txt: Build Script
# Copyright 2012-2021 Vincent Damewood
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

find_package(Readline)

add_executable(Silikegilo)
target_sources(Silikegilo
	PRIVATE
		StringSource.h
		Main.cc
		StringSource.cc
)
set_target_properties(Silikegilo PROPERTIES
	OUTPUT_NAME               silikegilo
	RUNTIME_OUTPUT_DIRECTORY  "${PROJECT_BINARY_DIR}/products"
	LIBRARY_OUTPUT_DIRECTORY  "${PROJECT_BINARY_DIR}/products"
	ARCHIVE_OUTPUT_DIRECTORY  "${PROJECT_BINARY_DIR}/products"
	PDB_OUTPUT_DIRECTORY      "${PROJECT_BINARY_DIR}/products"
)
target_compile_features(Silikegilo PRIVATE cxx_std_11)
target_link_libraries(Silikegilo PRIVATE Silikego::Silikego)

if (UNIX)
	target_compile_definitions(Silikegilo PRIVATE USE_UNIX=1)
endif()

if(Readline_FOUND)
	target_link_libraries(Silikegilo PRIVATE Readline::Readline)
	target_compile_definitions(Silikegilo PRIVATE HAVE_READLINE=1)
else()
	target_sources(Silikegilo PRIVATE rl.cc)
	target_compile_definitions(Silikegilo PRIVATE HAVE_READLINE=0)
endif()

if(APPLE AND SILIKEGO_BUILD_SHARED_LIBRARY)
	if (SILIKEGO_BUILD_FRAMEWORK)
		set_target_properties(Silikegilo PROPERTIES
			INSTALL_RPATH                   "@executable_path/../Frameworks"
		)
	else()
		set_target_properties(Silikegilo PROPERTIES
			INSTALL_RPATH                   "@executable_path/../lib"
		)
	endif()
endif()

install(TARGETS Silikegilo
RUNTIME
	DESTINATION  ${CMAKE_INSTALL_BINDIR}
	COMPONENT    Silikegilo
)
