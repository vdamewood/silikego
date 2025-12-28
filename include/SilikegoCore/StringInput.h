/* StringInput.h: Implementation of string as parser input stream
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

#if !defined SILIKEGO_CORE_STRING_INPUT_H
#define SILIKEGO_CORE_STRING_INPUT_H

#include <string>

#include <SilikegoCore/Api.h>
#include <SilikegoCore/Input.h>

namespace Silikego
{
	class SILIKEGOCORE_EXPORT StringInput : public Input
	{
		class Impl;
		Impl* _impl;
	public:
		StringInput() = delete;

		StringInput(const char* source);
		StringInput(const std::string& source);

		StringInput(const StringInput&) = delete;
		StringInput(StringInput&&) = delete;
		~StringInput() override;

		StringInput& operator=(const StringInput&) = delete;
		StringInput& operator=(StringInput&&) = delete;

		bool advance() override;
		char character() override;
	};
};

#endif // SILIKEGO_CORE_STRING_INPUT_H
