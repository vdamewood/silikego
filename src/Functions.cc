/* Functions.cc: Built-in functions
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

#include <cmath>
#include <cstdlib>
#include <ctime>
#include <limits>

#include <SilikegoCore/Value.h>

#include "Functions.h"

namespace Silikego
{
	Value Functions::add(std::vector<Value> Args)
	{
		if (Args.size() == 0)
			return 0;

		if (Args[0].isError())
			return Args[0];

		Value rVal = Args[0];
		for (auto i = Args.begin()+1; i != Args.end(); i++)
			switch (rVal.status())
			{
			case ValueStatus::Integer:
				switch(i->status())
				{
				case ValueStatus::Integer:
					rVal = rVal.asInteger() + i->asInteger();
					break;
				case ValueStatus::Float:
					rVal = rVal.asInteger() + i->asFloat();
					break;
				default:
					return *i;
				}
				break;
			case ValueStatus::Float:
				switch(i->status())
				{
				case ValueStatus::Integer:
					rVal = rVal.asFloat() + i->asInteger();
					break;
				case ValueStatus::Float:
					rVal = rVal.asFloat() + i->asFloat();
					break;
				default:
					return *i;
				}
				break;
			default: // shouldn't happen
				return Error::FunctionArguments;
			}
		return rVal;
	}

	Value Functions::subtract(std::vector<Value> Args)
	{
		if (!Args.size())
			return 0;

		if (Args[0].isError())
			return Args[0];

		Value rVal = Args[0];
		for (auto i = Args.begin()+1; i != Args.end(); i++)
			switch (rVal.status())
			{
			case ValueStatus::Integer:
				switch(i->status())
				{
				case ValueStatus::Integer:
					rVal = rVal.asInteger() - i->asInteger();
					break;
				case ValueStatus::Float:
					rVal = rVal.asInteger() - i->asFloat();
					break;
				default:
					return *i;
				}
				break;
			case ValueStatus::Float:
				switch(i->status())
				{
				case ValueStatus::Integer:
					rVal = rVal.asFloat() - i->asInteger();
					break;
				case ValueStatus::Float:
					rVal = rVal.asFloat() - i->asFloat();
					break;
				default:
					return *i;
				}
				break;
			default: // shouldn't happen
				return Error::FunctionArguments;
			}
		return rVal;
	}

	Value Functions::multiply(std::vector<Value> Args)
	{
		if (!Args.size())
			return 0;

		if (Args[0].isError())
			return Args[0];

		Value rVal = Args[0];
		for (auto i = Args.begin()+1; i != Args.end(); i++)
			switch (rVal.status())
			{
			case ValueStatus::Integer:
				switch(i->status())
				{
				case ValueStatus::Integer:
					rVal = rVal.asInteger() * i->asInteger();
					break;
				case ValueStatus::Float:
					rVal = rVal.asInteger() * i->asFloat();
					break;
				default:
					return Error::FunctionArguments;
				}
				break;
			case ValueStatus::Float:
				switch(i->status())
				{
				case ValueStatus::Integer:
					rVal = rVal.asFloat() * i->asInteger();
					break;
				case ValueStatus::Float:
					rVal = rVal.asFloat() * i->asFloat();
					break;
				default:
					return Error::FunctionArguments;
				}
				break;
			default:
				return Error::FunctionArguments;
			}
		return rVal;	}

	Value Functions::divide(std::vector<Value> Args)
	{
		if (Args.size() < 2)
			return Error::FunctionArguments;

		if (Args[0].isError())
			return Args[0];

		Value rVal = Args[0];
		for (auto i = Args.begin()+1; i != Args.end(); i++)
		{
			/* Division-by-Zero Error */
			if ((i->status() == ValueStatus::Float && i->asFloat() == 0.0)
				|| (i->status() == ValueStatus::Integer && i->asInteger() == 0))
			{
				return Error::ZeroDivision;
			}

			switch (rVal.status())
			{
			case ValueStatus::Integer:
				switch(i->status())
				{
				case ValueStatus::Integer:
					if (rVal.asInteger() % i->asInteger() == 0)
						rVal = rVal.asInteger() / i->asInteger();
					else
						rVal = static_cast<double>(rVal.asInteger())
							/ static_cast<double>(i->asInteger());
					break;
				case ValueStatus::Float:
					rVal = rVal.asInteger() / i->asFloat();
					break;
				default:
					return Error::FunctionArguments;
				}
				break;
			case ValueStatus::Float:
				switch(i->status())
				{
				case ValueStatus::Integer:
					rVal = rVal.asFloat() / i->asInteger();
					break;
				case ValueStatus::Float:
					rVal = rVal.asFloat() / i->asFloat();
					break;
				default:
					return Error::FunctionArguments;
				}
				break;
			default:
				return Error::FunctionArguments;
			}
		}
		return rVal;
	}

	Value Functions::power(std::vector<Value> args)
	{
		if (args.size() == 0)
			return Error::ZeroDivision;

		if (args[0].isError())
			return args[0];

		double result = args[0].asFloat();
		for (auto i = args.begin()+1; i != args.end(); i++)
			result = std::pow(result, i->asFloat());
		return result;
	}


	Value Functions::dice(std::vector<Value> args)
	{
		if(args.size() != 2)
			return Error::FunctionArguments;

		long long int count = args[0].asInteger();
		long long int faces = args[1].asInteger();
		if (faces == 0)
			return 0;

		static int hasSeeded = 0;
		if (!hasSeeded)
		{
			hasSeeded = 1;
			std::srand((unsigned int)std::time(NULL));
		}

		long long int result = 0;
		for (int i = 1; i <= count; i++)
			result += (std::rand() % faces) + 1;
		return result;
	}

	Value Functions::abs(std::vector<Value> args)
	{
		if (args.size() != 1)
			return Error::FunctionArguments;

		switch (args[0].status())
		{
		case ValueStatus::Float:
			return std::abs(args[0].asFloat());
		case ValueStatus::Integer:
			return std::abs(args[0].asInteger());
		default:
			return args[0];
		}
	}

	Value Functions::acos(std::vector<Value> args)
	{
		if (args.size() != 1)
			return Error::FunctionArguments;

		if (args[0].asFloat() < -1 || args[0].asFloat() > 1)
			return Error::Domain;

		return std::acos(args[0].asFloat());
	}

	Value Functions::asin(std::vector<Value> args)
	{
		if (args.size() != 1)
			return Error::FunctionArguments;

		if (args[0].asFloat() < -1 || args[0].asFloat() > 1)
			return Error::Domain;

		return std::asin(args[0].asFloat());
	}

	Value Functions::atan(std::vector<Value> args)
	{
		if (args.size() != 1)
			return Error::FunctionArguments;

		return std::atan(args[0].asFloat());
	}

	Value Functions::ceil(std::vector<Value> args)
	{
		if (args.size() != 1)
			return Error::FunctionArguments;

		double result = std::ceil(args[0].asFloat());
		if (result <= std::numeric_limits<long long int>::max()
				&& result >= std::numeric_limits<long long int>::min())
			return static_cast<long long int>(result);
		else
			return result;
	}

	Value Functions::cos(std::vector<Value> args)
	{
		if (args.size() != 1)
			return Error::FunctionArguments;

		return std::cos(args[0].asFloat());
	}

	Value Functions::cosh(std::vector<Value> args)
	{
		if (args.size() != 1)
			return Error::FunctionArguments;

		return std::cosh(args[0].asFloat());
	}

	Value Functions::exp(std::vector<Value> args)
	{
		if (args.size() != 1)
			return Error::FunctionArguments;

		return std::exp(args[0].asFloat());
	}

	Value Functions::floor(std::vector<Value> args)
	{
		if (args.size() != 1)
			return Error::FunctionArguments;

		double result = std::floor(args[0].asFloat());

		if (result <= std::numeric_limits<long long int>::max()
				&& result >= std::numeric_limits<long long int>::min())
			return static_cast<long long int>(result);
		else
			return result;
	}

	Value Functions::log(std::vector<Value> args)
	{
		if (args.size() != 1)
			return Error::FunctionArguments;

		return std::log(args[0].asFloat());
	}

	Value Functions::log10(std::vector<Value> args)
	{
		if (args.size() != 1)
			return Error::FunctionArguments;

		return std::log10(args[0].asFloat());
	}

	Value Functions::sin(std::vector<Value> args)
	{
		if (args.size() != 1)
			return Error::FunctionArguments;

		return std::sin(args[0].asFloat());
	}

	Value Functions::sinh(std::vector<Value> args)
	{
		if (args.size() != 1)
			return Error::FunctionArguments;

		return std::sinh(args[0].asFloat());
	}

	Value Functions::sqrt(std::vector<Value> args)
	{
		if (args.size() != 1)
			return Error::FunctionArguments;

		if (args[0].asFloat() < 0.0)
			return Error::Domain;

		return std::sqrt(args[0].asFloat());
	}

	Value Functions::tan(std::vector<Value> args)
	{
		if (args.size() != 1)
			return Error::FunctionArguments;

		return std::tan(args[0].asFloat());
	}

	Value Functions::tanh(std::vector<Value> args)
	{
		if (args.size() != 1)
			return Error::FunctionArguments;

		return std::tanh(args[0].asFloat());
	}
}
