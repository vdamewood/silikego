/* FunctionCaller.hh: Function handling
 * Copyright 2012, 2014, 2015, 2016 Vincent Damewood
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

#if !defined SILIKEGO_FUNCTION_CALLER_HH
#define SILIKEGO_FUNCTION_CALLER_HH

#include <string>
#include <vector>

#include "W32Dll.hh"
#include "Value.hh"


namespace Silikego
{
	namespace FunctionCaller
	{
		typedef Value (*FunctionPointer)(std::vector<Value>);

		SILIKEGO_API bool SetUp();
		SILIKEGO_API void TearDown();
		SILIKEGO_API Value Call(const std::string &Name, std::vector<Value> Args);
		SILIKEGO_API void Install(const std::string &Name, FunctionPointer Function);
	};
};
#endif // SILIKEGO_FUNCTION_CALLER_H
