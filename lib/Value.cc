/* Value.cc: Data structure to represent a single value
 * Copyright 2014, 2015, 2016 Vincent Damewood
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

#include "Value.h"

namespace Silikego
{
	class Value::State
	{
	public:
		State(Value::ValueStatus NewStatus) : Status(NewStatus), Integer(0) { }
		State(int NewInteger) : Status(INTEGER), Integer(NewInteger) { }
		State(double NewFloat) : Status(FLOAT), Float(NewFloat) { }

		ValueStatus Status;
		union
		{
			int Integer;
			double Float;
		};
	};

	Value::Value() : S(new State(0)) { }
	Value::Value(Value::ValueStatus NewStatus) : S(new State(NewStatus)) { }
	Value::Value(int NewValue) : S(new State(NewValue)) { }
	Value::Value(double NewValue) : S(new State(NewValue)) { }
	Value::~Value()
	{
		delete S;
	}

	Value::ValueStatus Value::Status() const
	{
		return S->Status;
	}

	int Value::Integer() const
	{
		if (S->Status == INTEGER)
			return S->Integer;
		else if (S->Status == FLOAT)
			return static_cast<int>(S->Float);
		else
			return 0;
	}

	double Value::Float() const
	{
		if (S->Status == INTEGER)
			return static_cast<double>(S->Integer);
		else if (S->Status == FLOAT)
			return S->Float;
		else
			return std::numeric_limits<double>::quiet_NaN();
	}

	bool Value::IsNumber() const
	{
		return (S->Status == INTEGER || S->Status == FLOAT);
	}

	std::string Value::ToString() const
	{
		std::ostringstream tmp;
		switch (S->Status)
		{
		case INTEGER:
			tmp << Integer();
			break;
		case FLOAT:
			tmp << Float();
			break;
		case MEMORY_ERR:
			tmp << "Error: Out of memory";
			break;
		case SYNTAX_ERR:
			tmp << "Error: Syntax error";
			break;
		case ZERO_DIV_ERR:
			tmp << "Error: Division by zero";
			break;
		case BAD_FUNCTION:
			tmp << "Error: Function not found";
			break;
		case BAD_ARGUMENTS:
			tmp << "Error: Bad argument count";
			break;
		case DOMAIN_ERR:
			tmp << "Error: Domain error";
			break;
		case RANGE_ERR:
			tmp << "Error: Range error";
		}

		return tmp.str();
	}
}