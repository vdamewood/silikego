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


#if !defined SILIKEGO_CORE_ENGINE_H
#define SILIKEGO_CORE_ENGINE_H

#include <string>
#include <vector>

#include <SilikegoCore/Api.h>
#include <SilikegoCore/Function.h>
#include <SilikegoCore/Value.h>

namespace Silikego
{
	class SILIKEGOCORE_EXPORT Engine
	{
		class Impl;
		Impl* _impl;
	public:
		Engine();
		Engine(const Engine&) = delete;
		Engine(Engine&&) = delete;
		~Engine();

		Engine& operator=(const Engine&) = delete;
		Engine& operator=(Engine&&) = delete;

		Value callFunction(
			const std::string& name,
			std::vector<Value> args);
		Function* fetchFunction(
			const std::string& name);
		void installFunction(
			const std::string& name,
			std::unique_ptr<Function> function);
	};
};

#endif // SILIKEGO_CORE_ENGINE_H
