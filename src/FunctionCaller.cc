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

#include <SilikegoCore/FunctionCaller.h>
#include <SilikegoCore/Value.h>

#include "Functions.h"

namespace Silikego
{
	class FunctionCaller::Impl
	{
	public:
		std::unordered_map<
			std::string,
			std::unique_ptr<Function>
		> lookup;
	};

	FunctionCaller::FunctionCaller(): _impl(new Impl) { }

	FunctionCaller::~FunctionCaller()
	{
		delete _impl;
	}

	void FunctionCaller::install(
		const std::string& name,
		std::unique_ptr<Function> pointer)
	{
		_impl->lookup[name] = std::move(pointer);
	}

	Function* FunctionCaller::fetch(
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

	Value FunctionCaller::call(
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

	void InstallOperators(FunctionCaller& caller)
	{
		caller.install("add",
			std::make_unique<PureFunction>(Functions::add));
		caller.install("subtract",
			std::make_unique<PureFunction>(Functions::subtract));
		caller.install("multiply",
			std::make_unique<PureFunction>(Functions::multiply));
		caller.install("divide",
			std::make_unique<PureFunction>(Functions::divide));
		caller.install("power",
			std::make_unique<PureFunction>(Functions::power));
		caller.install("dice",
			std::make_unique<PureFunction>(Functions::dice));
	}

	void InstallFunctions(FunctionCaller& caller)
	{
		caller.install("abs",
			std::make_unique<PureFunction>(Functions::abs));
		caller.install("acos",
			std::make_unique<PureFunction>(Functions::acos));
		caller.install("asin",
			std::make_unique<PureFunction>(Functions::asin));
		caller.install("atan",
			std::make_unique<PureFunction>(Functions::atan));
		caller.install("ceil",
			std::make_unique<PureFunction>(Functions::ceil));
		caller.install("cos",
			std::make_unique<PureFunction>(Functions::cos));
		caller.install("cosh",
			std::make_unique<PureFunction>(Functions::cosh));
		caller.install("exp",
			std::make_unique<PureFunction>(Functions::exp));
		caller.install("floor",
			std::make_unique<PureFunction>(Functions::floor));
		caller.install("log",
			std::make_unique<PureFunction>(Functions::log));
		caller.install("log10",
			std::make_unique<PureFunction>(Functions::log10));
		caller.install("sin",
			std::make_unique<PureFunction>(Functions::sin));
		caller.install("sinh",
			std::make_unique<PureFunction>(Functions::sinh));
		caller.install("sqrt",
			std::make_unique<PureFunction>(Functions::sqrt));
		caller.install("tan",
			std::make_unique<PureFunction>(Functions::tan));
		caller.install("tanh",
			std::make_unique<PureFunction>(Functions::tanh));
	}
}
