# CMakeLists.txt: Build Script
# Copyright 2012-2021 Vincent Damewood
#
# This library is free software: you can redistribute it and/or modify
# it under the terms of the GNU Lesser General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This library is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
# GNU Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public License
# along with this library. If not, see <http://www.gnu.org/licenses/>.

if(APPLE)
	set(SILIKEGO_INSTALL_FRAMEWORKDIR Frameworks
		CACHE STRING "Locaton to install framework bundles (relative to install prefix)"
	)
	set(SILIKEGO_INSTALL_CMAKEDIR "${SILIKO_INSTALL_FRAMEWORKDIR}/Siliko.framework/Resources")
else()
	set(SILIKEGO_INSTALL_FRAMEWORKDIR Frameworks)
	set(SILIKEGO_INSTALL_CMAKEDIR ${CMAKE_INSTALL_LIBDIR}/cmake/Siliko
		CACHE STRING "Locaton to install CMake package files (relative to install prefix)"
	)
endif()

add_library(Silikego)
add_library(Silikego::Silikego ALIAS Silikego)

set(PUBLIC_HEADERS
	include/Silikego/Value.h
	include/Silikego/FunctionCaller.h
	include/Silikego/SyntaxTree.h
	include/Silikego/DataSource.h
	include/Silikego/StringSource.h
	include/Silikego/Token.h
	include/Silikego/Lexer.h
	include/Silikego/InfixParser.h
	${CMAKE_CURRENT_BINARY_DIR}/include/Silikego/Api.h
)
target_sources(Silikego
	PRIVATE
		${PUBLIC_HEADERS}
		src/Functions.h
		src/Value.cc
		src/Functions.cc
		src/FunctionCaller.cc
		src/SyntaxTree.cc
		src/DataSource.cc
		src/StringSource.cc
		src/Token.cc
		src/Lexer.cc
		src/InfixParser.cc
)
target_include_directories(Silikego
	PUBLIC
		"$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
		"$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>"
)
if (NOT APPLE)
	target_include_directories(Silikego
	INTERFACE
		$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)
endif()
target_compile_features(Silikego PRIVATE cxx_std_11)
set_target_properties(Silikego PROPERTIES
	PUBLIC_HEADER             "${PUBLIC_HEADERS}"
	CXX_VISIBILITY_PRESET     hidden
	VISIBILITY_INLINES_HIDDEN On
	FRAMEWORK                 TRUE
	FRAMEWORK_VERSION         ${PROJECT_VERSION}
	VERSION                   ${PROJECT_VERSION}
	SOVERSION                 ${PROJECT_VERSION_MAJOR}
	RUNTIME_OUTPUT_DIRECTORY  "${PROJECT_BINARY_DIR}/products"
	LIBRARY_OUTPUT_DIRECTORY  "${PROJECT_BINARY_DIR}/products"
	ARCHIVE_OUTPUT_DIRECTORY  "${PROJECT_BINARY_DIR}/products"
	PDB_OUTPUT_DIRECTORY      "${PROJECT_BINARY_DIR}/products"
)

include(GenerateExportHeader)
generate_export_header(Silikego
	EXPORT_FILE_NAME include/Silikego/Api.h
	EXPORT_MACRO_NAME SILIKEGO_API
)

include(CMakePackageConfigHelpers)
write_basic_package_version_file(
	SilikegoConfigVersion.cmake
	VERSION ${Silikego_VERSION}
	COMPATIBILITY SameMajorVersion
)

install(TARGETS Silikego
	EXPORT Silikego
RUNTIME
	DESTINATION           ${CMAKE_INSTALL_BINDIR}
	COMPONENT             Silikego_Runtime
ARCHIVE
	DESTINATION           ${CMAKE_INSTALL_LIBDIR}
	COMPONENT             Silikego_Development
LIBRARY
	DESTINATION           ${CMAKE_INSTALL_LIBDIR}
	COMPONENT             Silikego_Runtime
	NAMELINK_COMPONENT    Silikego_Development
FRAMEWORK
	DESTINATION           ${SILIKEGO_INSTALL_FRAMEWORKDIR}
	COMPONENT             Silikego_Development
PUBLIC_HEADER
	DESTINATION           ${CMAKE_INSTALL_INCLUDEDIR}/Silikego
	COMPONENT             Silikego_Development
)
install(EXPORT Silikego
	NAMESPACE             Silikego::
	FILE                  SilikegoConfig.cmake
	DESTINATION           ${SILIKEGO_INSTALL_CMAKEDIR}
	COMPONENT             Silikego_Development
)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/SilikegoConfigVersion.cmake
	DESTINATION           ${SILIKEGO_INSTALL_CMAKEDIR}
	COMPONENT             Silikego_Development
)
