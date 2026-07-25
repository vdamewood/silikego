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

namespace {
	// Taken from
	// The Art of Computer Programming, Volume 2, Third Edition
	// By Donald E. Knuth
	const double Pi =    0x3.243f6a8885a30p0;
	const double Euler = 0x2.b7e151628aed2p0;

	inline bool
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

	inline bool
	IsIdCharacter(int character)
	{
		return (std::isalnum(character) || character == '_');
	}

	enum class DfaState
	{
		Error,
		Start,	    /* "" */
		Euler,		/* "e" */
		PiStart,	/* "p" */
		PiFull,	    /* "pi" */
		Dice,	    /* "d" */
		Id,		    /* ([a-ce-z][a-z0-9]*) | (d[a-z][a-z0-9]*) */
		Integer,	/* [0-9]+ */
		Real,	    /* [0-9]+\.[0-9]+ */
		TerminateInteger,
		TerminateReal,
		TerminateEuler,
		TerminatePi,
		TerminateCharacter,
		TerminateId,
		TerminateEndOfInput,
		Finish
	};
};

namespace Silikego
{
	class Lexer::Impl
	{
	public:
		Impl(
			std::unique_ptr<Input> new_source,
			bool support_dice,
			bool support_constants
		) :
			source(std::move(new_source)),
			supportDice(support_dice),
			supportConstants(support_constants)
		{
		}

		bool error = false;
		std::unique_ptr<Input> source;
		Silikego::Token token;
		bool supportDice;
		bool supportConstants;
	};

	Lexer::Lexer(
		std::unique_ptr<Input> source,
		bool support_dice,
		bool support_constants
	) :
		_impl(new(std::nothrow) Impl(
			std::move(source),
			support_dice,
			support_constants))
	{
		advance();
	}

	Lexer::~Lexer()
	{
		delete _impl;
	}

	void Lexer::advance()
	{
		if (isEmpty())
			return;

		if (_impl->token.status() == TokenStatus::EndOfInput
				|| _impl->error)
			return;

		DfaState dfa_state = DfaState::Start;
		std::string lexeme;

		while (dfa_state != DfaState::Finish)
		switch (dfa_state)
		{
		case DfaState::Finish:
			break;
		case DfaState::Start:
			if(IsOperator(_impl->source->character()))
			{
				lexeme += _impl->source->character();
				_impl->source->advance();
				dfa_state = DfaState::TerminateCharacter;
			}
			else if (_impl->supportDice
				&& _impl->source->character() == 'd')
			{
				lexeme += _impl->source->character();
				_impl->source->advance();
				dfa_state = DfaState::Dice;
			}
			else if (_impl->source->character() == 'e')
			{
				lexeme += _impl->source->character();
				_impl->source->advance();
				dfa_state = DfaState::Euler;
			}
			else if (_impl->source->character() == 'p')
			{
				lexeme += _impl->source->character();
				_impl->source->advance();
				dfa_state = DfaState::PiStart;
			}
			else if (std::isdigit(_impl->source->character()))
			{
				lexeme += _impl->source->character();
				_impl->source->advance();
				dfa_state = DfaState::Integer;
			}
			else if (std::isalpha(_impl->source->character()))
			{
				lexeme += _impl->source->character();
				_impl->source->advance();
				dfa_state = DfaState::Id;
			}
			else if (std::isspace(_impl->source->character()))
			{
				_impl->source->advance();
			}
			else if (_impl->source->character() == '\0')
			{
				dfa_state = DfaState::TerminateEndOfInput;
			}
			else
			{
				dfa_state = DfaState::Error;
			}
			break;
		case DfaState::Dice:
			if (std::isalpha(_impl->source->character()))
			{
				lexeme += _impl->source->character();
				_impl->source->advance();
				dfa_state = DfaState::Id;
			}
			else
			{
				dfa_state = DfaState::TerminateCharacter;
			}
			break;
		case DfaState::Euler:
			if (std::isalnum(_impl->source->character()))
			{
				lexeme += _impl->source->character();
				_impl->source->advance();
				dfa_state = DfaState::Id;
			}
			else
			{
				dfa_state = DfaState::TerminateEuler;
			}
			break;
		case DfaState::PiStart:
			if (_impl->source->character() == 'i')
			{
				lexeme += _impl->source->character();
				_impl->source->advance();
				dfa_state = DfaState::PiFull;
			}
			else if (IsIdCharacter(_impl->source->character()))
			{
				lexeme += _impl->source->character();
				_impl->source->advance();
				dfa_state = DfaState::Id;
			}
			else
			{
				dfa_state = DfaState::TerminateId;
			}
			break;
		case DfaState::PiFull:
			if (IsIdCharacter(_impl->source->character()))
			{
				lexeme += _impl->source->character();
				_impl->source->advance();
				dfa_state = DfaState::Id;
			}
			else
			{
				dfa_state = DfaState::TerminatePi;
			}
			break;
		case DfaState::Id:
			if (std::isalnum(_impl->source->character()))
			{
				lexeme += _impl->source->character();
				_impl->source->advance();
			}
			else
			{
				dfa_state = DfaState::TerminateId;
			}
			break;
		case DfaState::Integer:
			if (_impl->source->character() == '.')
			{
				lexeme += _impl->source->character();
				_impl->source->advance();
				dfa_state = DfaState::Real;
			}
			else if (std::isdigit(_impl->source->character()))
			{
				lexeme += _impl->source->character();
				_impl->source->advance();
			}
			else
			{
				dfa_state = DfaState::TerminateInteger;
			}
			break;
		case DfaState::Real:
			if (std::isdigit(_impl->source->character()))
			{
				lexeme += _impl->source->character();
				_impl->source->advance();
			}
			else
			{
				dfa_state = DfaState::TerminateReal;
			}
			break;
		case DfaState::TerminateInteger:
			_impl->token = std::strtoll(lexeme.c_str(), 0, 10);
			dfa_state = DfaState::Finish;
			break;
		case DfaState::TerminateReal:
			_impl->token = std::strtod(lexeme.c_str(), 0);
			dfa_state = DfaState::Finish;
			break;
		case DfaState::TerminateEuler:
			_impl->token = Euler;
			dfa_state = DfaState::Finish;
			break;
		case DfaState::TerminatePi:
			_impl->token = Pi;
			dfa_state = DfaState::Finish;
			break;
		case DfaState::TerminateCharacter:
			_impl->token = lexeme[0];
			dfa_state = DfaState::Finish;
			break;
		case DfaState::TerminateId:
			_impl->token = lexeme;
			dfa_state = DfaState::Finish;
			break;
		case DfaState::TerminateEndOfInput:
			_impl->token = EndOfInput();
			dfa_state = DfaState::Finish;
			break;
		case DfaState::Error:
			_impl->error = true;
			dfa_state = DfaState::Finish;
			break;
		}
	}

	Token* Lexer::token()
	{
		if (isEmpty())
			return nullptr;

		return &_impl->token;
	}
}
