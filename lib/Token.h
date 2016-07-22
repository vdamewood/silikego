/* Token.h: Token passed from tokenizer to parser
 * Copyright 2014, 2015, 2016 Vincent Damewood
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

#if !defined SILIKEGO_TOKEN_H
#define SILIKEGO_TOKEN_H

#include <string>

#include "W32Dll.h"

namespace Silikego
{
	class SILIKEGO_API Token
	{
	public:
		enum TokenType
		{
			ERROR = -1,
			UNSET = 0,
			LPAREN = '(',
			RPAREN = ')',
			MULTIPLY = '*',
			ADDITION = '+',
			COMMA = ',',
			SUBTRACT = '-',
			DIVISION = '/',
			EXPONENT = '^',
			DICE = 'd',
			INTEGER = 256,
			FLOAT,
			ID,
			EOL
		};

		Token(TokenType NewType);
		Token(int NewInteger);
		Token(float NewFloat);
		Token(const char *NewString);
		~Token();

		TokenType   Type() const;
		int         Integer() const;
		float       Float() const;
		const char *Id() const;

	private:
		TokenType MyType;
		union
		{
			int MyInteger;
			float MyFloat;
			char *MyId;
		};
	};
};

#endif // SILIKEGO_TOKEN_H