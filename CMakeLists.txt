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

############################
# Preamble (Semi-Required) #
############################

cmake_minimum_required(VERSION 3.1.0)

project(Silikego CXX)
set(Silikego_VERSION_MAJOR 0)
set(Silikego_VERSION_MINOR 0)
set(Silikego_VERSION_PATCH 0)

#########################
# Configuration Options #
#########################

include(CMakeDependentOption)
option(BUILD_SHARED_LIBS "Build shared libraries" On)
option(BUILD_GUI "Build a GUI using native API" On)

#######################
# Dependency Searches #
#######################

# Find Cocoa and ibtool
if (BUILD_GUI AND APPLE)
  set(USE_COCOA On)

  message("-- Finding Cocoa")
  find_library(COCOA Cocoa)
  if(NOT ${COCOA} STREQUAL "COCOA-NOTFOUND")
    message("-- Finding Cocoa - found")
    set(COCOA_FOUND Yes)
  else()
    message("-- Finding Cocoa - not found")
    set(COCOA_FOUND No)
  endif()

  message("-- Finding ibtool")
  find_program(IBTOOL ibtool)
  if (NOT ${IBTOOL} STREQUAL "IBTOOL-NOTFOUND")
    message("-- Finding ibtool - found")
    set(IBTOOL_FOUND Yes)
  else()
    message("-- Finding ibtool - not found")
    set(IBTOOL_FOUND No)
  endif()
else()
  set(USE_COCOA Off)
  set(COCOA_FOUND No)
  set(IBTOOL_FOUND No)
endif()

if (BUILD_GUI AND UNIX AND NOT APPLE)
  set(USE_GTK2 On)

  message("-- Finding GTKmm 2")
  find_package(GTK2 QUIET COMPONENTS gtk gtkmm)
  if(GTK2_FOUND)
    message("-- Finding GTKmm 2 - found")
  else()
    message("-- Finding GTKmm 2 - not found")
  endif()
else()
  set(USE_GTK2 Off)
  set(GTK2_FOUND No)
endif()

if (BUILD_GUI_NATIVE AND WIN32)
  set(USE_MFC On)

  message("-- Finding MFC Framework")
  find_package(MFC QUIET)
  if(MFC_FOUND)
    message("-- Finding MFC Framework - found")
  else()
    message("-- Finding MFC Framework - not found")
  endif()
else()
  set(USE_MFC Off)
  set(MFC_FOUND No)
endif()

####################
# Failure Messages #
####################

if(USE_COCOA AND NOT COCOA_FOUND)
  message(FATAL_ERROR "-- Can't build GUI on OS X. Cocoa not found.")
endif()

if(USE_COCOA AND NOT IBTOOL_FOUND)
  message(FATAL_ERROR "-- Can't build GUI on OS X. ibtool not found.")
endif()

if(USE_GTK2 AND NOT GTK2_FOUND)
    message(FATAL_ERROR "-- Can't build GUI on Unix. GTKmm 2 not found.")
endif()

if (USE_MFC AND NOT MFC_FOUND)
  message(FATAL_ERROR "-- Can't build GUI on Windows. MFC not found.")
endif()

############################
# Build-Independent Values #
############################

set(VERSION_STRING "${Silikego_VERSION_MAJOR}.${Silikego_VERSION_MINOR}.${Silikego_VERSION_PATCH}")
set(LIB_COPYRIGHT_STATEMENT "Copyright 2012, 2014, 2015, 2016 Vincent Damewood. Licensed under the terms of the GNU Lesser General Public License, version 3 or later.")
set(APP_COPYRIGHT_STATEMENT "Copyright 2012, 2014, 2015, 2016 Vincent Damewood. Licensed under the terms of the GNU General Public License, version 3 or later.")
set(BUNDLE_ID_DOMAIN "com.vdamewood")

###########################
# System-dependent Values #
###########################

if(WIN32)
  set(RUNTIME_INSTALL_PATH   .)
  set(ARCHIVE_INSTALL_PATH   .)
  set(LIBRARY_INSTALL_PATH   .)
  set(BUNDLE_INSTALL_PATH    Error)
  set(FRAMEWORK_INSTALL_PATH Error)
  set(HEADER_INSTALL_PATH    ./include)
  set(SHARE_INSTALL_PATH     .)
  set(DOC_INSTALL_PATH       .)
