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


#include <string>
#include <variant>

#include <SilikegoCore/Token.h>

namespace
{
	const int UnsetIndex = 0;
	const int CharacterIndex = 1;
	const int IntegerIndex = 2;
	const int RealIndex = 3;
	const int IdIndex = 4;
	const int EndOfInputIndex = 5;
};

namespace Silikego
{
	class Token::Impl
	{
	public:
		Impl() { }
		Impl(char source) : data{source} { }
		Impl(long long int source) : data{source} { }
		Impl(double source) : data{source} { }
		Impl(const std::string& source) : data{source} { }
		Impl(EndOfInput source) : data({source}) { }
		Impl(const Impl& source) : data{source.data} { }

		Impl& operator=(const Impl& right_side)
		{
			data = right_side.data;
			return *this;
		}

		std::variant<
			std::monostate,
			char,
			long long int,
			double,
			std::string,
			EndOfInput
		> data;
	};

	Token::Token()
		: _impl(new Impl) { }
	Token::Token(char source)
		: _impl(new Impl{source}) { }
	Token::Token(int source)
		: _impl(new Impl{static_cast<long long int>(source)}) { }
	Token::Token(long long int source)
		: _impl(new Impl{source}) { }
	Token::Token(double source)
		: _impl(new Impl{source}) { }
	Token::Token(const std::string& source)
		: _impl(new Impl{source}) { }
	Token::Token(EndOfInput source)
		: _impl(new Impl{source}) { }
	Token::Token(const Token& source)
		: _impl(new Impl{*source._impl}) { }
	Token::Token(Token&& source)
		: _impl{source._impl}
	{
		source._impl = nullptr;
	}

	Token::~Token()
	{
		delete _impl;
	}

	Token& Token::operator=(int source)
	{
		_impl->data = source;
		return *this;
	}

	Token& Token::operator=(long long int source)
	{
		_impl->data = source;
		return *this;
	}

	Token& Token::operator=(double source)
	{
		_impl->data = source;
		return *this;
	}

	Token& Token::operator=(char source)
	{
		_impl->data = source;
		return *this;
	}

	Token& Token::operator=(const std::string& source)
	{
		_impl->data = source;
		return *this;
	}

	Token& Token::operator=(const EndOfInput& source)
	{
		_impl->data = source;
		return *this;
	}

	Token& Token::operator=(const Token& source)
	{
		*_impl = *source._impl;
		return *this;
	}

	Token& Token::operator=(Token&& source)
	{
		_impl = source._impl;
		source._impl = nullptr;
		return *this;
	}

	TokenStatus Token::status() const
	{
		switch(_impl->data.index())
		{
		case UnsetIndex:
			return TokenStatus::Unset;
		case CharacterIndex:
			return TokenStatus::Character;
		case IntegerIndex:
			return TokenStatus::Integer;
		case RealIndex:
			return TokenStatus::Real;
		case IdIndex:
			return TokenStatus::Id;
		case EndOfInputIndex:
			return TokenStatus::EndOfInput;
		default:
			throw; // shouldn't happen
		}
	}

	char Token::character() const
	{
		return std::get<CharacterIndex>(_impl->data);
	}

	long long int Token::integer() const
	{
		return std::get<IntegerIndex>(_impl->data);
	}

	double Token::real() const
	{
		return std::get<RealIndex>(_impl->data);
	}

	const std::string& Token::id() const
	{
		return std::get<IdIndex>(_impl->data);
	}
}
