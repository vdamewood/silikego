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
	Value Functions::add(std::vector<Value> args)
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
			{
				result = static_cast<long long int>(result)
					+ static_cast<long long int>(*i);
			}
			else
			{
				result = static_cast<double>(result)
					+ static_cast<double>(*i);
			}
		}
		return result;
	}

	Value Functions::subtract(std::vector<Value> args)
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
			{
				result = static_cast<long long int>(result)
					- static_cast<long long int>(*i);
			}
			else
			{
				result = static_cast<double>(result)
					- static_cast<double>(*i);
			}
		}
		return result;
	}

	Value Functions::multiply(std::vector<Value> args)
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
			{
				result = static_cast<long long int>(result)
					* static_cast<long long int>(*i);
			}
			else
			{
				result = static_cast<double>(result)
					* static_cast<double>(*i);
			}
		}
		return result;
	}

	Value Functions::divide(std::vector<Value> args)
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

			if(static_cast<double>(*i) == 0.0)
				return Error::ZeroDivision;

			if(result.status() == ValueStatus::Integer
				&& i->status() == ValueStatus::Integer
				&& static_cast<long long int>(result)
					% static_cast<long long int>(*i)
					== 0)
			{
				result = static_cast<long long int>(result)
					/ static_cast<long long int>(*i);
			}
			else
			{
				result = static_cast<double>(result)
					/ static_cast<double>(*i);
			}
		}
		return result;
	}

	Value Functions::power(std::vector<Value> args)
	{
		if (args.size() < 1)
			return Error::FunctionArguments;

		if (args[0].status() == ValueStatus::Error)
			return args[0];

		double result = static_cast<double>(args[0]);
		for (auto i = args.begin()+1; i != args.end(); i++)
		{
			if (i->status() == ValueStatus::Error)
				return *i;
			result = std::pow(result, *i);
		}
		return result;
	}

	Value Functions::dice(std::vector<Value> args)
	{
		if(args.size() != 2)
			return Error::FunctionArguments;

		if (args[0].status() == ValueStatus::Error)
			return args[0];

		if (args[1].status() == ValueStatus::Error)
			return args[1];
			
		long long int count = static_cast<long long int>(args[0]);
		long long int faces = static_cast<long long int>(args[1]);

		if (faces == 0)
			return 0;

		static int has_seeded = 0;
		if (!has_seeded)
		{
			has_seeded = 1;
			std::srand((unsigned int)std::time(nullptr));
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
		case ValueStatus::Integer:
			return std::abs(static_cast<long long int>(args[0]));
		case ValueStatus::Real:
			return std::abs(static_cast<double>(args[0]));
		default:
			return args[0];
		}
	}

	Value Functions::acos(std::vector<Value> args)
	{
		if (args.size() != 1)
			return Error::FunctionArguments;

		if (args[0].status() == ValueStatus::Error)
			return args[0];

		double input = static_cast<double>(args[0]);
		if (input < -1.0 || input > 1.0)
			return Error::Domain;

		return std::acos(input);
	}

	Value Functions::asin(std::vector<Value> args)
	{
		if (args.size() != 1)
			return Error::FunctionArguments;

		if (args[0].status() == ValueStatus::Error)
			return args[0];

		double input = static_cast<double>(args[0]);
		if (input < -1.0 || input > 1.0)
			return Error::Domain;

		return std::asin(input);
	}

	Value Functions::atan(std::vector<Value> args)
	{
		if (args.size() != 1)
			return Error::FunctionArguments;

		if (args[0].status() == ValueStatus::Error)
			return args[0];

		return std::atan(static_cast<double>(args[0]));
	}

	Value Functions::ceil(std::vector<Value> args)
	{
		if (args.size() != 1)
			return Error::FunctionArguments;

		switch(args[0].status())
		{
		case ValueStatus::Error:
		case ValueStatus::Integer:
			return args[0];
		case ValueStatus::Real:
		{
			double result = std::ceil(static_cast<double>(args[0]));
			if (result <= std::numeric_limits<long long int>::max()
					&& result
						>= std::numeric_limits<long long int>::min())
				return static_cast<long long int>(result);
			return result;
		}
		}
	}

	Value Functions::cos(std::vector<Value> args)
	{
		if (args.size() != 1)
			return Error::FunctionArguments;
		
		if (args[0].status() == ValueStatus::Error)
			return args[0];

		return std::cos(static_cast<double>(args[0]));
	}

	Value Functions::cosh(std::vector<Value> args)
	{
		if (args.size() != 1)
			return Error::FunctionArguments;
		
		if (args[0].status() == ValueStatus::Error)
			return args[0];

		return std::cosh(static_cast<double>(args[0]));
	}

	Value Functions::exp(std::vector<Value> args)
	{
		if (args.size() != 1)
			return Error::FunctionArguments;
		
		if (args[0].status() == ValueStatus::Error)
			return args[0];

		return std::exp(static_cast<double>(args[0]));
	}

	Value Functions::floor(std::vector<Value> args)
	{
		if (args.size() != 1)
			return Error::FunctionArguments;

		switch(args[0].status())
		{
		case ValueStatus::Error:
		case ValueStatus::Integer:
			return args[0];
		case ValueStatus::Real:
		{
			double result = std::floor(static_cast<double>(args[0]));
			if (result <= std::numeric_limits<long long int>::max()
					&& result
						>= std::numeric_limits<long long int>::min())
				return static_cast<long long int>(result);
			return result;
		}
		}
	}

	Value Functions::log(std::vector<Value> args)
	{
		if (args.size() != 1)
			return Error::FunctionArguments;
		
		if (args[0].status() == ValueStatus::Error)
			return args[0];

		return std::log(static_cast<double>(args[0]));
	}

	Value Functions::log10(std::vector<Value> args)
	{
		if (args.size() != 1)
			return Error::FunctionArguments;
		
		if (args[0].status() == ValueStatus::Error)
			return args[0];

		return std::log10(static_cast<double>(args[0]));
	}

	Value Functions::sin(std::vector<Value> args)
	{
		if (args.size() != 1)
			return Error::FunctionArguments;
		
		if (args[0].status() == ValueStatus::Error)
			return args[0];

		return std::sin(static_cast<double>(args[0]));
	}

	Value Functions::sinh(std::vector<Value> args)
	{
		if (args.size() != 1)
			return Error::FunctionArguments;
		
		if (args[0].status() == ValueStatus::Error)
			return args[0];

		return std::sinh(static_cast<double>(args[0]));
	}

	Value Functions::sqrt(std::vector<Value> args)
	{
		if (args.size() != 1)
			return Error::FunctionArguments;
		
		if (args[0].status() == ValueStatus::Error)
			return args[0];

		if (static_cast<double>(args[0]) < 0.0)
			return Error::Domain;

		return std::sqrt(static_cast<double>(args[0]));
	}

	Value Functions::tan(std::vector<Value> args)
	{
		if (args.size() != 1)
			return Error::FunctionArguments;
		
		if (args[0].status() == ValueStatus::Error)
			return args[0];

		return std::tan(static_cast<double>(args[0]));
	}

	Value Functions::tanh(std::vector<Value> args)
	{
		if (args.size() != 1)
			return Error::FunctionArguments;
		
		if (args[0].status() == ValueStatus::Error)
			return args[0];

		return std::tanh(static_cast<double>(args[0]));
	}
}
