/* Input.h: Abstract interface for parser input stream
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

#if !defined SILIKEGO_CORE_INPUT_H
#define SILIKEGO_CORE_INPUT_H

#include <SilikegoCore/Api.h>

namespace Silikego
{
	class SILIKEGOCORE_EXPORT Input
	{
	public:
		Input();
		Input(const Input&) = delete;
		Input(Input&&) = delete;
		virtual ~Input();

		Input& operator=(const Input&) = delete;
		Input& operator=(Input&&) = delete;

		virtual bool advance() = 0;
		virtual char character() = 0;
	};
};

#endif // SILIKEGO_CORE_INPUT_H
