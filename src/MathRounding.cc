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

namespace
{
	// This is the largest value that can be represented by both a
	// double and a long long int, assuming the long long int is a
	// 64-bit, signed integer, and the double is an IEEE 754-compliant
	// binary double-precision floating-point number.
	const double CommonMax = 9223372036854774784.0;
}

namespace Silikego
{
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
			if (result <= CommonMax && result >= -CommonMax)
				return static_cast<long long int>(result);
			return result;
		}
		}
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
			if (result <= CommonMax && result >= -CommonMax)
				return static_cast<long long int>(result);
			return result;
		}
		}
	}
}
