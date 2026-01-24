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


#if !defined SILIKEGO_CORE_VALUE_H
#define SILIKEGO_CORE_VALUE_H

#include <SilikegoCore/Api.h>

namespace Silikego
{
	enum class Error
	{
		None,
		NullObject,
		Syntax,
		ZeroDivision,
		FunctionName,
		FunctionArguments,
		Domain,
		Range
	};

	enum class ValueStatus
	{
		Error,
		Integer,
		Real,
	};

	class SILIKEGOCORE_EXPORT Value
	{
		class Impl;
		Impl* _impl;
	public:
		Value();
		Value(Error source);
		Value(int source);
		Value(long long int source);
		Value(double source);
		Value(const Value& source);
		Value(Value&& source);
		~Value();

		Value& operator=(Error right_side);
		Value& operator=(int right_side);
		Value& operator=(long long int right_side);
		Value& operator=(double right_side);
		Value& operator=(const Value& right_side);
		Value& operator=(Value&& right_side);

		inline bool isEmpty() const
		{
			return _impl == nullptr;
		};

		ValueStatus status() const;
		Error error() const;
		long long int integer() const;
		double real() const;
		void negate();
	};
}

#endif // SILIKEGO_CORE_VALUE_H
