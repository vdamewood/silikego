/* DataSource.h: Abstract interface for input data
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

#if !defined SILIKEGO_CORE_DATA_SOURCE_H
#define SILIKEGO_CORE_DATA_SOURCE_H

#include <SilikegoCore/Api.h>

namespace Silikego
{
	class SILIKEGOCORE_EXPORT DataSource
	{
	public:
		DataSource() = default;

		DataSource(const DataSource&) = delete;
		DataSource(DataSource&&) = delete;
		virtual ~DataSource() = default;

		DataSource& operator=(const DataSource&) = delete;
		DataSource& operator=(DataSource&&) = delete;

		virtual bool advance() = 0;
		virtual char current() = 0;
	};
};

#endif // SILIKEGO_CORE_DATA_SOURCE_H
