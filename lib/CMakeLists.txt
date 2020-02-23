# CMakeLists.txt: Build Script
# Copyright 2012, 2014, 2015, 2016, 2019 Vincent Damewood
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

add_library(Silikego)
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
		Functions.h
		Value.cc
		Functions.cc
		FunctionCaller.cc
		SyntaxTree.cc
		DataSource.cc
		StringSource.cc
		Token.cc
		Lexer.cc
		InfixParser.cc
		SilikegoConfig.cmake
)
add_library(Silikego::Silikego ALIAS Silikego)
target_include_directories(Silikego
	PUBLIC
		"${CMAKE_CURRENT_SOURCE_DIR}/include"
		"${CMAKE_CURRENT_BINARY_DIR}/include"
)
target_compile_features(Silikego PRIVATE cxx_std_11)
set_target_properties(Silikego PROPERTIES
	PUBLIC_HEADER             "${PUBLIC_HEADERS}"
	CXX_VISIBILITY_PRESET     hidden
	VISIBILITY_INLINES_HIDDEN On
	RESOURCE                  SilikegoConfig.cmake
	FRAMEWORK                 TRUE
	FRAMEWORK_VERSION         ${PROJECT_VERSION}
	VERSION                   ${PROJECT_VERSION}
	SOVERSION                 ${PROJECT_VERSION_MAJOR}
)

include(GenerateExportHeader)
generate_export_header(Silikego
	EXPORT_FILE_NAME include/Silikego/Api.h
	EXPORT_MACRO_NAME SILIKEGO_API
)

install(TARGETS Silikego
	RUNTIME       DESTINATION ${INSTBIN}
	ARCHIVE       DESTINATION ${INSTLIB}
	LIBRARY       DESTINATION ${INSTLIB}
	FRAMEWORK     DESTINATION .
	RESOURCE      DESTINATION ${INSTDAT}
	PUBLIC_HEADER DESTINATION ${INSTINC}/Silikego)

if(UNIX AND NOT APPLE)
	configure_file(Silikego.pc.in Silikego.pc @ONLY)
	install(FILES "${CMAKE_CURRENT_BINARY_DIR}/Silikego.pc" DESTINATION "${INSTLIB}/pkgconfig")
endif()
