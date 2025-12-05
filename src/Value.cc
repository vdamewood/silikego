/* Value.cc: Data structure to represent a single value
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

#if defined _WIN32
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <string>
#include <sstream>
#include <limits>

#include <SilikegoCore/Value.h>

namespace {
	const int ErrorIndex = 0;
	const int IntegerIndex = 1;
	const int FloatIndex = 2;
}

namespace Silikego
{
	class Value::State
	{
	public:
		State(Error new_error) : data(new_error) { }
		State(long long int new_integer) : data(new_integer) { }
		State(double new_float) : data(new_float) { }
		State(const State &source) : data(source.data) { }

		std::variant<Error, long long int, double> data;
	};

	Value::Value(Error new_error)
		: S(new State(new_error)) { }
	Value::Value(short new_integer)
		: S(new State(static_cast<long long int>(new_integer))) { }
	Value::Value(int new_integer)
		: S(new State(static_cast<long long int>(new_integer))) { }
	Value::Value(long int new_integer)
		: S(new State(static_cast<long long int>(new_integer))) { }
	Value::Value(long long int new_integer)
		: S(new State(new_integer)) { }
	Value::Value(float new_float)
		: S(new State(static_cast<double>(new_float))) { }
	Value::Value(double new_float)
		: S(new State(new_float)) { }
	Value::Value(const Value& source)
		: S(new State(*source.S)) { }

	Value::~Value()
	{
		delete S;
	}

	Value& Value::operator=(const Value& right_side)
	{
		S->data = right_side.S->data;
		return *this;
	}

	Value& Value::negate()
	{
		switch (S->data.index())
		{
		case (IntegerIndex):
			std::get<IntegerIndex>(S->data) *= -1;
			break;
		case (FloatIndex):
			std::get<FloatIndex>(S->data) *= -1.0;
			break;
		default:
			; // Do nothing. Silence warning.
		}
		return *this;
	}

	ValueStatus Value::status() const
	{
		switch (S->data.index())
		{
		case ErrorIndex:
			return ValueStatus::Error;
		case IntegerIndex:
			return ValueStatus::Integer;
		case FloatIndex:
			return ValueStatus::Float;
		default:
			throw; // shouldn't happen
		}
	}

	bool Value::isInteger() const
	{
		return S->data.index() == IntegerIndex;
	}

	long long int Value::asInteger() const
	{
		switch (S->data.index())
		{
		case IntegerIndex:
			return std::get<IntegerIndex>(S->data);
		case FloatIndex:
			return static_cast<long long int>(std::get<FloatIndex>(S->data));
		default:
			return 0;
		}
	}

	bool Value::isFloat() const
	{
		return S->data.index() == FloatIndex;
	}

	double Value::asFloat() const
	{
		switch (S->data.index())
		{
		case IntegerIndex:
			return static_cast<double>(std::get<IntegerIndex>(S->data));
		case FloatIndex:
			return std::get<FloatIndex>(S->data);
		default:
			return std::numeric_limits<double>::quiet_NaN();
		}
	}

	bool Value::isError() const
	{
		return S->data.index() == ErrorIndex;
	}

	Error Value::asError() const
	{
		return S->data.index() == ErrorIndex
			? std::get<ErrorIndex>(S->data)
			: Error::None;
	}
}
