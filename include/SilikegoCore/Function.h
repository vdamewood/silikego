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


#if !defined SILIKEGO_CORE_FUNCTION_H
#define SILIKEGO_CORE_FUNCTION_H

#include <vector>

#include <SilikegoCore/Api.h>
#include <SilikegoCore/Value.h>

namespace Silikego
{
    class SILIKEGOCORE_EXPORT Function
    {
    public:
        Function();
		Function(const Function&) = delete;
		Function(Function&&) = delete;
        virtual ~Function();
        virtual Function* clone() = 0;
        virtual Value operator()(const std::vector<Value>& args) = 0;
        virtual bool isEmpty() const = 0;
    };

    class SILIKEGOCORE_EXPORT PureFunction : public Function
    {
        class Impl;
        Impl *_impl;
    public:
        PureFunction(Value (*function)(const std::vector<Value>& args));
        ~PureFunction() override;
        Function* clone() override;

        Value operator()(const std::vector<Value>& args) override;

        bool isEmpty() const override;
    };
};

#endif // SILIKEGO_CORE_FUNCTIONS_H