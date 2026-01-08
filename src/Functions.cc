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
#include <memory>

#include <SilikegoCore/Operator.h>
#include <SilikegoCore/Math.h>
#include <SilikegoCore/Value.h>

namespace Silikego
{
	Value OperatorAdd(const std::vector<Value>& args)
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

	Value OperatorSubtract(const std::vector<Value>& args)
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

	Value OperatorMultiply(const std::vector<Value>& args)
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

	Value OperatorDivide(const std::vector<Value>& args)
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

	Value OperatorPower(const std::vector<Value>& args)
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

	Value OperatorDice(const std::vector<Value>& args)
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

	Value MathAbs(const std::vector<Value>& args)
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

	Value MathAcos(const std::vector<Value>& args)
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

	Value MathAsin(const std::vector<Value>& args)
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

	Value MathAtan(const std::vector<Value>& args)
	{
		if (args.size() != 1)
			return Error::FunctionArguments;

		if (args[0].status() == ValueStatus::Error)
			return args[0];

		return std::atan(args[0].real());
	}

	Value MathCeil(const std::vector<Value>& args)
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

	Value MathCos(const std::vector<Value>& args)
	{
		if (args.size() != 1)
			return Error::FunctionArguments;
		
		if (args[0].status() == ValueStatus::Error)
			return args[0];

		return std::cos(args[0].real());
	}

	Value MathCosh(const std::vector<Value>& args)
	{
		if (args.size() != 1)
			return Error::FunctionArguments;
		
		if (args[0].status() == ValueStatus::Error)
			return args[0];

		return std::cosh(args[0].real());
	}

	Value MathExp(const std::vector<Value>& args)
	{
		if (args.size() != 1)
			return Error::FunctionArguments;
		
		if (args[0].status() == ValueStatus::Error)
			return args[0];

		return std::exp(args[0].real());
	}

	Value MathFloor(const std::vector<Value>& args)
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

	Value MathLog(const std::vector<Value>& args)
	{
		if (args.size() != 1)
			return Error::FunctionArguments;
		
		if (args[0].status() == ValueStatus::Error)
			return args[0];

		return std::log(args[0].real());
	}

	Value MathLog10(const std::vector<Value>& args)
	{
		if (args.size() != 1)
			return Error::FunctionArguments;
		
		if (args[0].status() == ValueStatus::Error)
			return args[0];

		return std::log10(args[0].real());
	}

	Value MathSin(const std::vector<Value>& args)
	{
		if (args.size() != 1)
			return Error::FunctionArguments;
		
		if (args[0].status() == ValueStatus::Error)
			return args[0];

		return std::sin(args[0].real());
	}

	Value MathSinh(const std::vector<Value>& args)
	{
		if (args.size() != 1)
			return Error::FunctionArguments;
		
		if (args[0].status() == ValueStatus::Error)
			return args[0];

		return std::sinh(args[0].real());
	}

	Value MathSqrt(const std::vector<Value>& args)
	{
		if (args.size() != 1)
			return Error::FunctionArguments;
		
		if (args[0].status() == ValueStatus::Error)
			return args[0];

		if (args[0].real() < 0.0)
			return Error::Domain;

		return std::sqrt(args[0].real());
	}

	Value MathTan(const std::vector<Value>& args)
	{
		if (args.size() != 1)
			return Error::FunctionArguments;
		
		if (args[0].status() == ValueStatus::Error)
			return args[0];

		return std::tan(args[0].real());
	}

	Value MathTanh(const std::vector<Value>& args)
	{
		if (args.size() != 1)
			return Error::FunctionArguments;
		
		if (args[0].status() == ValueStatus::Error)
			return args[0];

		return std::tanh(args[0].real());
	}

	void InstallOperators(Engine& engine)
	{
		engine.installFunction("add",
			std::make_unique<PureFunction>(OperatorAdd));
		engine.installFunction("subtract",
			std::make_unique<PureFunction>(OperatorSubtract));
		engine.installFunction("multiply",
			std::make_unique<PureFunction>(OperatorMultiply));
		engine.installFunction("divide",
			std::make_unique<PureFunction>(OperatorDivide));
		engine.installFunction("power",
			std::make_unique<PureFunction>(OperatorPower));
		engine.installFunction("dice",
			std::make_unique<PureFunction>(OperatorDice));
	}

	void InstallMathFunctions(Engine& engine)
	{
		engine.installFunction("abs",
			std::make_unique<PureFunction>(MathAbs));
		engine.installFunction("acos",
			std::make_unique<PureFunction>(MathCos));
		engine.installFunction("asin",
			std::make_unique<PureFunction>(MathSin));
		engine.installFunction("atan",
			std::make_unique<PureFunction>(MathTan));
		engine.installFunction("ceil",
			std::make_unique<PureFunction>(MathCeil));
		engine.installFunction("cos",
			std::make_unique<PureFunction>(MathCos));
		engine.installFunction("cosh",
			std::make_unique<PureFunction>(MathCosh));
		engine.installFunction("exp",
			std::make_unique<PureFunction>(MathExp));
		engine.installFunction("floor",
			std::make_unique<PureFunction>(MathFloor));
		engine.installFunction("log",
			std::make_unique<PureFunction>(MathLog));
		engine.installFunction("log10",
			std::make_unique<PureFunction>(MathLog10));
		engine.installFunction("sin",
			std::make_unique<PureFunction>(MathSin));
		engine.installFunction("sinh",
			std::make_unique<PureFunction>(MathSinh));
		engine.installFunction("sqrt",
			std::make_unique<PureFunction>(MathSqrt));
		engine.installFunction("tan",
			std::make_unique<PureFunction>(MathTan));
		engine.installFunction("tanh",
			std::make_unique<PureFunction>(MathTanh));
	}
}
