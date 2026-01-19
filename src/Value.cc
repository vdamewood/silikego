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


#if defined _WIN32
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <limits>
#include <variant>

#include <SilikegoCore/Value.h>

namespace {
	enum DataIndex
	{
		ErrorIndex = 0,
		IntegerIndex = 1,
		RealIndex = 2
	};
}

namespace Silikego
{
	class Value::Impl
	{
	public:
		Impl(Error source) : data(source) { }
		Impl(long long int source) : data(source) { }
		Impl(double source) : data(source) { }

		std::variant<Error, long long int, double> data;
	};

	Value::Value(Error source)
		: _impl(new(std::nothrow) Impl(source)) { }

	Value::Value(int source)
		: _impl(new(std::nothrow) Impl(static_cast<long long int>(source))) { }

	Value::Value(long long int source)
		: _impl(new(std::nothrow) Impl(source)) { }

	Value::Value(double source)
		: _impl(new(std::nothrow) Impl(source)) { }

	Value::Value(const Value& source)
		: _impl(new(std::nothrow) Impl(*source._impl)) { }

	Value::Value(Value&& source)
		: _impl(source._impl)
	{
		source._impl = nullptr;
	}

	Value::~Value()
	{
		delete _impl;
	}

	Value& Value::operator=(Error source)
	{
		_impl->data = source;
		return *this;
	}

	Value& Value::operator=(int source)
	{
		_impl->data = static_cast<long long int>(source);
		return *this;
	}

	Value& Value::operator=(long long int source)
	{
		_impl->data = source;
		return *this;
	}

	Value& Value::operator=(double source)
	{
		_impl->data = source;
		return *this;
	}

	Value& Value::operator=(const Value& source)
	{
		_impl->data = source._impl->data;
		return *this;
	}

	Value& Value::operator=(Value&& source)
	{
		if (this != &source)
		{
			delete _impl;
			_impl = source._impl;
			source._impl = nullptr;
		}
		return *this;
	}

	ValueStatus Value::status() const
	{
		if (isEmpty())
			return ValueStatus::Error;
		switch (_impl->data.index())
		{
		case ErrorIndex:
			return ValueStatus::Error;
		case IntegerIndex:
			return ValueStatus::Integer;
		case RealIndex:
			return ValueStatus::Real;
		}
		// shouldn't happen
		return ValueStatus::Error;
	}

	Error Value::error() const
	{
		if (isEmpty())
			return Error::Internal;

		return _impl->data.index() == ErrorIndex
			? std::get<ErrorIndex>(_impl->data)
			: Error::None;
	}

	long long int Value::integer() const
	{
		if (isEmpty())
			return 0;

		switch (_impl->data.index())
		{
		case ErrorIndex:
			return 0;
		case IntegerIndex:
			return std::get<IntegerIndex>(_impl->data);
		case RealIndex:
			return static_cast<long long int>(std::get<RealIndex>(_impl->data));
		}
		// shouldn't happen
		return 0;
	}

	double Value::real() const
	{
		if (isEmpty())
			return std::numeric_limits<double>::quiet_NaN();

		switch (_impl->data.index())
		{
		case ErrorIndex:
			return std::numeric_limits<double>::quiet_NaN();
		case IntegerIndex:
			return static_cast<double>(std::get<IntegerIndex>(_impl->data));
		case RealIndex:
			return std::get<RealIndex>(_impl->data);
		}
		// shouldn't happen
		return std::numeric_limits<double>::quiet_NaN();
	}

	void Value::negate()
	{
		if (isEmpty())
			return;

		switch (_impl->data.index())
		{
		case ErrorIndex:
			break;
		case (IntegerIndex):
			std::get<IntegerIndex>(_impl->data) *= -1;
			break;
		case (RealIndex):
			std::get<RealIndex>(_impl->data) *= -1.0;
			break;
		}
	}
}
