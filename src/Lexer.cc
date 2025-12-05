/* Lexer.cc: Lexical analyzer
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

#include <cstdlib>
#include <cctype>
#include <cstring>
#include <memory>

#include <SilikegoCore/Lexer.h>

/* Values taken from Google Calculator 2011-07-06 */
#define EULER 2.71828183
#define PI    3.14159265

static int isOperator(int character)
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

static int isIdCharacter(int character)
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
		Impl(std::unique_ptr<DataSource> NewSource)
			: Source(std::move(NewSource))
		{
		}

		bool error = false;
		std::unique_ptr<DataSource> Source;
		Silikego::Token Token;
	};

	Lexer::Lexer(std::unique_ptr<DataSource> source) : impl(new Impl(std::move(source)))
	{
		advance();
	}

	Lexer::~Lexer()
	{
		delete impl;
	}

	void Lexer::advance()
	{
		if (impl->Token.status() == TokenStatus::EndOfInput || impl->error)
			return;

		SilikegoDfaState dfaState = DFA_START;
		std::string lexeme;

		while (dfaState != DFA_END)
		switch (dfaState)
		{
		case DFA_END:
			break;
		case DFA_START:
			if(isOperator(impl->Source->current()))
			{
				lexeme += impl->Source->current();
				impl->Source->advance();
				dfaState = DFA_TERM_CHAR;
			}
			else if (impl->Source->current() == 'd')
			{
				lexeme += impl->Source->current();
				impl->Source->advance();
				dfaState = DFA_DICE;
			}
			else if (impl->Source->current() == 'e')
			{
				lexeme += impl->Source->current();
				impl->Source->advance();
				dfaState = DFA_E;
			}
			else if (impl->Source->current() == 'p')
			{
				lexeme += impl->Source->current();
				impl->Source->advance();
				dfaState = DFA_PI_1;
			}
			else if (std::isdigit(impl->Source->current()))
			{
				lexeme += impl->Source->current();
				impl->Source->advance();
				dfaState = DFA_INTEGER;
			}
			else if (std::isalpha(impl->Source->current()))
			{
				lexeme += impl->Source->current();
				impl->Source->advance();
				dfaState = DFA_ID;
			}
			else if (std::isspace(impl->Source->current()))
			{
				impl->Source->advance();
			}
			else if (impl->Source->current() == '\0')
			{
				dfaState = DFA_TERM_EOI;
			}
			else
			{
				dfaState = DFA_ERROR;
			}
			break;
		case DFA_DICE:
			if (std::isalpha(impl->Source->current()))
			{
				lexeme += impl->Source->current();
				impl->Source->advance();
				dfaState = DFA_ID;
			}
			else
			{
				dfaState = DFA_TERM_CHAR;
			}
			break;
		case DFA_E:
			if (std::isalnum(impl->Source->current()))
			{
				lexeme += impl->Source->current();
				impl->Source->advance();
				dfaState = DFA_ID;
			}
			else
			{
				dfaState = DFA_TERM_E;
			}
			break;
		case DFA_PI_1:
			if (impl->Source->current() == 'i')
			{
				lexeme += impl->Source->current();
				impl->Source->advance();
				dfaState = DFA_PI_2;
			}
			else if (isIdCharacter(impl->Source->current()))
			{
				lexeme += impl->Source->current();
				impl->Source->advance();
				dfaState = DFA_ID;
			}
			else
			{
				dfaState = DFA_TERM_STRING;
			}
			break;
		case DFA_PI_2:
			if (isIdCharacter(impl->Source->current()))
			{
				lexeme += impl->Source->current();
				impl->Source->advance();
				dfaState = DFA_ID;
			}
			else
			{
				dfaState = DFA_TERM_PI;
			}
			break;
		case DFA_ID:
			if (isalnum(impl->Source->current()))
			{
				lexeme += impl->Source->current();
				impl->Source->advance();
			}
			else
			{
				dfaState = DFA_TERM_STRING;
			}
			break;
		case DFA_INTEGER:
			if (impl->Source->current() == '.')
			{
				lexeme += impl->Source->current();
				impl->Source->advance();
				dfaState = DFA_FLOAT;
			}
			else if (std::isdigit(impl->Source->current()))
			{
				lexeme += impl->Source->current();
				impl->Source->advance();
			}
			else
			{
				dfaState = DFA_TERM_INTEGER;
			}
			break;
		case DFA_FLOAT:
			if (std::isdigit(impl->Source->current()))
			{
				lexeme += impl->Source->current();
				impl->Source->advance();
			}
			else
			{
				dfaState = DFA_TERM_FLOAT;
			}
			break;
		case DFA_TERM_INTEGER:
			impl->Token = std::strtoll(lexeme.c_str(), 0, 10);
			dfaState = DFA_END;
			break;
		case DFA_TERM_FLOAT:
			impl->Token = std::atof(lexeme.c_str());
			dfaState = DFA_END;
			break;
		case DFA_TERM_E:
			impl->Token = EULER;
			dfaState = DFA_END;
			break;
		case DFA_TERM_PI:
			impl->Token = PI;
			dfaState = DFA_END;
			break;
		case DFA_TERM_CHAR:
			impl->Token = lexeme[0];
			dfaState = DFA_END;
			break;
		case DFA_TERM_STRING:
			impl->Token = lexeme;
			dfaState = DFA_END;
			break;
		case DFA_TERM_EOI:
			impl->Token = EndOfInput();
			dfaState = DFA_END;
			break;
		case DFA_ERROR:
			impl->error = true;
			dfaState = DFA_END;
			break;
		}
	}

	Token& Lexer::token()
	{
		return impl->Token;
	}
}
