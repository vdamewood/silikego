/* StringSource.cc: Class to input data from a string
 * Copyright 2012-2025 Vincent Damewood
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


#include <string>

#include <SilikegoCore/StringSource.h>

namespace Silikego
{
	class StringSource::Impl
	{
	public:
		Impl(const char* NewInput) : Input(NewInput) { }
		Impl(const std::string &NewInput) : Input(NewInput) { }

		std::string Input;
		std::string::iterator Index = Input.begin();
	};

	StringSource::StringSource(const char *NewSource)
		: _impl(new Impl(NewSource))
	{
	}

	StringSource::StringSource(const std::string &NewSource)
		: _impl(new Impl(NewSource))
	{
	}

	StringSource::~StringSource()
	{
		delete _impl;
	}

	bool StringSource::advance()
	{
		if (_impl->Index != _impl->Input.end())
		{
			_impl->Index++;
			return true;
		}
		else
		{
			return false;
		}
	}

	char StringSource::current()
	{
		if (_impl->Index != _impl->Input.end())
			return *_impl->Index;
		else
			return '\0';
	}
}
