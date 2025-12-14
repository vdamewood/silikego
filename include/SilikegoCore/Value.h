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
		Real,
	};

	class SILIKEGOCORE_EXPORT Value
	{
	public:
		Value() = delete;

		Value(Error source);
		Value(int source);
		Value(long long int source);
		Value(double source);
		Value(const Value& source);
		Value(Value&& source);
		~Value();

		operator Error() const;
		operator long long int() const;
		operator double() const;

		Value& operator=(Error source);
		Value& operator=(int source);
		Value& operator=(long long int source);
		Value& operator=(double source);
		Value& operator=(const Value& source);
		Value& operator=(Value&& source);

		ValueStatus status() const;
		Value& negate();

	private:
		class Impl;
		Impl* _impl;
	};
}

#endif // SILIKEGO_CORE_VALUE_H
