/* StringSource.h: Class to input data from a string
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

#if !defined SILIKEGO_CORE_STRING_SOURCE_H
#define SILIKEGO_CORE_STRING_SOURCE_H

#include <string>

#include <SilikegoCore/Api.h>
#include <SilikegoCore/DataSource.h>

namespace Silikego
{
	class SILIKEGOCORE_EXPORT StringSource : public DataSource
	{
	public:
		StringSource() = delete;

		StringSource(const char *);
		StringSource(const std::string &);

		StringSource(const StringSource&) = delete;
		StringSource(StringSource&&) = delete;
		virtual ~StringSource() override;

		StringSource& operator=(const StringSource&) = delete;
		StringSource& operator=(StringSource&&) = delete;

		virtual bool advance() override;
		virtual char current() override;

	private:
		class Impl;
		Impl* _impl;
	};
};

#endif // SILIKEGO_CORE_STRING_SOURCE_H
