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
		Token(EndOfInput);
		Token(const Token& sourcce);
		Token(Token&& source);
		~Token();

		Token& operator=(int source);
		Token& operator=(long long int source);
		Token& operator=(double source);
		Token& operator=(char source);
		Token& operator=(const std::string& source);
		Token& operator=(const EndOfInput& source);
		Token& operator=(const Token& source);
		Token& operator=(Token&& source);

		TokenStatus status() const;
		long long int integer() const;
		double real() const;
		char character() const;
		const std::string& id() const;
	};
};

#endif // SILIKEGO_CORE_TOKEN_H
