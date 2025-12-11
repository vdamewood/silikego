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

#include <limits>
#include <variant>

#include <SilikegoCore/Value.h>

namespace {
	const int ErrorIndex = 0;
	const int IntegerIndex = 1;
	const int FloatIndex = 2;
}

namespace Silikego
{
	class Value::Impl
	{
	public:
		Impl(Error new_error) : data(new_error) { }
		Impl(long long int new_integer) : data(new_integer) { }
		Impl(double new_float) : data(new_float) { }

		std::variant<Error, long long int, double> data;
	};

	Value::Value(Error new_error)
		: _impl(new Impl(new_error)) { }
	Value::Value(short new_integer)
		: _impl(new Impl(static_cast<long long int>(new_integer))) { }
	Value::Value(int new_integer)
		: _impl(new Impl(static_cast<long long int>(new_integer))) { }
	Value::Value(long int new_integer)
		: _impl(new Impl(static_cast<long long int>(new_integer))) { }
	Value::Value(long long int new_integer)
		: _impl(new Impl(new_integer)) { }
	Value::Value(float new_float)
		: _impl(new Impl(static_cast<double>(new_float))) { }
	Value::Value(double new_float)
		: _impl(new Impl(new_float)) { }

	Value::Value(const Value& source)
		: _impl(new Impl(*source._impl)) { }

	Value::Value(Value&& source)
		: _impl(source._impl)
	{
		source._impl = nullptr;
	}

	Value::~Value()
	{
		delete _impl;
	}

	Value& Value::operator=(const Value& right_side)
	{
		_impl->data = right_side._impl->data;
		return *this;
	}

	Value& Value::operator=(Value&& right_side)
	{
		delete _impl;
		_impl = right_side._impl;
		right_side._impl = nullptr;
		return *this;
	}

	Value& Value::negate()
	{
		switch (_impl->data.index())
		{
		case (IntegerIndex):
			std::get<IntegerIndex>(_impl->data) *= -1;
			break;
		case (FloatIndex):
			std::get<FloatIndex>(_impl->data) *= -1.0;
			break;
		default:
			; // Do nothing. Silence warning.
		}
		return *this;
	}

	ValueStatus Value::status() const
	{
		switch (_impl->data.index())
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

	long long int Value::asInteger() const
	{
		switch (_impl->data.index())
		{
		case IntegerIndex:
			return std::get<IntegerIndex>(_impl->data);
		case FloatIndex:
			return static_cast<long long int>(std::get<FloatIndex>(_impl->data));
		default:
			return 0;
		}
	}

	double Value::asFloat() const
	{
		switch (_impl->data.index())
		{
		case IntegerIndex:
			return static_cast<double>(std::get<IntegerIndex>(_impl->data));
		case FloatIndex:
			return std::get<FloatIndex>(_impl->data);
		default:
			return std::numeric_limits<double>::quiet_NaN();
		}
	}

	Error Value::asError() const
	{
		return _impl->data.index() == ErrorIndex
			? std::get<ErrorIndex>(_impl->data)
			: Error::None;
	}
}
