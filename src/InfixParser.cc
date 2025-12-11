/* InfixParser.cc: Recursive-descent infix parser
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

#include <memory>
#include <string>
#include <utility>

#include <SilikegoCore/DataSource.h>
#include <SilikegoCore/Lexer.h>
#include <SilikegoCore/InfixParser.h>
#include <SilikegoCore/SyntaxTree.h>
#include <SilikegoCore/Token.h>
#include <SilikegoCore/Value.h>

namespace Silikego
{
	static SyntaxTreeNode GetExprssion(Lexer&);
	static SyntaxTreeNode GetExprssionRest(Lexer&, SyntaxTreeNode&&);
	static SyntaxTreeNode GetTerm(Lexer&);
	static SyntaxTreeNode GetTermRest(Lexer&, SyntaxTreeNode&&);
	static SyntaxTreeNode GetExponent(Lexer&);
	static SyntaxTreeNode GetExponentRest(Lexer&);
	static SyntaxTreeNode GetRoll(Lexer&);
	static SyntaxTreeNode GetRollRest(Lexer&);
	static SyntaxTreeNode GetAtom(Lexer&);
	static SyntaxTreeNode GetNumber(Lexer&);
	static SyntaxTreeNode GetUnsignedNumber(Lexer&);
	static SyntaxTreeNode GetFunctionCall(Lexer&);
	static SyntaxTreeNode GetArguments(Lexer&, const std::string&);


	SyntaxTreeNode ParseInfix(std::unique_ptr<DataSource> NewSource)
	{
		Lexer lexer(std::move(NewSource));
		SyntaxTreeNode result = GetExprssion(lexer);
		if (lexer.token().status() != TokenStatus::EndOfInput)
			return Error::Syntax;
		return result;
	}

	static SyntaxTreeNode GetExprssion(Lexer& lexer)
	{
		return GetExprssionRest(lexer, GetTerm(lexer));
	}

	static SyntaxTreeNode GetExprssionRest(Lexer& lexer, SyntaxTreeNode&& left_side)
	{
		if (lexer.token().status() != TokenStatus::Operator
				|| (lexer.token().operatorValue() != '+'
					&& lexer.token().operatorValue() != '-'))
			return left_side;

		const char *function_id;
		switch (lexer.token().operatorValue())
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

		SyntaxTreeNode branch{function_id};
		branch.pushRight(std::move(left_side));
		branch.pushRight(GetTerm(lexer));
		return GetExprssionRest(lexer, std::move(branch));
	}

	static SyntaxTreeNode GetTerm(Lexer& lexer)
	{
		return GetTermRest(lexer, GetExponent(lexer));
	}

	static SyntaxTreeNode GetTermRest(Lexer& lexer, SyntaxTreeNode&& left_side)
	{
		if (lexer.token().status() != TokenStatus::Operator
				|| (lexer.token().operatorValue() != '*'
					&& lexer.token().operatorValue() != '/'))
			return left_side;

		const char *function_id;
		switch (lexer.token().operatorValue())
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

		SyntaxTreeNode branch{function_id};
		branch.pushRight(std::move(left_side));
		branch.pushRight(GetTerm(lexer));
		return GetTermRest(lexer, std::move(branch));
	}

	static SyntaxTreeNode GetExponent(Lexer& lexer)
	{
		SyntaxTreeNode leftValue = GetRoll(lexer);
		SyntaxTreeNode Rest = GetExponentRest(lexer);
		if (Rest.status() == NodeStatus::Nothing)
			return leftValue;

        SyntaxTreeNode result{"power"};
		result.pushRight(std::move(leftValue));
		result.pushRight(std::move(Rest));
		return std::move(result);
	}

	static SyntaxTreeNode GetExponentRest(Lexer& lexer)
	{
		if (lexer.token().status() != TokenStatus::Operator
				|| lexer.token().operatorValue() != '^')
			return SyntaxTreeNode();

		lexer.advance();

		switch (lexer.token().status())
		{
		case TokenStatus::Integer:
		case TokenStatus::Float:
		case TokenStatus::Id:
			return GetExponent(lexer);
		case TokenStatus::Operator:
			switch(lexer.token().operatorValue())
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

	static SyntaxTreeNode GetRoll(Lexer& lexer)
	{
		SyntaxTreeNode left_side = GetAtom(lexer);
		SyntaxTreeNode rest = GetRollRest(lexer);

		if (rest.status() == NodeStatus::Nothing)
			return left_side;

        SyntaxTreeNode result{"dice"};
		result.pushRight(std::move(left_side));
		result.pushRight(std::move(rest));
        return std::move(result);
	}

	static SyntaxTreeNode GetRollRest(Lexer& lexer)
	{
		if(lexer.token().status() != TokenStatus::Operator
				|| lexer.token().operatorValue() != 'd')
			return SyntaxTreeNode();

		lexer.advance();

		if (lexer.token().status() == TokenStatus::Integer)
		{
			long long int result = lexer.token().integerValue();
			lexer.advance();
			return result;
		}
		else
		{
			return Error::Syntax;
		}
	}

	static SyntaxTreeNode GetAtom(Lexer& lexer)
	{
		switch(lexer.token().status())
		{
		case TokenStatus::Integer:
		case TokenStatus::Float:
			return GetNumber(lexer);
		case TokenStatus::Id:
			return GetFunctionCall(lexer);
		case TokenStatus::Operator:
			switch (lexer.token().operatorValue())
			{
			case '-':
				return GetNumber(lexer);
			case '(':
			{
				lexer.advance();
				SyntaxTreeNode value = GetExprssion(lexer);

				if (lexer.token().status() == TokenStatus::Operator
					&& lexer.token().operatorValue() == ')')
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

	static SyntaxTreeNode GetNumber(Lexer& lexer)
	{
		switch (lexer.token().status())
		{
		case TokenStatus::Integer:
		case TokenStatus::Float:
			return GetUnsignedNumber(lexer);
		case TokenStatus::Operator:
		{
			if ( lexer.token().operatorValue() == '-')
			{
				lexer.advance();
				SyntaxTreeNode number = GetUnsignedNumber(lexer);
				number.negate();
				return number;
			}
		}
		default:
			;
		}
		return Error::Syntax;
	}

	static SyntaxTreeNode GetUnsignedNumber(Lexer& lexer)
	{
		SyntaxTreeNode rVal;

		switch (lexer.token().status())
		{
		case TokenStatus::Integer:
		{
            long long int number = lexer.token().integerValue();
			lexer.advance();
			return number;
		}
		case TokenStatus::Float:
		{
            double number = lexer.token().floatValue();
			lexer.advance();
			return number;
		}
		default:
			return Error::Syntax;
		}
	}

	static SyntaxTreeNode GetFunctionCall(Lexer& lexer)
	{
		if (lexer.token().status() != TokenStatus::Id)
			return Error::Syntax;

        std::string FunctionName = lexer.token().idValue();
        lexer.advance();

		if (lexer.token().status() != TokenStatus::Operator
				|| lexer.token().operatorValue() != '(')
            return Error::Syntax;

        lexer.advance();

        SyntaxTreeNode function_call(GetArguments(lexer, FunctionName));

		if (lexer.token().status() == TokenStatus::Operator
				&& lexer.token().operatorValue() == ')')
		{
	        lexer.advance();
    	    return function_call;
		}

		return Error::Syntax;
	}

	static SyntaxTreeNode GetArguments(Lexer& lexer, const std::string& function_id)
	{
		SyntaxTreeNode branch{function_id};
		while(true)
		{
			SyntaxTreeNode current = GetExprssion(lexer);
            bool was_error = current.isError();
            branch.pushRight(std::move(current));

			if (was_error || (
					lexer.token().status() == TokenStatus::Operator
					&& lexer.token().operatorValue() == ')'))
			{
				break;
			}
			else if (lexer.token().status() != TokenStatus::Operator
					|| lexer.token().operatorValue() != ',')
			{
                branch.pushRight(Error::Syntax);
				break;
			}
			lexer.advance();
		}
        return std::move(branch);
	}
}
