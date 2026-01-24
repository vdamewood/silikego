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


#include <cstdlib>
#include <cctype>
#include <memory>
#include <string>
#include <utility>

#include <SilikegoCore/Input.h>
#include <SilikegoCore/Lexer.h>
#include <SilikegoCore/Token.h>

/* Values taken from Google Calculator 2011-07-06 */
#define EULER 2.71828183
#define PI    3.14159265

static inline bool
IsOperator(int character)
{
	return
	(
		character == '+'
		|| character == '-'
		|| character == '/'
		|| character == '*'
		|| character == '^'
		|| character == ','
		|| character == '('
		|| character == ')'
	);
}

static inline bool
IsIdCharacter(int character)
{
	return (std::isalnum(character) || character == '_');
}

enum SilikegoDfaState
{
	DFA_ERROR = -1,
	DFA_START = 0,	/* "" */
	DFA_E,		/* "e" */
	DFA_PI_1,	/* "p" */
	DFA_PI_2,	/* "pi" */
	DFA_DICE,	/* "d" */
	DFA_ID,		/* ([a-ce-z][a-z0-9]*) | (d[a-z][a-z0-9]*) */
	DFA_INTEGER,	/* [0-9]+ */
	DFA_FLOAT,	/* [0-9]+\.[0-9]+ */
	DFA_TERM_INTEGER,
	DFA_TERM_FLOAT,
	DFA_TERM_E,
	DFA_TERM_PI,
	DFA_TERM_CHAR,
	DFA_TERM_STRING,
	DFA_TERM_EOI,
	DFA_END
};
namespace Silikego
{
	class Lexer::Impl
	{
	public:
		Impl(std::unique_ptr<Input> NewSource)
			: Source(std::move(NewSource))
		{
		}

		bool error = false;
		std::unique_ptr<Input> Source;
		Silikego::Token Token;
	};

	Lexer::Lexer(std::unique_ptr<Input> source) : _impl(new Impl(std::move(source)))
	{
		advance();
	}

	Lexer::~Lexer()
	{
		delete _impl;
	}

	void Lexer::advance()
	{
		if (_impl->Token.status() == TokenStatus::EndOfInput || _impl->error)
			return;

		SilikegoDfaState dfaState = DFA_START;
		std::string lexeme;

		while (dfaState != DFA_END)
		switch (dfaState)
		{
		case DFA_END:
			break;
		case DFA_START:
			if(IsOperator(_impl->Source->character()))
			{
				lexeme += _impl->Source->character();
				_impl->Source->advance();
				dfaState = DFA_TERM_CHAR;
			}
			else if (_impl->Source->character() == 'd')
			{
				lexeme += _impl->Source->character();
				_impl->Source->advance();
				dfaState = DFA_DICE;
			}
			else if (_impl->Source->character() == 'e')
			{
				lexeme += _impl->Source->character();
				_impl->Source->advance();
				dfaState = DFA_E;
			}
			else if (_impl->Source->character() == 'p')
			{
				lexeme += _impl->Source->character();
				_impl->Source->advance();
				dfaState = DFA_PI_1;
			}
			else if (std::isdigit(_impl->Source->character()))
			{
				lexeme += _impl->Source->character();
				_impl->Source->advance();
				dfaState = DFA_INTEGER;
			}
			else if (std::isalpha(_impl->Source->character()))
			{
				lexeme += _impl->Source->character();
				_impl->Source->advance();
				dfaState = DFA_ID;
			}
			else if (std::isspace(_impl->Source->character()))
			{
				_impl->Source->advance();
			}
			else if (_impl->Source->character() == '\0')
			{
				dfaState = DFA_TERM_EOI;
			}
			else
			{
				dfaState = DFA_ERROR;
			}
			break;
		case DFA_DICE:
			if (std::isalpha(_impl->Source->character()))
			{
				lexeme += _impl->Source->character();
				_impl->Source->advance();
				dfaState = DFA_ID;
			}
			else
			{
				dfaState = DFA_TERM_CHAR;
			}
			break;
		case DFA_E:
			if (std::isalnum(_impl->Source->character()))
			{
				lexeme += _impl->Source->character();
				_impl->Source->advance();
				dfaState = DFA_ID;
			}
			else
			{
				dfaState = DFA_TERM_E;
			}
			break;
		case DFA_PI_1:
			if (_impl->Source->character() == 'i')
			{
				lexeme += _impl->Source->character();
				_impl->Source->advance();
				dfaState = DFA_PI_2;
			}
			else if (IsIdCharacter(_impl->Source->character()))
			{
				lexeme += _impl->Source->character();
				_impl->Source->advance();
				dfaState = DFA_ID;
			}
			else
			{
				dfaState = DFA_TERM_STRING;
			}
			break;
		case DFA_PI_2:
			if (IsIdCharacter(_impl->Source->character()))
			{
				lexeme += _impl->Source->character();
				_impl->Source->advance();
				dfaState = DFA_ID;
			}
			else
			{
				dfaState = DFA_TERM_PI;
			}
			break;
		case DFA_ID:
			if (isalnum(_impl->Source->character()))
			{
				lexeme += _impl->Source->character();
				_impl->Source->advance();
			}
			else
			{
				dfaState = DFA_TERM_STRING;
			}
			break;
		case DFA_INTEGER:
			if (_impl->Source->character() == '.')
			{
				lexeme += _impl->Source->character();
				_impl->Source->advance();
				dfaState = DFA_FLOAT;
			}
			else if (std::isdigit(_impl->Source->character()))
			{
				lexeme += _impl->Source->character();
				_impl->Source->advance();
			}
			else
			{
				dfaState = DFA_TERM_INTEGER;
			}
			break;
		case DFA_FLOAT:
			if (std::isdigit(_impl->Source->character()))
			{
				lexeme += _impl->Source->character();
				_impl->Source->advance();
			}
			else
			{
				dfaState = DFA_TERM_FLOAT;
			}
			break;
		case DFA_TERM_INTEGER:
			_impl->Token = std::strtoll(lexeme.c_str(), 0, 10);
			dfaState = DFA_END;
			break;
		case DFA_TERM_FLOAT:
			_impl->Token = std::atof(lexeme.c_str());
			dfaState = DFA_END;
			break;
		case DFA_TERM_E:
			_impl->Token = EULER;
			dfaState = DFA_END;
			break;
		case DFA_TERM_PI:
			_impl->Token = PI;
			dfaState = DFA_END;
			break;
		case DFA_TERM_CHAR:
			_impl->Token = lexeme[0];
			dfaState = DFA_END;
			break;
		case DFA_TERM_STRING:
			_impl->Token = lexeme;
			dfaState = DFA_END;
			break;
		case DFA_TERM_EOI:
			_impl->Token = EndOfInput();
			dfaState = DFA_END;
			break;
		case DFA_ERROR:
			_impl->error = true;
			dfaState = DFA_END;
			break;
		}
	}

	Token* Lexer::token()
	{
		if (isEmpty())
			return nullptr;

		return &_impl->Token;
	}
}
