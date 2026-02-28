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
		: _impl(new(std::nothrow) Impl) { }
	Token::Token(char source)
		: _impl(new(std::nothrow) Impl{source}) { }
	Token::Token(int source)
		: _impl(new(std::nothrow)
			Impl{static_cast<long long int>(source)}) { }
	Token::Token(long long int source)
		: _impl(new(std::nothrow) Impl{source}) { }
	Token::Token(double source)
		: _impl(new(std::nothrow) Impl{source}) { }
	Token::Token(const std::string& source)
		: _impl(new(std::nothrow) Impl{source}) { }
	Token::Token(EndOfInput source)
		: _impl(new(std::nothrow) Impl{source}) { }
	Token::Token(const Token& source)
		: _impl(new(std::nothrow) Impl{*source._impl}) { }
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
		if (isEmpty())
			_impl = new(std::nothrow)
				Impl{static_cast<long long int>(source)};
		else
			_impl->data = source;
		return *this;
	}

	Token& Token::operator=(long long int source)
	{
		if (isEmpty())
			_impl = new(std::nothrow) Impl{source};
		else
			_impl->data = source;
		return *this;
	}

	Token& Token::operator=(double source)
	{
		if (isEmpty())
			_impl = new(std::nothrow) Impl{source};
		else
			_impl->data = source;
		return *this;
	}

	Token& Token::operator=(char source)
	{
		if (isEmpty())
			_impl = new(std::nothrow) Impl{source};
		else
			_impl->data = source;
		return *this;
	}

	Token& Token::operator=(const std::string& source)
	{
		if (isEmpty())
			_impl = new(std::nothrow) Impl{source};
		else
			_impl->data = source;
		return *this;
	}

	Token& Token::operator=(const EndOfInput& source)
	{
		if (isEmpty())
			_impl = new(std::nothrow) Impl{source};
		else
			_impl->data = source;
		return *this;
	}

	Token& Token::operator=(const Token& source)
	{
		if (this != &source)
		{
			delete _impl;
			_impl = new(std::nothrow) Impl{*source._impl};
		}
		return *this;
	}

	Token& Token::operator=(Token&& right_side)
	{
		if (this != &right_side)
		{
			delete _impl;
			_impl = right_side._impl;
			right_side._impl = nullptr;
		}
		return *this;
	}

	TokenStatus Token::status() const
	{
		if (isEmpty())
			return TokenStatus::Unset;

		return std::visit([](auto&& data)
		{
			using T = std::decay_t<decltype(data)>;
			if constexpr (std::is_same_v<T, std::monostate>)
				return TokenStatus::Unset;
			else if constexpr (std::is_same_v<T, char>)
				return TokenStatus::Character;
			else if constexpr (std::is_same_v<T, long long int>)
				return TokenStatus::Integer;
			else if constexpr (std::is_same_v<T, double>)
				return TokenStatus::Real;
			else if constexpr (std::is_same_v<T, std::string>)
				return TokenStatus::Id;
			else if constexpr (std::is_same_v<T, EndOfInput>)
				return TokenStatus::EndOfInput;
			else
				return TokenStatus::Unset;
		}, _impl->data);
	}

	char Token::character() const
	{
		if (isEmpty() || !std::holds_alternative<char>(_impl->data)
)
			return '\0';

		return std::get<char>(_impl->data);
	}

	long long int Token::integer() const
	{
		if (isEmpty() || !std::holds_alternative<long long int>(
				_impl->data))
			return 0LL;

		return std::get<long long int>(_impl->data);
	}

	double Token::real() const
	{
		if (isEmpty() || !std::holds_alternative<double>(_impl->data))
			return std::numeric_limits<double>::quiet_NaN();

		return std::get<double>(_impl->data);
	}

	const std::string* Token::id() const
	{
		if (isEmpty() || !std::holds_alternative<std::string>(
				_impl->data))
			return nullptr;

		return &std::get<std::string>(_impl->data);
	}
}
