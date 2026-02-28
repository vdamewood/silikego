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


#include <memory>
#include <string>
#include <utility>

#include <SilikegoCore/Input.h>
#include <SilikegoCore/Lexer.h>
#include <SilikegoCore/ParseInfix.h>
#include <SilikegoCore/Node.h>
#include <SilikegoCore/Token.h>
#include <SilikegoCore/Value.h>

namespace Silikego
{
	static Node GetExprssion(Lexer&);
	static Node GetExprssionRest(Lexer&, Node&&);
	static Node GetTerm(Lexer&);
	static Node GetTermRest(Lexer&, Node&&);
	static Node GetExponent(Lexer&);
	static Node GetExponentRest(Lexer&);
	static Node GetRoll(Lexer&);
	static Node GetRollRest(Lexer&);
	static Node GetAtom(Lexer&);
	static Node GetNumber(Lexer&);
	static Node GetUnsignedNumber(Lexer&);
	static Node GetFunctionCall(Lexer&);
	static Node GetArguments(Lexer&, const std::string&);


	Node ParseInfix(std::unique_ptr<Input> input_source)
	{
		if (!input_source)
			return Error::NullObject;

		Lexer lexer(std::move(input_source), true);
		if (lexer.isEmpty())
			return Error::NullObject;

		Node result = GetExprssion(lexer);
		if (lexer.token()->status() != TokenStatus::EndOfInput)
			return Error::Syntax;
		return result;
	}

	static Node GetExprssion(Lexer& lexer)
	{
		return GetExprssionRest(lexer, GetTerm(lexer));
	}

	static Node GetExprssionRest(Lexer& lexer, Node&& left_side)
	{
		if (lexer.token()->status() != TokenStatus::Character
				|| (lexer.token()->character() != '+'
					&& lexer.token()->character() != '-'))
			return left_side;

		const char *function_id;
		switch (lexer.token()->character())
		{
		case '+':
			function_id = "add";
			break;
		case '-':
			function_id = "subtract";
			break;
		default:
			return left_side;
		}

		lexer.advance();

		Node branch{function_id};
		branch.pushRight(std::move(left_side));
		branch.pushRight(GetTerm(lexer));
		return GetExprssionRest(lexer, std::move(branch));
	}

	static Node GetTerm(Lexer& lexer)
	{
		return GetTermRest(lexer, GetExponent(lexer));
	}

	static Node GetTermRest(Lexer& lexer, Node&& left_side)
	{
		if (lexer.token()->status() != TokenStatus::Character
				|| (lexer.token()->character() != '*'
					&& lexer.token()->character() != '/'))
			return left_side;

		const char *function_id;
		switch (lexer.token()->character())
		{
			case '*':
				function_id = "multiply";
				break;
			case '/':
				function_id = "divide";
				break;
			default:
				return std::move(left_side);
		}

		lexer.advance();

		Node branch{function_id};
		branch.pushRight(std::move(left_side));
		branch.pushRight(GetTerm(lexer));
		return GetTermRest(lexer, std::move(branch));
	}

	static Node GetExponent(Lexer& lexer)
	{
		Node leftValue = GetRoll(lexer);
		Node Rest = GetExponentRest(lexer);
		if (Rest.status() == NodeStatus::Nothing)
			return leftValue;

        Node result{"power"};
		result.pushRight(std::move(leftValue));
		result.pushRight(std::move(Rest));
		return result;
	}

	static Node GetExponentRest(Lexer& lexer)
	{
		if (lexer.token()->status() != TokenStatus::Character
				|| lexer.token()->character() != '^')
			return Node();

		lexer.advance();

		switch (lexer.token()->status())
		{
		case TokenStatus::Integer:
		case TokenStatus::Real:
		case TokenStatus::Id:
			return GetExponent(lexer);
		case TokenStatus::Character:
			switch(lexer.token()->character())
			{
			case '-':
			case '(':
				return GetExponent(lexer);
			default:
				;
			}
		default:
			;
		}
		return Error::Syntax;
	}

