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
				result = result.integer() + i->integer();
			else
				result = result.real() + i->real();
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
				result = result.integer() - i->integer();
			else
				result = result.real() - i->real();
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
				result = result.integer() * i->integer();
			else
				result = result.real() * i->real();
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

	Value Functions::power(std::vector<Value> args)
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

	Value Functions::dice(std::vector<Value> args)
	{
		if(args.size() != 2)
			return Error::FunctionArguments;

		if (args[0].status() == ValueStatus::Error)
			return args[0];

		if (args[1].status() == ValueStatus::Error)
			return args[1];
			
		long long int count = args[0].integer();
		long long int faces = args[1].integer();

		if (faces == 0)
			return 0;

		static int has_seeded = 0;
		if (!has_seeded)
		{
			has_seeded = 1;
			std::srand(static_cast<unsigned int>(std::time(nullptr)));
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
			return std::abs(args[0].integer());
		case ValueStatus::Real:
			return std::fabs(args[0].real());
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

		double input = args[0].real();
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

		double input = args[0].real();
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

		return std::atan(args[0].real());
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
			double result = std::ceil(args[0].real());
			if (result <= std::numeric_limits<long long int>::max()
					&& result
						>= std::numeric_limits<long long int>::min())
				return result;
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

		return std::cos(args[0].real());
	}

	Value Functions::cosh(std::vector<Value> args)
	{
		if (args.size() != 1)
			return Error::FunctionArguments;
		
		if (args[0].status() == ValueStatus::Error)
			return args[0];

		return std::cosh(args[0].real());
	}

	Value Functions::exp(std::vector<Value> args)
	{
		if (args.size() != 1)
			return Error::FunctionArguments;
		
		if (args[0].status() == ValueStatus::Error)
			return args[0];

		return std::exp(args[0].real());
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
			double result = std::floor(args[0].real());
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

		return std::log(args[0].real());
	}

	Value Functions::log10(std::vector<Value> args)
	{
		if (args.size() != 1)
			return Error::FunctionArguments;
		
		if (args[0].status() == ValueStatus::Error)
			return args[0];

		return std::log10(args[0].real());
	}

	Value Functions::sin(std::vector<Value> args)
	{
		if (args.size() != 1)
			return Error::FunctionArguments;
		
		if (args[0].status() == ValueStatus::Error)
			return args[0];

		return std::sin(args[0].real());
	}

	Value Functions::sinh(std::vector<Value> args)
	{
		if (args.size() != 1)
			return Error::FunctionArguments;
		
		if (args[0].status() == ValueStatus::Error)
			return args[0];

		return std::sinh(args[0].real());
	}

	Value Functions::sqrt(std::vector<Value> args)
	{
		if (args.size() != 1)
			return Error::FunctionArguments;
		
		if (args[0].status() == ValueStatus::Error)
			return args[0];

		if (args[0].real() < 0.0)
			return Error::Domain;

		return std::sqrt(args[0].real());
	}

	Value Functions::tan(std::vector<Value> args)
	{
		if (args.size() != 1)
			return Error::FunctionArguments;
		
		if (args[0].status() == ValueStatus::Error)
			return args[0];

		return std::tan(args[0].real());
	}

	Value Functions::tanh(std::vector<Value> args)
	{
		if (args.size() != 1)
			return Error::FunctionArguments;
		
		if (args[0].status() == ValueStatus::Error)
			return args[0];

		return std::tanh(args[0].real());
	}
}
