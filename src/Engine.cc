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


#include <memory>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include <SilikegoCore/Engine.h>
#include <SilikegoCore/Function.h>
#include <SilikegoCore/Value.h>

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

	Engine::Engine() : _impl(new(std::nothrow) Impl) { }

	Engine::~Engine()
	{
		delete _impl;
	}

	void Engine::installFunction(
		const std::string& name,
		std::unique_ptr<Function> pointer)
	{
		if (isEmpty())
			return;

		_impl->lookup[name] = std::move(pointer);
	}

	Function* Engine::fetchFunction(
		const std::string& name)
	{
		if (isEmpty() || _impl->lookup.count(name) == 0)
			return nullptr;

		return _impl->lookup[name].get();
	}

	Value Engine::callFunction(
		const std::string& name,
		std::vector<Value> args)
	{
		if (isEmpty())
			return Error::NullObject;

		if (_impl->lookup.count(name) == 0)
			return Error::FunctionName;

		return (*_impl->lookup[name])(args);
	}
}
