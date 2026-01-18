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


#include <SilikegoCore/Operator.h>

namespace Silikego
{
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
			std::make_unique<OperatorDice>(1));
	}
}
