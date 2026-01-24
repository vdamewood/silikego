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


#include <cmath>
#include <cstdlib>
#include <ctime>

#include <SilikegoCore/Operation.h>
#include <SilikegoCore/Value.h>

namespace Silikego
{
	Value OperationAdd(const std::vector<Value>& args)
	{
		if (args.size() < 0)
			return Error::FunctionArguments;

		Value result = args[0];
		if (result.status() == ValueStatus::Error)
			return result;
		for (auto i = args.begin()+1; i != args.end(); i++)
		{
			if (i->status() == ValueStatus::Error)
				return *i;

			if(result.status() == ValueStatus::Integer
					&& i->status() == ValueStatus::Integer)
				result = result.integer() + i->integer();
			else
				result = result.real() + i->real();
		}
		return result;
	}

	Value OperationSubtract(const std::vector<Value>& args)
	{
		if (args.size() < 0)
			return Error::FunctionArguments;

		Value result = args[0];
		if (result.status() == ValueStatus::Error)
			return result;
		for (auto i = args.begin()+1; i != args.end(); i++)
		{
			if (i->status() == ValueStatus::Error)
				return *i;

			if(result.status() == ValueStatus::Integer
					&& i->status() == ValueStatus::Integer)
				result = result.integer() - i->integer();
			else
				result = result.real() - i->real();
		}
		return result;
	}

	Value OperationMultiply(const std::vector<Value>& args)
	{
		if (args.size() < 0)
			return Error::FunctionArguments;

		Value result = args[0];
		if (result.status() == ValueStatus::Error)
			return result;
		for (auto i = args.begin()+1; i != args.end(); i++)
		{
			if (i->status() == ValueStatus::Error)
				return *i;

			if(result.status() == ValueStatus::Integer
					&& i->status() == ValueStatus::Integer)
				result = result.integer() * i->integer();
			else
				result = result.real() * i->real();
		}
		return result;
	}

	Value OperationDivide(const std::vector<Value>& args)
	{
		if (args.size() < 0)
			return Error::FunctionArguments;

		Value result = args[0];
		if (result.status() == ValueStatus::Error)
			return result;
		for (auto i = args.begin()+1; i != args.end(); i++)
		{
			if (i->status() == ValueStatus::Error)
				return *i;

			if(i->real() == 0.0)
				return Error::ZeroDivision;

			if(result.status() == ValueStatus::Integer
					&& i->status() == ValueStatus::Integer
					&& result.integer()
						% i->integer()
						== 0)
				result = result.integer() / i->integer();
			else
				result = result.real() / i->real();
		}
		return result;
	}

	Value OperationPower(const std::vector<Value>& args)
	{
		if (args.size() < 1)
			return Error::FunctionArguments;

		if (args[0].status() == ValueStatus::Error)
			return args[0];

		double result = args[0].real();
		for (auto i = args.begin()+1; i != args.end(); i++)
		{
			if (i->status() == ValueStatus::Error)
				return *i;
			result = std::pow(result, i->real());
		}
		return result;
	}
}
