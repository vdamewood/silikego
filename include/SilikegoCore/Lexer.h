/* Lexer.h: Lexical analyzer
 * Copyright 2012-2025 Vincent Damewood
 *
 * This library is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library. If not, see <http://www.gnu.org/licenses/>.
 */

#if !defined SILIKEGO_CORE_LEXER_H
#define SILIKEGO_CORE_LEXER_H

#include <memory>

#include <SilikegoCore/Api.h>
#include <SilikegoCore/DataSource.h>
#include <SilikegoCore/Token.h>

namespace Silikego
{
	class SILIKEGOCORE_EXPORT Lexer
	{
	public:
		Lexer() = delete;
	
		Lexer(std::unique_ptr<DataSource>);

		Lexer(const Lexer&) = delete;
		Lexer(Lexer&&) = default;
		~Lexer();

		Lexer& operator=(const Lexer&) = delete;
		Lexer& operator=(Lexer&&) = default;

		void advance();
		Silikego::Token& current();

	private:
		class Impl;
		Impl* _impl;
	};
};

#endif // SILIKEGO_CORE_LEXER_H
