/* Token.cc: Token passed from tokenizer to parser
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

// Visual Studio complains about strcpy without this.
#if defined _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif /* _MSC_VER */

#include <cstring>

#include "Token.hh"

Silikego::Token::Token(TokenType NewType)
{
	MyType = NewType;
	MyInteger = 0;
}

Silikego::Token::Token(int NewIntegerValue)
{
	MyType = INTEGER;
	MyInteger = NewIntegerValue;
}

Silikego::Token::Token(float NewFloatValue)
{
	MyType = FLOAT;
	MyFloat = NewFloatValue;
}

Silikego::Token::Token(const char *NewIdValue)
{
	MyType = ID;
	MyId = new char[std::strlen(NewIdValue) + 1];
	std::strcpy(MyId, NewIdValue);
}

Silikego::Token::~Token()
{
	if (MyType == ID)
		delete[] MyId;
}

Silikego::Token::TokenType Silikego::Token::Type(void) const
{
	return MyType;
}

int Silikego::Token::Integer(void) const
{
	return MyInteger;
}

float Silikego::Token::Float(void) const
{
	return MyFloat;
}

const char *Silikego::Token::Id(void) const
{
	return MyId;
}
