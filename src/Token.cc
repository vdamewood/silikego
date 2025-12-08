/* Token.cc: Token passed from tokenizer to parser
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

#include <string>
#include <variant>

#include <SilikegoCore/Token.h>

namespace
{
	const int UnsetIndex = 0;
	const int OperatorIndex = 1;
	const int IntegerIndex = 2;
	const int FloatIndex = 3;
	const int IdIndex = 4;
	const int EndOfInputIndex = 5;
};

namespace Silikego
{
	class Token::Impl
	{
	public:
		Impl() { }
		Impl(char new_operator) : data(new_operator) { }
		Impl(long long int new_value) : data(new_value) { }
		Impl(double new_value) : data(new_value) { }
		Impl(const std::string& new_id) : data(new_id) { }
		Impl(EndOfInput placeholder) : data(placeholder) { }
		Impl(const Impl& other) : data(other.data) { }

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
		: impl(new Impl()) { }
	Token::Token(char new_operator)
		: impl(new Impl(new_operator)) { }
	Token::Token(short new_value)
		: impl(new Impl(static_cast<long long int>(new_value))) { }
	Token::Token(int new_value)
		: impl(new Impl(static_cast<long long int>(new_value))) { }
	Token::Token(long int new_value)
		: impl(new Impl(static_cast<long long int>(new_value))) { }
	Token::Token(long long int new_value)
		: impl(new Impl(new_value)) { }
	Token::Token(float new_value)
		: impl(new Impl(static_cast<double>(new_value))) { }
	Token::Token(double new_value)
		: impl(new Impl(new_value)) { }
	Token::Token(const std::string& new_id)
		: impl(new Impl(new_id)) { }
	Token::Token(EndOfInput placeholder)
		: impl(new Impl(placeholder)) { }
	Token::Token(const Token& right_side)
		: impl(new Impl(*right_side.impl)) { }

	Token::~Token()
	{
		delete impl;
	}

	Token& Token::operator=(const Token& RightSide)
	{
		*impl = *RightSide.impl;
		return *this;
	}

	TokenStatus Token::status() const
	{
		switch(impl->data.index())
		{
		case UnsetIndex:
			return TokenStatus::Unset;
		case OperatorIndex:
			return TokenStatus::Operator;
		case IntegerIndex:
			return TokenStatus::Integer;
		case FloatIndex:
			return TokenStatus::Float;
		case IdIndex:
			return TokenStatus::Id;
		case EndOfInputIndex:
			return TokenStatus::EndOfInput;
		default:
			throw; // shouldn't happen
		}
	}

	char Token::operatorValue() const
	{
		return std::get<OperatorIndex>(impl->data);
	}

	long long int Token::integerValue() const
	{
		return std::get<IntegerIndex>(impl->data);
	}

	double Token::floatValue() const
	{
		return std::get<FloatIndex>(impl->data);
	}

	const std::string& Token::idValue() const
	{
		return std::get<IdIndex>(impl->data);
	}
}
