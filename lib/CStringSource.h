/* CStringSource.h: Class to input data from a C-style string
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

#if !defined SILIKEGO_CSTRING_SOURCE_H
#define SILIKEGO_CSTRING_SOURCE_H

#include <string>

#include "W32Dll.h"
#include "DataSource.h"

namespace Silikego
{
	class SILIKEGO_API CStringSource : public DataSource
	{
	public:
		CStringSource(const char *);
		CStringSource(const std::string &);
		virtual bool Advance();
		virtual char GetCurrent();
		virtual ~CStringSource();

	private:
		class State;
		State* S;
	};
};

#endif // SILIKEGO_CSTRING_SOURCE_H
