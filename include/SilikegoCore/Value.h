/* Value.h: Data structure to represent a single value
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

#if !defined SILIKEGO_CORE_VALUE_H
#define SILIKEGO_CORE_VALUE_H

#include <SilikegoCore/Api.h>

namespace Silikego
{
	enum class Error
	{
		None,
		Memory,
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
		Float,
	};

	class SILIKEGOCORE_EXPORT Value
	{
	public:
		Value() = delete;

		Value(Error);
		Value(short int);
		Value(int);
		Value(long int);
		Value(long long int);
		Value(float);
		Value(double);

		Value(const Value&);
		Value(Value&&);
		~Value();

		Value& operator=(Error right_side);
		Value& operator=(long long int right_side);
		Value& operator=(double right_side);
		Value& operator=(const Value&);
		Value& operator=(Value&&);

		Value& negate();
		ValueStatus status() const;
		long long int asInteger() const;
		double asFloat() const;
		Error asError() const;

	private:
		class Impl;
		Impl* _impl;
	};
}

#endif // SILIKEGO_CORE_VALUE_H
