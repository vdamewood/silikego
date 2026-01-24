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


#include <SilikegoCore/Operation.h>

namespace Silikego
{
	void InstallOperations(Engine& engine)
	{
		if (engine.isEmpty())
			return;

		engine.installFunction("add",
			std::make_unique<PureFunction>(OperationAdd));
		engine.installFunction("subtract",
			std::make_unique<PureFunction>(OperationSubtract));
		engine.installFunction("multiply",
			std::make_unique<PureFunction>(OperationMultiply));
		engine.installFunction("divide",
			std::make_unique<PureFunction>(OperationDivide));
		engine.installFunction("power",
			std::make_unique<PureFunction>(OperationPower));
		engine.installFunction("dice",
			std::make_unique<OperationDice>(1));
	}
}
