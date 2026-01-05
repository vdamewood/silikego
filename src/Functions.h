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


#if !defined SILIKEGO_FUNCTIONS_H
#define SILIKEGO_FUNCTIONS_H

#include <vector>

#include <SilikegoCore/Value.h>

namespace Silikego
{
	namespace Functions
	{
		Silikego::Value add(const std::vector<Silikego::Value>& Args);
		Silikego::Value subtract(const std::vector<Silikego::Value>& Args);
		Silikego::Value multiply(const std::vector<Silikego::Value>& Args);
		Silikego::Value divide(const std::vector<Silikego::Value>& Args);
		Silikego::Value power(const std::vector<Silikego::Value>& Args);
		Silikego::Value dice(const std::vector<Silikego::Value>& Args);
		Silikego::Value abs(const std::vector<Silikego::Value>& Args);
		Silikego::Value acos(const std::vector<Silikego::Value>& Args);
		Silikego::Value asin(const std::vector<Silikego::Value>& Args);
		Silikego::Value atan(const std::vector<Silikego::Value>& Args);
		Silikego::Value ceil(const std::vector<Silikego::Value>& Args);
		Silikego::Value cos(const std::vector<Silikego::Value>& Args);
		Silikego::Value cosh(const std::vector<Silikego::Value>& Args);
		Silikego::Value exp(const std::vector<Silikego::Value>& Args);
		Silikego::Value floor(const std::vector<Silikego::Value>& Args);
		Silikego::Value log(const std::vector<Silikego::Value>& Args);
		Silikego::Value log10(const std::vector<Silikego::Value>& Args);
		Silikego::Value sin(const std::vector<Silikego::Value>& Args);
		Silikego::Value sinh(const std::vector<Silikego::Value>& Args);
		Silikego::Value sqrt(const std::vector<Silikego::Value>& Args);
		Silikego::Value tan(const std::vector<Silikego::Value>& Args);;
		Silikego::Value tanh(const std::vector<Silikego::Value>& Args);
	};
};
#endif // SILIKEGO_FUNCTIONS_H
