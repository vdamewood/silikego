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

#include <SilikegoCore/Math.h>
#include <SilikegoCore/Value.h>

namespace Silikego
{
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

	Value MathCos(const std::vector<Value>& args)
	{
		if (args.size() != 1)
			return Error::FunctionArguments;

		if (args[0].status() == ValueStatus::Error)
			return args[0];

		return std::cos(args[0].real());
	}

	Value MathSin(const std::vector<Value>& args)
	{
		if (args.size() != 1)
			return Error::FunctionArguments;

		if (args[0].status() == ValueStatus::Error)
			return args[0];

		return std::sin(args[0].real());
	}

	Value MathTan(const std::vector<Value>& args)
	{
		if (args.size() != 1)
			return Error::FunctionArguments;

		if (args[0].status() == ValueStatus::Error)
			return args[0];

		return std::tan(args[0].real());
	}
}
