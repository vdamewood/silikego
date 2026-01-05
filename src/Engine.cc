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


#include <unordered_map>
#include <stdexcept>
#include <string>
#include <vector>

#include <SilikegoCore/Engine.h>
#include <SilikegoCore/Value.h>

#include "Functions.h"

namespace Silikego
{
	class Engine::Impl
	{
	public:
		std::unordered_map<
			std::string,
			std::unique_ptr<Function>
		> lookup;
	};

	Engine::Engine(): _impl(new Impl) { }

	Engine::~Engine()
	{
		delete _impl;
	}

	void Engine::installFunction(
		const std::string& name,
		std::unique_ptr<Function> pointer)
	{
		_impl->lookup[name] = std::move(pointer);
	}

	Function* Engine::fetchFunction(
		const std::string& name)
	{
		try
		{
			return _impl->lookup.at(name).get();
		}
		catch (const std::out_of_range&)
		{
			return nullptr;
		}
	}

	Value Engine::callFunction(
		const std::string& name,
		std::vector<Value> args)
	try
	{
		return (*_impl->lookup.at(name))(args);
	}
	catch (const std::out_of_range&)
	{
		return Error::FunctionName;
	}

	void InstallOperators(Engine& engine)
	{
		engine.installFunction("add",
			std::make_unique<PureFunction>(Functions::add));
		engine.installFunction("subtract",
			std::make_unique<PureFunction>(Functions::subtract));
		engine.installFunction("multiply",
			std::make_unique<PureFunction>(Functions::multiply));
		engine.installFunction("divide",
			std::make_unique<PureFunction>(Functions::divide));
		engine.installFunction("power",
			std::make_unique<PureFunction>(Functions::power));
		engine.installFunction("dice",
			std::make_unique<PureFunction>(Functions::dice));
	}

	void InstallMathFunctions(Engine& engine)
	{
		engine.installFunction("abs",
			std::make_unique<PureFunction>(Functions::abs));
		engine.installFunction("acos",
			std::make_unique<PureFunction>(Functions::acos));
		engine.installFunction("asin",
			std::make_unique<PureFunction>(Functions::asin));
		engine.installFunction("atan",
			std::make_unique<PureFunction>(Functions::atan));
		engine.installFunction("ceil",
			std::make_unique<PureFunction>(Functions::ceil));
		engine.installFunction("cos",
			std::make_unique<PureFunction>(Functions::cos));
		engine.installFunction("cosh",
			std::make_unique<PureFunction>(Functions::cosh));
		engine.installFunction("exp",
			std::make_unique<PureFunction>(Functions::exp));
		engine.installFunction("floor",
			std::make_unique<PureFunction>(Functions::floor));
		engine.installFunction("log",
			std::make_unique<PureFunction>(Functions::log));
		engine.installFunction("log10",
			std::make_unique<PureFunction>(Functions::log10));
		engine.installFunction("sin",
			std::make_unique<PureFunction>(Functions::sin));
		engine.installFunction("sinh",
			std::make_unique<PureFunction>(Functions::sinh));
		engine.installFunction("sqrt",
			std::make_unique<PureFunction>(Functions::sqrt));
		engine.installFunction("tan",
			std::make_unique<PureFunction>(Functions::tan));
		engine.installFunction("tanh",
			std::make_unique<PureFunction>(Functions::tanh));
	}
}
