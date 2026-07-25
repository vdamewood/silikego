// Copyright 2012-2026 Vincent Damewood
// SPDX-License-Identifier: LGPL-3.0-or-later

// This file is part of Silikego.

// Silikego is free software: you can redistribute it and/or modify it
// under the terms of the GNU Lesser General Public License as published
// by the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// Silikego is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
// Lesser General Public License for more details.

// You should have received a copy of the GNU Lesser General Public
// License along with Silikego. If not, see
// <http://www.gnu.org/licenses/>.


#if !defined SILIKEGO_CORE_TOKEN_H
#define SILIKEGO_CORE_TOKEN_H

#include <string>

#include <SilikegoCore/Api.h>

namespace Silikego
{
	enum class TokenStatus
	{
		Unset,
		Integer,
		Real,
		Character,
		Id,
		EndOfInput
	};

	struct SILIKEGOCORE_EXPORT EndOfInput{};

	class SILIKEGOCORE_EXPORT Token
	{
		class Impl;
		Impl* _impl;
	public:
		Token();
		Token(int source);
		Token(long long int source);
		Token(double source);
		Token(char source);
		Token(const std::string& source);
		Token(EndOfInput end_of_input);
		Token(const Token& sourcce);
		Token(Token&& source);
		~Token();

		Token& operator=(int right_side);
		Token& operator=(long long int right_side);
		Token& operator=(double right_side);
		Token& operator=(char right_side);
		Token& operator=(const std::string& right_side);
		Token& operator=(const EndOfInput& right_side);
		Token& operator=(const Token& right_side);
		Token& operator=(Token&& right_side);

		inline bool isEmpty() const
		{
			return _impl == nullptr;
		};

		TokenStatus status() const;
		long long int integer() const;
		double real() const;
		char character() const;
		const std::string* id() const;
	};
};

#endif // SILIKEGO_CORE_TOKEN_H
