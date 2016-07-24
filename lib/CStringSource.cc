/* CStringSource.cc: Class to input data from a C-style string
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

// Visual Studio complains about strcpy without this.
#if defined _WIN32 && defined _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif /* _WIN32 && defined _MSC_VER */

#include <cstring>
#include <string>

#include "CStringSource.h"

namespace Silikego
{
	class CStringSource::State
	{
	public:
		State(const char* NewInput) : Input(NewInput) { }
		State(const std::string &NewInput) : Input(NewInput) { }

		std::string Input;
		std::string::iterator Index = Input.begin();
	};

	CStringSource::CStringSource(const char *NewSource)
		: S(new State(NewSource))
	{
	}

	CStringSource::CStringSource(const std::string &NewSource)
		: S(new State(NewSource))
	{
	}

	CStringSource::~CStringSource()
	{
		delete S;
	}

	bool CStringSource::Advance()
	{
		if (S->Index != S->Input.end())
		{
			S->Index++;
			return true;
		}
		else
		{
			return false;
		}
	}

	char CStringSource::GetCurrent()
	{
		return *S->Index;
	}
}
