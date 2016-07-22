/* W32Dll.hh: Header to accomodate building a DLL on Microsoft Windows
 * Copyright 2014, 2015, 2016 Vincent Damewood
 *
 * This library is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library. If not, see <http://www.gnu.org/licenses/>.
 */

#if !defined SILIKEGO_W32_DLL_HH

#if defined _WIN32 && defined _MSC_VER && !defined SILIKEGO_STATIC
#  if defined SilikegoDLib_EXPORTS
#    define SILIKEGO_API __declspec(dllexport)
#  else // SILIKEGO_EXPORTS
#    define SILIKEGO_API __declspec(dllimport)
#  endif // SILIKEGO_EXPORTS
#else // _WIN32 && __MSC_VER
#  define SILIKEGO_API
#endif // _WIN32 && __MSC_VER

#endif // SILIKEGO_W32_DLL_HH
