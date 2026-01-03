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
			FunctionPointer
		> lookup;
	};

	FunctionCaller::FunctionCaller(): _impl(new Impl) { }

	FunctionCaller::~FunctionCaller()
	{
		delete _impl;
	}

	void FunctionCaller::install(
		const std::string& name,
		FunctionPointer pointer)
	{
		_impl->lookup[name] = pointer;
	}

	FunctionPointer FunctionCaller::fetch(
		const std::string& name)
	{
		try
		{
			return _impl->lookup.at(name);
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
		return _impl->lookup.at(name)(args);
	}
	catch (const std::out_of_range&)
	{
		return Error::FunctionName;
	}

	void InstallOperators(FunctionCaller& caller)
	{
		caller.install("add", Functions::add);
		caller.install("subtract", Functions::subtract);
		caller.install("multiply", Functions::multiply);
		caller.install("divide", Functions::divide);
		caller.install("power", Functions::power);
		caller.install("dice", Functions::dice);
	}

	void InstallFunctions(FunctionCaller& caller)
	{
		caller.install("abs", Functions::abs);
		caller.install("acos", Functions::acos);
		caller.install("asin", Functions::asin);
		caller.install("atan", Functions::atan);
		caller.install("ceil", Functions::ceil);
		caller.install("cos", Functions::cos);
		caller.install("cosh", Functions::cosh);
		caller.install("exp", Functions::exp);
		caller.install("floor", Functions::floor);
		caller.install("log", Functions::log);
		caller.install("log10", Functions::log10);
		caller.install("sin", Functions::sin);
		caller.install("sinh", Functions::sinh);
		caller.install("sqrt", Functions::sqrt);
		caller.install("tan", Functions::tan);
		caller.install("tanh", Functions::tanh);
	}
}
