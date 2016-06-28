# CMakeLists.txt: Build Script
# Copyright 2012, 2014, 2015, 2016 Vincent Damewood
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

set(TARGET SilikegoLib)

set(PUB_HEADERS
  W32Dll.hh
  Value.hh
  FunctionCaller.hh
  SyntaxTree.hh
  DataSource.hh
  CStringSource.hh
  Token.hh
  Lexer.hh
  InfixParser.hh)
set(HEADERS
  ${PUB_HEADERS}
  Functions.hh)
set(SOURCES
  Value.cc
  Functions.cc
  FunctionCaller.cc
  SyntaxTree.cc
  DataSource.cc
  CStringSource.cc
  Token.cc
  Lexer.cc
  InfixParser.cc)
set(RESOURCES
  SilikegoConfig.cmake)

add_library(${TARGET} ${HEADERS} ${SOURCES} ${RESOURCES})
set_target_properties(${TARGET} PROPERTIES
    OUTPUT_NAME   ${ONAMEDLIB}
    FRAMEWORK     ${BUILD_MACOSX}
    PUBLIC_HEADER ${PUB_HEADERS}
    RESOURCE      ${RESOURCES}
    VERSION       ${PROJECT_VERSION}
    SOVERSION     ${PROJECT_VERSION_MAJOR})

install(TARGETS ${TARGET}
  RUNTIME       DESTINATION ${INSTBIN}
  ARCHIVE       DESTINATION ${INSTLIB}
  LIBRARY       DESTINATION ${INSTLIB}
  FRAMEWORK     DESTINATION .
  RESOURCE      DESTINATION ${INSTDAT}
  PUBLIC_HEADER DESTINATION ${INSTINC}/Silikego)

set(LIB_BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR} PARENT_SCOPE)
