/* InfixParser.cc: Recursive-descent infix parser
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

#include <typeinfo>

#include "Lexer.h"
#include "InfixParser.h"

namespace Silikego
{
	static SyntaxTreeNode *GetExpr0(Lexer&);
	static SyntaxTreeNode *GetExpr0r(Lexer&);
	static SyntaxTreeNode *GetExpr1(Lexer&);
	static SyntaxTreeNode *GetExpr1r(Lexer&);
	static SyntaxTreeNode *GetExpr2(Lexer&);
	static SyntaxTreeNode *GetExpr2lf(Lexer&);
	static SyntaxTreeNode *GetExpr3(Lexer&);
	static SyntaxTreeNode *GetExpr3lf(Lexer&);
	static SyntaxTreeNode *GetAtom(Lexer&);
	static SyntaxTreeNode *GetNumber(Lexer&);
	static SyntaxTreeNode *GetUNumber(Lexer&);
	static SyntaxTreeNode *GetFCall(Lexer&);
	static void                  GetArguments(Lexer&, BranchNode&);


	SyntaxTreeNode* ParseInfix(DataSource* NewSource)
	{
		Lexer MyLexer(NewSource);
		SyntaxTreeNode* rVal = 0;

		if (MyLexer.GetToken().Type() == Token::EOL)
		{
			rVal = new IntegerNode(0);
		}
		else
		{
			rVal = GetExpr0(MyLexer);
			if (MyLexer.GetToken().Type() != Token::EOL
				&& typeid(rVal) != typeid(SyntaxErrorNode))
			{
				delete rVal;
				rVal = new SyntaxErrorNode();
			}
		}

		return rVal;
	}

	static SyntaxTreeNode *GetExpr0(Lexer& MyLexer)
	{
		SyntaxTreeNode *Left = GetExpr1(MyLexer);
		SyntaxTreeNode *Rest = GetExpr0r(MyLexer);

		if (typeid(*Rest) == typeid(NothingNode))
		{
			delete Rest;
			return Left;
		}

		if (reinterpret_cast<BranchNode*>(Rest)->GraftLeft(Left))
			return Rest;

		delete Rest;
		delete Left;
		return new SyntaxErrorNode();
	}

	static SyntaxTreeNode *GetExpr0r(Lexer& MyLexer)
	{
		const char *FunctionId;
		switch (MyLexer.GetToken().Type())
		{
		case '+':
			FunctionId = "add";
			break;
		case '-':
			FunctionId = "subtract";
			break;
		default:
			return new NothingNode();
		}

		MyLexer.Next();

		BranchNode *Branch = new BranchNode(FunctionId);
		Branch->PushRight(0);
		Branch->PushRight(GetExpr1(MyLexer));
		SyntaxTreeNode *Rest = GetExpr0r(MyLexer);

		if (typeid(*Rest) == typeid(NothingNode))
		{
			delete Rest;
			return Branch;
		}
		else
		{
			reinterpret_cast<BranchNode*>(Rest)->GraftLeft(Branch);
			return Rest;
		}
	}

	static SyntaxTreeNode *GetExpr1(Lexer& MyLexer)
	{
		SyntaxTreeNode *Left = GetExpr2(MyLexer);
		SyntaxTreeNode *Rest = GetExpr1r(MyLexer);

		if (typeid(*Rest) == typeid(NothingNode))
		{
			delete Rest;
			return Left;
		}

		if (reinterpret_cast<BranchNode*>(Rest)->GraftLeft(Left))
			return Rest;

		delete Rest;
		delete Left;
		return new SyntaxErrorNode();
	}

	static SyntaxTreeNode *GetExpr1r(Lexer& MyLexer)
	{
		const char *FunctionId;
		switch (MyLexer.GetToken().Type())
		{
			case '*':
				FunctionId = "multiply";
				break;
			case '/':
				FunctionId = "divide";
				break;
			default:
				return new NothingNode();
		}

		MyLexer.Next();

		BranchNode *Branch = new BranchNode(FunctionId);
		Branch->PushRight(0);
		Branch->PushRight(GetExpr2(MyLexer));
		SyntaxTreeNode *Rest = GetExpr1r(MyLexer);

		if (typeid(*Rest) == typeid(NothingNode))
		{
			delete Rest;
			return Branch;
		}
		else
		{
			reinterpret_cast<BranchNode*>(Rest)->GraftLeft(Branch);
			return Rest;
		}
	}


	static SyntaxTreeNode *GetExpr2(Lexer& MyLexer)
	{
		SyntaxTreeNode *leftValue = GetExpr3(MyLexer);
		SyntaxTreeNode *Rest = GetExpr2lf(MyLexer);

		if (typeid(*Rest) == typeid(NothingNode))
		{
			delete Rest;
			return leftValue;
		}

		BranchNode *rVal = new BranchNode("power");
		rVal->PushRight(leftValue);
		rVal->PushRight(Rest);
		return rVal;
	}

	static SyntaxTreeNode *GetExpr2lf(Lexer& MyLexer)
	{
		if (MyLexer.GetToken().Type() != '^')
			return new NothingNode();

		MyLexer.Next();

		switch (MyLexer.GetToken().Type())
		{
		case Token::INTEGER:
		case Token::FLOAT:
		case '-':
		case Token::ID:
		case '(':
			return GetExpr2(MyLexer);
		default:
			return new SyntaxErrorNode();
		}
	}

	static SyntaxTreeNode *GetExpr3(Lexer& MyLexer)
	{
		SyntaxTreeNode *leftValue = GetAtom(MyLexer);
		SyntaxTreeNode *Rest = GetExpr3lf(MyLexer);

		if (typeid(*Rest) == typeid(NothingNode))
		{
			delete Rest;
			return leftValue;
		}

		BranchNode *rVal = new BranchNode("dice");
		rVal->PushRight(leftValue);
		rVal->PushRight(Rest);
		return rVal;
	}

	static SyntaxTreeNode *GetExpr3lf(Lexer& MyLexer)
	{
		if(MyLexer.GetToken().Type() != 'd')
			return new NothingNode();

		MyLexer.Next();

		if (MyLexer.GetToken().Type() == Token::INTEGER)
		{
			SyntaxTreeNode *rVal = new IntegerNode(MyLexer.GetToken().Integer());
			MyLexer.Next();
			return rVal;
		}
		else
		{
			return new SyntaxErrorNode();
		}
	}

	static SyntaxTreeNode *GetAtom(Lexer& MyLexer)
	{
		SyntaxTreeNode *value;

		switch(MyLexer.GetToken().Type())
		{
		case '-':
		case Token::INTEGER:
		case Token::FLOAT:
			return GetNumber(MyLexer);
		case '(':
			MyLexer.Next();
			value = GetExpr0(MyLexer);

			if (MyLexer.GetToken().Type() != ')')
			{
				delete value;
				return new SyntaxErrorNode();
			}

			MyLexer.Next();
			return value;
		case Token::ID:
			return GetFCall(MyLexer);
		default:
			return new SyntaxErrorNode();
		}
	}

	static SyntaxTreeNode *GetNumber(Lexer& MyLexer)
	{
		SyntaxTreeNode *rVal;

		switch (MyLexer.GetToken().Type())
		{
		case Token::INTEGER:
		case Token::FLOAT:
			return GetUNumber(MyLexer);
		case '-':
			MyLexer.Next();
			rVal = GetUNumber(MyLexer);
			rVal->Negate();
			return rVal;
		default:
			return new SyntaxErrorNode();
		}
	}

	static SyntaxTreeNode *GetUNumber(Lexer& MyLexer)
	{
		SyntaxTreeNode *rVal;

		switch (MyLexer.GetToken().Type())
		{
		case Token::INTEGER:
			rVal = new IntegerNode(MyLexer.GetToken().Integer());
			MyLexer.Next();
			return rVal;
		case Token::FLOAT:
			rVal = new FloatNode(MyLexer.GetToken().Float());
			MyLexer.Next();
			return rVal;
		default:
			return new SyntaxErrorNode();
		}
	}

	static SyntaxTreeNode *GetFCall(Lexer& MyLexer)
	{
		if (MyLexer.GetToken().Type() != Token::ID)
			return new SyntaxErrorNode();

		BranchNode *rVal = new BranchNode(MyLexer.GetToken().Id());
		MyLexer.Next();

		if (MyLexer.GetToken().Type() != '(')
		{
			rVal->PushRight(new SyntaxErrorNode());
			return rVal;
		}
		MyLexer.Next();

		GetArguments(MyLexer, *rVal);

		if (MyLexer.GetToken().Type() != ')')
		{
			rVal->PushRight(new SyntaxErrorNode());
			return rVal;
		}
		MyLexer.Next();

		return rVal;
	}

	static void GetArguments(Lexer& MyLexer, BranchNode& rVal)
	{
		while(true)
		{
			SyntaxTreeNode *Expression = GetExpr0(MyLexer);
			rVal.PushRight(Expression);

			if (typeid(*Expression) == typeid(SyntaxErrorNode)
				|| MyLexer.GetToken().Type() == ')')
			{
				break;
			}
			else if (MyLexer.GetToken().Type() != ',')
			{
				rVal.PushRight(new SyntaxErrorNode());
				break;
			}
			MyLexer.Next();
		}
	}
}