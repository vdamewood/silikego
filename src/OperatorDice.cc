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


#include <random>
#include <cstdint>
#include <ctime>

#include <SilikegoCore/Function.h>
#include <SilikegoCore/Operator.h>

namespace Silikego
{
	class OperatorDice::Impl
	{
	public:
		std::mt19937_64 engine;
		Impl(unsigned long long seed)
		{
			if (seed == 0)
				seed = std::time(nullptr);
			engine.seed(seed);
		}

		Impl(const Impl& source)
		{
			engine = source.engine;
		}

		unsigned long long genrand()
		{
			return engine();
		}
	};

	OperatorDice::OperatorDice(unsigned long long int seed)
		: _impl(new Impl(seed))
	{
	}

	OperatorDice::OperatorDice(Impl *source)
		: _impl(source)
	{
	}

	Value OperatorDice::operator()(const std::vector<Value>& args)
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
			result += (_impl->genrand() % faces) + 1;
		return result;
	}

	Function* OperatorDice::clone()
	{
	    return new OperatorDice(new Impl(*this->_impl));
	}

	OperatorDice::~OperatorDice()
	{
		delete _impl;
	}
};
