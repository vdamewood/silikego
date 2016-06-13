/* SyntaxTree.cc: Abstract syntax tree classes
 * Copyright 2012, 2014, 2015, 2016 Vincent Damewood
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

#include "SyntaxTree.hh"
#include "FunctionCaller.hh"

Silikego::IntegerNode::IntegerNode(int NewValue)
{
	MyInteger = NewValue;
}

Silikego::Value Silikego::IntegerNode::Evaluate()
{
	return MyInteger;
}

void Silikego::IntegerNode::Negate()
{
	MyInteger *= -1;
}

Silikego::FloatNode::FloatNode(float NewValue)
{
	MyFloat = NewValue;
}

Silikego::Value Silikego::FloatNode::Evaluate()
{
	return MyFloat;
}

void Silikego::FloatNode::Negate()
{
	MyFloat *= -1.0;
}

Silikego::BranchNode::BranchNode(const char *NewId)
{
	MyId = new char[strlen(NewId) + 1];
	std::strcpy(MyId, NewId);
	MyChildren = std::list<Silikego::SyntaxTreeNode *>();
	IsNegated = false;
}

Silikego::BranchNode::~BranchNode()
{
	delete[] MyId;
	for (std::list<SyntaxTreeNode*>::iterator i = MyChildren.begin(); i != MyChildren.end(); i++)
		delete *i;
}

Silikego::Value Silikego::BranchNode::Evaluate()
{
	Silikego::Value rVal;
	std::vector<Silikego::Value> Arguments;

	if (MyChildren.size())
	{
		std::list<Silikego::SyntaxTreeNode *>::iterator i;
		for (i = MyChildren.begin(); i != MyChildren.end(); i++)
		{
			Silikego::Value Current = (*i)->Evaluate();
			if (!Current.IsNumber())
				return Current;

			Arguments.push_back(Current);
		}
	}

	rVal = Silikego::FunctionCaller::Call(MyId, Arguments);
	if (IsNegated)
	{
		if (rVal.Status() == Value::INTEGER)
			rVal = rVal.Integer() * -1;
		else if (rVal.Status() == Value::FLOAT)
			rVal = rVal.Float() * (float)-1.0;
	}

	return rVal;
}

void Silikego::BranchNode::Negate()
{
	IsNegated = !IsNegated;
}

void Silikego::BranchNode::PushLeft(Silikego::SyntaxTreeNode *NewChild)
{
	MyChildren.push_front(NewChild);
}

void Silikego::BranchNode::PushRight(Silikego::SyntaxTreeNode *NewChild)
{
	MyChildren.push_back(NewChild);
}


bool Silikego::BranchNode::GraftLeft(Silikego::SyntaxTreeNode *NewChild)
{
	if (MyChildren.size() == 0)
	{
		return false;
	}
	else if (MyChildren.front() == 0)
	{
		MyChildren.front() = NewChild;
		return true;
	}
	else
	{
		if (BranchNode *ChildBranch = dynamic_cast<BranchNode*>(MyChildren.front()))
			return ChildBranch->GraftLeft(NewChild);
		else
			return false;
	}
}

bool Silikego::BranchNode::GraftRight(Silikego::SyntaxTreeNode *NewChild)
{
	if (MyChildren.size() == 0)
	{
		return false;
	}
	else if (MyChildren.back() == 0)
	{
		MyChildren.back() = NewChild;
		return true;
	}
	else
	{
		if (BranchNode *ChildBranch = dynamic_cast<BranchNode*>(MyChildren.back()))
			return ChildBranch->GraftRight(NewChild);
		else
			return false;
	}
}

Silikego::SyntaxErrorNode::~SyntaxErrorNode(void)
{
}

Silikego::Value Silikego::SyntaxErrorNode::Evaluate(void)
{
	return Silikego::Value::SYNTAX_ERR;
}

void Silikego::SyntaxErrorNode::Negate(void)
{
}

Silikego::NothingNode::~NothingNode(void)
{
	// Do Nothing
}

Silikego::Value Silikego::NothingNode::Evaluate(void)
{
	return 0;
}

void Silikego::NothingNode::Negate(void)
{
	// Do Nothing
}
