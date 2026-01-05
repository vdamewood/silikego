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


#if !defined SILIKEGO_CORE_FUNCTION_CALLER_H
#define SILIKEGO_CORE_FUNCTION_CALLER_H

#include <string>
#include <vector>

#include <SilikegoCore/Api.h>
#include <SilikegoCore/Function.h>
#include <SilikegoCore/Value.h>


namespace Silikego
{
	//typedef Value (*FunctionPointer)(std::vector<Value>);

	class SILIKEGOCORE_EXPORT FunctionCaller
	{
	public:
		FunctionCaller();

		FunctionCaller(const FunctionCaller&) = delete;
		FunctionCaller(FunctionCaller&&) = default;
		~FunctionCaller();

		FunctionCaller& operator=(const FunctionCaller&) = delete;
		FunctionCaller& operator=(FunctionCaller&&) = default;

		Value call(
			const std::string &function_name,
			std::vector<Value> arguments);
		Function* fetch(
			const std::string& function_name);
		void install(
			const std::string &function_name,
			std::unique_ptr<Function> function_pointer);

	private:
		class Impl;
		Impl* _impl;
	};

	SILIKEGOCORE_EXPORT void InstallOperators(FunctionCaller&);
	SILIKEGOCORE_EXPORT void InstallFunctions(FunctionCaller&);
};
#endif // SILIKEGO_CORE_FUNCTION_CALLER_H
