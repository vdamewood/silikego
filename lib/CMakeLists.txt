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

add_library(SilikegoCore SHARED
	W32Dll.h
	Value.h
	FunctionCaller.h
	SyntaxTree.h
	DataSource.h
	StringSource.h
	Token.h
	Lexer.h
	InfixParser.h
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
add_library(Silikego::Core ALIAS SilikegoCore)
target_include_directories(SilikegoCore PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_compile_features(SilikegoCore PRIVATE cxx_std_11)

set(PUBLIC_HEADERS)
	W32Dll.h
	Value.h
	FunctionCaller.h
	SyntaxTree.h
	DataSource.h
	StringSource.h
	Token.h
	Lexer.h
	InfixParser.h
)
set_target_properties(SilikegoCore PROPERTIES
		PUBLIC_HEADER     "${PUBLIC_HEADERS}"
		RESOURCE
			SilikegoConfig.cmake
		OUTPUT_NAME       ${ONAMEDLIB}
		FRAMEWORK         ${BUILD_MACOS}
		FRAMEWORK_VERSION ${PROJECT_VERSION}
		VERSION           ${PROJECT_VERSION}
		SOVERSION         ${PROJECT_VERSION_MAJOR})

install(TARGETS SilikegoCore
	RUNTIME       DESTINATION ${INSTBIN}
	ARCHIVE       DESTINATION ${INSTLIB}
	LIBRARY       DESTINATION ${INSTLIB}
	FRAMEWORK     DESTINATION .
	RESOURCE      DESTINATION ${INSTDAT}
	PUBLIC_HEADER DESTINATION ${INSTINC}/Silikego)

if(BUILD_UNIX)
	configure_file(Silikego.pc.in Silikego.pc @ONLY)
	install(FILES "${CMAKE_CURRENT_BINARY_DIR}/Silikego.pc" DESTINATION "${INSTLIB}/pkgconfig")
endif()
