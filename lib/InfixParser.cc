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

static Silikego::SyntaxTreeNode *GetExpr0(Silikego::Lexer&);
static Silikego::SyntaxTreeNode *GetExpr0r(Silikego::Lexer&);
static Silikego::SyntaxTreeNode *GetExpr1(Silikego::Lexer&);
static Silikego::SyntaxTreeNode *GetExpr1r(Silikego::Lexer&);
static Silikego::SyntaxTreeNode *GetExpr2(Silikego::Lexer&);
static Silikego::SyntaxTreeNode *GetExpr2lf(Silikego::Lexer&);
static Silikego::SyntaxTreeNode *GetExpr3(Silikego::Lexer&);
static Silikego::SyntaxTreeNode *GetExpr3lf(Silikego::Lexer&);
static Silikego::SyntaxTreeNode *GetAtom(Silikego::Lexer&);
static Silikego::SyntaxTreeNode *GetNumber(Silikego::Lexer&);
static Silikego::SyntaxTreeNode *GetUNumber(Silikego::Lexer&);
static Silikego::SyntaxTreeNode *GetFCall(Silikego::Lexer&);
static void                  GetArguments(Silikego::Lexer&, Silikego::BranchNode&);


Silikego::SyntaxTreeNode* Silikego::ParseInfix(Silikego::DataSource* NewSource)
{
	Silikego::Lexer MyLexer(NewSource);
	Silikego::SyntaxTreeNode* rVal = 0;

	if (MyLexer.GetToken().Type() == Token::EOL)
	{
		rVal = new Silikego::IntegerNode(0);
	}
	else
	{
		rVal = GetExpr0(MyLexer);
		if (MyLexer.GetToken().Type() != Token::EOL
			&& typeid(rVal) != typeid(Silikego::SyntaxErrorNode))
		{
			delete rVal;
			rVal = new SyntaxErrorNode();
		}
	}

	return rVal;
}

static Silikego::SyntaxTreeNode *GetExpr0(Silikego::Lexer& MyLexer)
{
	Silikego::SyntaxTreeNode *Left = GetExpr1(MyLexer);
	Silikego::SyntaxTreeNode *Rest = GetExpr0r(MyLexer);

	if (typeid(*Rest) == typeid(Silikego::NothingNode))
	{
		delete Rest;
		return Left;
	}

	if (reinterpret_cast<Silikego::BranchNode*>(Rest)->GraftLeft(Left))
		return Rest;

	delete Rest;
	delete Left;
	return new Silikego::SyntaxErrorNode();
}

static Silikego::SyntaxTreeNode *GetExpr0r(Silikego::Lexer& MyLexer)
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
		return new Silikego::NothingNode();
	}

	MyLexer.Next();

	Silikego::BranchNode *Branch = new Silikego::BranchNode(FunctionId);
	Branch->PushRight(0);
	Branch->PushRight(GetExpr1(MyLexer));
	Silikego::SyntaxTreeNode *Rest = GetExpr0r(MyLexer);

	if (typeid(*Rest) == typeid(Silikego::NothingNode))
	{
		delete Rest;
		return Branch;
	}
	else
	{
		reinterpret_cast<Silikego::BranchNode*>(Rest)->GraftLeft(Branch);
		return Rest;
	}
}

static Silikego::SyntaxTreeNode *GetExpr1(Silikego::Lexer& MyLexer)
{
	Silikego::SyntaxTreeNode *Left = GetExpr2(MyLexer);
	Silikego::SyntaxTreeNode *Rest = GetExpr1r(MyLexer);

	if (typeid(*Rest) == typeid(Silikego::NothingNode))
	{
		delete Rest;
		return Left;
	}

	if (reinterpret_cast<Silikego::BranchNode*>(Rest)->GraftLeft(Left))
		return Rest;

	delete Rest;
	delete Left;
	return new Silikego::SyntaxErrorNode();
}

static Silikego::SyntaxTreeNode *GetExpr1r(Silikego::Lexer& MyLexer)
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
			return new Silikego::NothingNode();
	}

	MyLexer.Next();

	Silikego::BranchNode *Branch = new Silikego::BranchNode(FunctionId);
	Branch->PushRight(0);
	Branch->PushRight(GetExpr2(MyLexer));
	Silikego::SyntaxTreeNode *Rest = GetExpr1r(MyLexer);

	if (typeid(*Rest) == typeid(Silikego::NothingNode))
	{
		delete Rest;
		return Branch;
	}
	else
	{
		reinterpret_cast<Silikego::BranchNode*>(Rest)->GraftLeft(Branch);
		return Rest;
	}
}


static Silikego::SyntaxTreeNode *GetExpr2(Silikego::Lexer& MyLexer)
{
	Silikego::SyntaxTreeNode *leftValue = GetExpr3(MyLexer);
	Silikego::SyntaxTreeNode *Rest = GetExpr2lf(MyLexer);

	if (typeid(*Rest) == typeid(Silikego::NothingNode))
	{
		delete Rest;
		return leftValue;
	}

	Silikego::BranchNode *rVal = new Silikego::BranchNode("power");
	rVal->PushRight(leftValue);
	rVal->PushRight(Rest);
	return rVal;
}