	static Node GetRoll(Lexer& lexer)
	{
		Node left_side = GetAtom(lexer);
		Node rest = GetRollRest(lexer);

		if (rest.status() == NodeStatus::Nothing)
			return left_side;

        Node result{"dice"};
		result.pushRight(std::move(left_side));
		result.pushRight(std::move(rest));
        return result;
	}

	static Node GetRollRest(Lexer& lexer)
	{
		if(lexer.token()->status() != TokenStatus::Character
				|| lexer.token()->character() != 'd')
			return Node();

		lexer.advance();

		if (lexer.token()->status() == TokenStatus::Integer)
		{
			long long int result = lexer.token()->integer();
			lexer.advance();
			return result;
		}
		else
		{
			return Error::Syntax;
		}
	}

	static Node GetAtom(Lexer& lexer)
	{
		switch(lexer.token()->status())
		{
		case TokenStatus::Integer:
		case TokenStatus::Real:
			return GetNumber(lexer);
		case TokenStatus::Id:
			return GetFunctionCall(lexer);
		case TokenStatus::Character:
			switch (lexer.token()->character())
			{
			case '-':
				return GetNumber(lexer);
			case '(':
			{
				lexer.advance();
				Node value = GetExprssion(lexer);

				if (lexer.token()->status() == TokenStatus::Character
					&& lexer.token()->character() == ')')
				{
					lexer.advance();
					return value;
				}
			}
				break;
			default:
				;
			}
		default:
			;
		}
		return Error::Syntax;
	}

	static Node GetNumber(Lexer& lexer)
	{
		switch (lexer.token()->status())
		{
		case TokenStatus::Integer:
		case TokenStatus::Real:
			return GetUnsignedNumber(lexer);
		case TokenStatus::Character:
		{
			if ( lexer.token()->character() == '-')
			{
				lexer.advance();
				Node number = GetUnsignedNumber(lexer);
				number.negate();
				return number;
			}
		}
		default:
			;
		}
		return Error::Syntax;
	}

	static Node GetUnsignedNumber(Lexer& lexer)
	{
		Node rVal;

		switch (lexer.token()->status())
		{
		case TokenStatus::Integer:
		{
            long long int number = lexer.token()->integer();
			lexer.advance();
			return number;
		}
		case TokenStatus::Real:
		{
            double number = lexer.token()->real();
			lexer.advance();
			return number;
		}
		default:
			return Error::Syntax;
		}
	}

	static Node GetFunctionCall(Lexer& lexer)
	{
		if (lexer.token()->status() != TokenStatus::Id)
			return Error::Syntax;

        std::string FunctionName = *lexer.token()->id();
        lexer.advance();

		if (lexer.token()->status() != TokenStatus::Character
				|| lexer.token()->character() != '(')
            return Error::Syntax;

        lexer.advance();

        Node function_call(GetArguments(lexer, FunctionName));

		if (lexer.token()->status() == TokenStatus::Character
				&& lexer.token()->character() == ')')
		{
	        lexer.advance();
    	    return function_call;
		}

		return Error::Syntax;
	}

	static Node GetArguments(
		Lexer& lexer,
		const std::string& function_id)
	{
		Node branch{function_id};
		while(true)
		{
			Node current = GetExprssion(lexer);
            bool was_error = current.status() == NodeStatus::Leaf
				&& current.value()->status() == ValueStatus::Error;
            branch.pushRight(std::move(current));

			if (was_error || (
					lexer.token()->status() == TokenStatus::Character
					&& lexer.token()->character() == ')'))
			{
				break;
			}
			else if (lexer.token()->status() != TokenStatus::Character
					|| lexer.token()->character() != ',')
			{
                branch.pushRight(Error::Syntax);
				break;
			}
			lexer.advance();
		}
        return branch;
	}
}
