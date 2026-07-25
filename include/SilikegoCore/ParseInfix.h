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


#if !defined SILIKEGO_CORE_PARSE_INFIX_H
#define SILIKEGO_CORE_PARSE_INFIX_H

#include <memory>

#include <SilikegoCore/Api.h>
#include <SilikegoCore/Lexer.h>
#include <SilikegoCore/Node.h>

namespace Silikego
{
	SILIKEGOCORE_EXPORT
	Node ParseInfix(Lexer&& lexer);
};

#endif // SILIKEGO_CORE_PARSE_INFIX_H