static Silikego::SyntaxTreeNode *GetExpr2lf(Silikego::Lexer& MyLexer)
{
	if (MyLexer.GetToken().Type() != '^')
		return new Silikego::NothingNode();

	MyLexer.Next();

	switch (MyLexer.GetToken().Type())
	{
	case Silikego::Token::INTEGER:
	case Silikego::Token::FLOAT:
	case '-':
	case Silikego::Token::ID:
	case '(':
		return GetExpr2(MyLexer);
	default:
		return new Silikego::SyntaxErrorNode();
	}
}

static Silikego::SyntaxTreeNode *GetExpr3(Silikego::Lexer& MyLexer)
{
	Silikego::SyntaxTreeNode *leftValue = GetAtom(MyLexer);
	Silikego::SyntaxTreeNode *Rest = GetExpr3lf(MyLexer);

	if (typeid(*Rest) == typeid(Silikego::NothingNode))
	{
		delete Rest;
		return leftValue;
	}

	Silikego::BranchNode *rVal = new Silikego::BranchNode("dice");
	rVal->PushRight(leftValue);
	rVal->PushRight(Rest);
	return rVal;
}

static Silikego::SyntaxTreeNode *GetExpr3lf(Silikego::Lexer& MyLexer)
{
	if(MyLexer.GetToken().Type() != 'd')
		return new Silikego::NothingNode();

	MyLexer.Next();

	if (MyLexer.GetToken().Type() == Silikego::Token::INTEGER)
	{
		Silikego::SyntaxTreeNode *rVal = new Silikego::IntegerNode(MyLexer.GetToken().Integer());
		MyLexer.Next();
		return rVal;
	}
	else
	{
		return new Silikego::SyntaxErrorNode();
	}
}

static Silikego::SyntaxTreeNode *GetAtom(Silikego::Lexer& MyLexer)
{
	Silikego::SyntaxTreeNode *value;

	switch(MyLexer.GetToken().Type())
	{
	case '-':
	case Silikego::Token::INTEGER:
	case Silikego::Token::FLOAT:
		return GetNumber(MyLexer);
	case '(':
		MyLexer.Next();
		value = GetExpr0(MyLexer);

		if (MyLexer.GetToken().Type() != ')')
		{
			delete value;
			return new Silikego::SyntaxErrorNode();
		}

		MyLexer.Next();
		return value;
	case Silikego::Token::ID:
		return GetFCall(MyLexer);
	default:
		return new Silikego::SyntaxErrorNode();
	}
}

static Silikego::SyntaxTreeNode *GetNumber(Silikego::Lexer& MyLexer)
{
	Silikego::SyntaxTreeNode *rVal;

	switch (MyLexer.GetToken().Type())
	{
	case Silikego::Token::INTEGER:
	case Silikego::Token::FLOAT:
		return GetUNumber(MyLexer);
	case '-':
		MyLexer.Next();
		rVal = GetUNumber(MyLexer);
		rVal->Negate();
		return rVal;
	default:
		return new Silikego::SyntaxErrorNode();
	}
}

static Silikego::SyntaxTreeNode *GetUNumber(Silikego::Lexer& MyLexer)
{
	Silikego::SyntaxTreeNode *rVal;

	switch (MyLexer.GetToken().Type())
	{
	case Silikego::Token::INTEGER:
		rVal = new Silikego::IntegerNode(MyLexer.GetToken().Integer());
		MyLexer.Next();
		return rVal;
	case Silikego::Token::FLOAT:
		rVal = new Silikego::FloatNode(MyLexer.GetToken().Float());
		MyLexer.Next();
		return rVal;
	default:
		return new Silikego::SyntaxErrorNode();
	}
}

static Silikego::SyntaxTreeNode *GetFCall(Silikego::Lexer& MyLexer)
{
	if (MyLexer.GetToken().Type() != Silikego::Token::ID)
		return new Silikego::SyntaxErrorNode();

	Silikego::BranchNode *rVal = new Silikego::BranchNode(MyLexer.GetToken().Id());
	MyLexer.Next();

	if (MyLexer.GetToken().Type() != '(')
	{
		rVal->PushRight(new Silikego::SyntaxErrorNode());
		return rVal;
	}
	MyLexer.Next();

	GetArguments(MyLexer, *rVal);

	if (MyLexer.GetToken().Type() != ')')
	{
		rVal->PushRight(new Silikego::SyntaxErrorNode());
		return rVal;
	}
	MyLexer.Next();

	return rVal;
}

static void GetArguments(Silikego::Lexer& MyLexer, Silikego::BranchNode& rVal)
{
	while(true)
	{
		Silikego::SyntaxTreeNode *Expression = GetExpr0(MyLexer);
		rVal.PushRight(Expression);

		if (typeid(*Expression) == typeid(Silikego::SyntaxErrorNode)
			|| MyLexer.GetToken().Type() == ')')
		{
			break;
		}
		else if (MyLexer.GetToken().Type() != ',')
		{
			rVal.PushRight(new Silikego::SyntaxErrorNode());
			break;
		}
		MyLexer.Next();
	}
}
