# SilikegoConfig.cmake: Find Silikego library
# Copyright 2016 Vincent Damewood
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

include(FindPackageHandleStandardArgs)

find_path(SILIKEGO_INC
  NAMES Silikego/Value.hh
  DOC "Silikego Include Files")

find_library(SILIKEGO_LIB
  NAMES Silikego
  DOC "Silikego Library")

find_package_handle_standard_args(Silikego DEFAULT_MSG SILIKEGO_INC SILIKEGO_LIB)

if(Silikego_FOUND)
    set(SILIKEGO_LIBRARIES    ${SILIKEGO_LIB})
    set(SILIKEGO_INCLUDE_DIRS ${SILIKEGO_INC})
    set(SILIKEGO_DEFINITIONS  )
endif()

mark_as_advanced(SILIKEGO_INC SILIKEGO_LIB)
