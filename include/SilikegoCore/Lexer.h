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


#if !defined SILIKEGO_CORE_LEXER_H
#define SILIKEGO_CORE_LEXER_H

#include <memory>

#include <SilikegoCore/Api.h>
#include <SilikegoCore/Input.h>
#include <SilikegoCore/Token.h>

namespace Silikego
{
	class SILIKEGOCORE_EXPORT Lexer
	{
		class Impl;
		Impl* _impl;
	public:
		Lexer(std::unique_ptr<Input> source, bool support_dice);
		Lexer(const Lexer&) = delete;
		Lexer(Lexer&&) = delete;
		~Lexer();

		inline bool isEmpty() const
		{
			return _impl == nullptr;
		};

		Lexer& operator=(const Lexer&) = delete;
		Lexer& operator=(Lexer&&) = delete;

		void advance();
		Silikego::Token* token();
	};
};

#endif // SILIKEGO_CORE_LEXER_H