elseif(APPLE)
  set(RUNTIME_INSTALL_PATH   bin)
  set(ARCHIVE_INSTALL_PATH   Error)
  set(LIBRARY_INSTALL_PATH   Error)
  set(BUNDLE_INSTALL_PATH    .)
  set(FRAMEWORK_INSTALL_PATH .)
  set(HEADER_INSTALL_PATH    Error)
  set(SHARE_INSTALL_PATH     Error)
  set(DOC_INSTALL_PATH       .)
elseif(UNIX)
  set(RUNTIME_INSTALL_PATH   bin)
  if(${CMAKE_SYSTEM_PROCESSOR} STREQUAL "x86_64")
    set(ARCHIVE_INSTALL_PATH   lib64)
    set(LIBRARY_INSTALL_PATH   lib64)
  else()
    set(ARCHIVE_INSTALL_PATH   lib)
    set(LIBRARY_INSTALL_PATH   lib)
  endif()
  set(BUNDLE_INSTALL_PATH    share/Silikego/error)
  set(FRAMEWORK_INSTALL_PATH share/Silikego/error)
  set(HEADER_INSTALL_PATH    include)
  set(SHARE_INSTALL_PATH     share/Silikego)
  set(DOC_INSTALL_PATH       share/Silikego/docs)
endif()


#############################
# Compiler-dependent values #
#############################


if (${CMAKE_CXX_COMPILER_ID} STREQUAL "Clang"
  OR ${CMAKE_CXX_COMPILER_ID} STREQUAL "AppleClang"
  OR ${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU"
  OR ${CMAKE_CXX_COMPILER_ID} STREQUAL "SunPro")

  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
endif()

#############################
# Package-creation Settings #
#############################

set(CPACK_PACKAGE_DESCRIPTION_FILE "${Silikego_SOURCE_DIR}/Readme.md")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY
  "Silikego is a mathematical expression parser library.")
set(CPACK_PACKAGE_EXECUTABLES Silikego-Cli "Silikego (Command Line)" Silikego-Cli "Silikego (Command Line)")
list(APPEND CPACK_PACKAGE_EXECUTABLES Silikego-Cli "Silikego (Command Line)" Silikego-Cli "Silikego (Command Line)")

if (USE_COCOA)
  list(APPEND CPACK_PACKAGE_EXECUTABLES Silikego-Gui-Cocoa "Silikego (Cocoa GUI)")
endif()

if (USE_GTKMM2)
  list(APPEND CPACK_PACKAGE_EXECUTABLES Silikego-Gui-Gtk "Silikego (Gtkmm GUI)")
endif()

if (USE_MFC)
  list(APPEND CPACK_PACKAGE_EXECUTABLES Silikego-Gui-Win32 "Silikego (MFC GUI)")
endif()

set(CPACK_PACKAGE_VERSION_MAJOR ${Silikego_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${Silikego_VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${Silikego_VERSION_PATCH})
set(CPACK_PACKAGE_FILE_NAME "${CMAKE_PROJECT_NAME}-${VERSION_STRING}")
set(CPACK_SOURCE_GENERATOR TGZ TBZ2 ZIP)
set(CPACK_SOURCE_PACKAGE_FILE_NAME ${CPACK_PACKAGE_FILE_NAME})
set(CPACK_SOURCE_IGNORE_FILES "/.git/" "/.gitignore" "/Build/")
set(CPACK_RESOURCE_FILE_README "${Silikego_SOURCE_DIR}/Readme.md")

if(APPLE)
  set(CPACK_GENERATOR DragNDrop)
elseif(WIN32)
  set(CPACK_GENERATOR NSIS)
  set(CPACK_RESOURCE_FILE_LICENSE "${Silikego_SOURCE_DIR}/GPL-3.0.txt")
  if(BUILD_GUI_NATIVE OR BUILD_GUI_QT)
    set(CPACK_NSIS_EXECUTABLES_DIRECTORY ${RUNTIME_INSTALL_PATH})
  endif()
endif()

###########################
# Subdirectory Inclusions #
###########################

# Libraries
add_subdirectory(lib)

# Allow executable builds to include library headers.
include_directories(${Silikego_SOURCE_DIR}/lib)

# Command-line Programs
add_subdirectory(command)

# GUI Programs
if(USE_COCOA)
  add_subdirectory(cocoa)
endif()

if(USE_GTK)
  add_subdirectory(gtkmm2)
endif()

if(USE_MFC)
    add_subdirectory(mfc)
endif()

##################################
# Preexisting File Installations #
##################################

install(
  FILES Readme.md Authors.txt GPL-3.0.txt LGPL-3.0.txt
  DESTINATION ${DOC_INSTALL_PATH})

###############################################
# CPack Inclusion (Used for package creation) #
###############################################

include(CPack)
