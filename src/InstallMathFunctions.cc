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


#include <SilikegoCore/Math.h>

namespace Silikego
{
	void InstallMathFunctions(Engine& engine)
	{
		if (engine.isEmpty())
			return;

		engine.installFunction("abs",
			std::make_unique<PureFunction>(MathAbs));
		engine.installFunction("acos",
			std::make_unique<PureFunction>(MathCos));
		engine.installFunction("asin",
			std::make_unique<PureFunction>(MathSin));
		engine.installFunction("atan",
			std::make_unique<PureFunction>(MathTan));
		engine.installFunction("cbrt",
			std::make_unique<PureFunction>(MathCbrt));
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
		engine.installFunction("hypot",
			std::make_unique<PureFunction>(MathHypot));
		engine.installFunction("log",
			std::make_unique<PureFunction>(MathLog));
		engine.installFunction("log2",
			std::make_unique<PureFunction>(MathLog2));
		engine.installFunction("log10",
			std::make_unique<PureFunction>(MathLog10));
		engine.installFunction("round",
			std::make_unique<PureFunction>(MathRound));
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
		engine.installFunction("trunc",
			std::make_unique<PureFunction>(MathTrunc));
	}
}
