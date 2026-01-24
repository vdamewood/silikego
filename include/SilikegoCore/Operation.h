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

#if !defined SILIKEGO_CORE_OPERATORS_H
#define SILIKEGO_CORE_OPERATORS_H

#include <vector>

#include <SilikegoCore/Api.h>
#include <SilikegoCore/Engine.h>
#include <SilikegoCore/Value.h>

namespace Silikego
{
	SILIKEGOCORE_EXPORT
	Silikego::Value OperationAdd(
		const std::vector<Silikego::Value>& args);

	SILIKEGOCORE_EXPORT
	Silikego::Value OperationSubtract(
		const std::vector<Silikego::Value>& args);

	SILIKEGOCORE_EXPORT
	Silikego::Value OperationMultiply(
		const std::vector<Silikego::Value>& args);

	SILIKEGOCORE_EXPORT
	Silikego::Value OperationDivide(
		const std::vector<Silikego::Value>& args);

	SILIKEGOCORE_EXPORT
	Silikego::Value OperationPower(
		const std::vector<Silikego::Value>& args);

	class SILIKEGOCORE_EXPORT OperationDice : public Function
    {
        class Impl;
        Impl* _impl;
	public:
        OperationDice(unsigned long long int seed);
        ~OperationDice() override;

        Value operator()(const std::vector<Value>& args) override;

        bool isEmpty() const override;
	};

SILIKEGOCORE_EXPORT
	void InstallOperations(Engine& destination);
};

#endif // SILIKEGO_CORE_OPERATORS_H
