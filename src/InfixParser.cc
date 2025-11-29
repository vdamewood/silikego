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

#include <SilikegoCore/Lexer.h>
#include <SilikegoCore/InfixParser.h>
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
		Lexer MyLexer(std::move(NewSource));
		SyntaxTreeNode result = GetExprssion(MyLexer);
		if (MyLexer.GetToken().Type() != Token::EOL)
			return ValueStatus::SYNTAX_ERR;
		return result;
	}

	static SyntaxTreeNode GetExprssion(Lexer& MyLexer)
	{
		return GetExprssionRest(MyLexer, GetTerm(MyLexer));
	}

	static SyntaxTreeNode GetExprssionRest(Lexer& lexer, SyntaxTreeNode&& left_side)
	{
		const char *function_id;
		switch (lexer.GetToken().Type())
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

		lexer.Next();

		SyntaxTreeNode branch{function_id};
		branch.PushRight(std::move(left_side));
		branch.PushRight(GetTerm(lexer));
		return GetExprssionRest(lexer, std::move(branch));
	}

	static SyntaxTreeNode GetTerm(Lexer& MyLexer)
	{
		return GetTermRest(MyLexer, GetExponent(MyLexer));
	}

	static SyntaxTreeNode GetTermRest(Lexer& MyLexer, SyntaxTreeNode&& left_side)
	{
		const char *function_id;
		switch (MyLexer.GetToken().Type())
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

		MyLexer.Next();

		SyntaxTreeNode branch{function_id};
		branch.PushRight(std::move(left_side));
		branch.PushRight(GetTerm(MyLexer));
		return GetTermRest(MyLexer, std::move(branch));
	}


	static SyntaxTreeNode GetExponent(Lexer& MyLexer)
	{
		SyntaxTreeNode leftValue = GetRoll(MyLexer);
		SyntaxTreeNode Rest = GetExponentRest(MyLexer);
		if (Rest.IsNothing())
			return leftValue;

        SyntaxTreeNode result{"power"};
		result.PushRight(std::move(leftValue));
		result.PushRight(std::move(Rest));
		return std::move(result);
	}

	static SyntaxTreeNode GetExponentRest(Lexer& MyLexer)
	{
		if (MyLexer.GetToken().Type() != '^')
			return SyntaxTreeNode();

		MyLexer.Next();

		switch (MyLexer.GetToken().Type())
		{
		case Token::INTEGER:
		case Token::FLOAT:
		case '-':
		case Token::ID:
		case '(':
			return GetExponent(MyLexer);
		default:
			return ValueStatus::SYNTAX_ERR;
		}
	}

	static SyntaxTreeNode GetRoll(Lexer& lexer)
	{
		SyntaxTreeNode left_side = GetAtom(lexer);
		SyntaxTreeNode rest = GetRollRest(lexer);

		if (rest.IsNothing())
			return left_side;

        SyntaxTreeNode result{"dice"};
		result.PushRight(std::move(left_side));
		result.PushRight(std::move(rest));
        return std::move(result);
	}

	static SyntaxTreeNode GetRollRest(Lexer& MyLexer)
	{
		if(MyLexer.GetToken().Type() != 'd')
			return SyntaxTreeNode();

		MyLexer.Next();

		if (MyLexer.GetToken().Type() == Token::INTEGER)
		{
			long long int result = MyLexer.GetToken().Integer();
			MyLexer.Next();
			return result;
		}
		else
		{
			return ValueStatus::SYNTAX_ERR;
		}
	}

	static SyntaxTreeNode GetAtom(Lexer& MyLexer)
	{
		SyntaxTreeNode value;

		switch(MyLexer.GetToken().Type())
		{
		case '-':
		case Token::INTEGER:
		case Token::FLOAT:
			return GetNumber(MyLexer);
		case '(':
			MyLexer.Next();
			value = GetExprssion(MyLexer);

			if (MyLexer.GetToken().Type() != ')')
			{
				return ValueStatus::SYNTAX_ERR;
			}

			MyLexer.Next();
			return value;
		case Token::ID:
			return GetFunctionCall(MyLexer);
		default:
			return ValueStatus::SYNTAX_ERR;
		}
	}

	static SyntaxTreeNode GetNumber(Lexer& MyLexer)
	{
		switch (MyLexer.GetToken().Type())
		{
		case Token::INTEGER:
		case Token::FLOAT:
			return GetUnsignedNumber(MyLexer);
		case '-':
		{
			MyLexer.Next();
			SyntaxTreeNode number = GetUnsignedNumber(MyLexer);
			number.Negate();
			return number;
		}
		default:
			return ValueStatus::SYNTAX_ERR;
		}
	}

	static SyntaxTreeNode GetUnsignedNumber(Lexer& MyLexer)
	{
		SyntaxTreeNode rVal;

		switch (MyLexer.GetToken().Type())
		{
		case Token::INTEGER:
		{
            long long int number = MyLexer.GetToken().Integer();
			MyLexer.Next();
			return number;
		}
		case Token::FLOAT:
		{
            double number = MyLexer.GetToken().Float();
			MyLexer.Next();
			return number;
		}
		default:
			return ValueStatus::SYNTAX_ERR;
		}
	}

	static SyntaxTreeNode GetFunctionCall(Lexer& MyLexer)
	{
		if (MyLexer.GetToken().Type() != Token::ID)
			return ValueStatus::SYNTAX_ERR;

        std::string FunctionName = MyLexer.GetToken().Id();
        MyLexer.Next();

		if (MyLexer.GetToken().Type() != '(')
            return ValueStatus::SYNTAX_ERR;

        MyLexer.Next();

        SyntaxTreeNode rVal(GetArguments(MyLexer, FunctionName));

		if (MyLexer.GetToken().Type() != ')')
            return ValueStatus::SYNTAX_ERR;

        MyLexer.Next();
        return rVal;
	}

	static SyntaxTreeNode GetArguments(Lexer& MyLexer, const std::string& function_id)
	{
		SyntaxTreeNode branch{function_id};
		while(true)
		{
			SyntaxTreeNode current = GetExprssion(MyLexer);
            bool was_error = current.IsError();
            branch.PushRight(std::move(current));

			if (was_error || MyLexer.GetToken().Type() == ')')
			{
				break;
			}
			else if (MyLexer.GetToken().Type() != ',')
			{
                branch.PushRight(ValueStatus::SYNTAX_ERR);
				break;
			}
			MyLexer.Next();
		}
        return std::move(branch);
	}
}
