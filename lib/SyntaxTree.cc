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
#include <list>

#include "SyntaxTree.h"
#include "FunctionCaller.h"

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

namespace Silikego
{
	class BranchNode::State
	{
	public:
		State(const char* newName) : Id(newName) {}
		~State()
		{
			for
				(std::list<SyntaxTreeNode*>::iterator i = Children.begin();
				i != Children.end();
				i++)
				delete *i;

		}
		bool IsNegated = false;
		std::string Id;
		std::list<SyntaxTreeNode *> Children = std::list<Silikego::SyntaxTreeNode *>();
	};
};

Silikego::BranchNode::BranchNode(const char *NewId) : MyState(new State(NewId))
{
}

Silikego::BranchNode::~BranchNode()
{
	delete MyState;
}

Silikego::Value Silikego::BranchNode::Evaluate()
{
	Silikego::Value rVal;
	std::vector<Silikego::Value> Arguments;

	if (MyState->Children.size())
	{
		std::list<Silikego::SyntaxTreeNode *>::iterator i;
		for (i = MyState->Children.begin(); i != MyState->Children.end(); i++)
		{
			Silikego::Value Current = (*i)->Evaluate();
			if (!Current.IsNumber())
				return Current;

			Arguments.push_back(Current);
		}
	}

	rVal = Silikego::FunctionCaller::Call(MyState->Id.c_str(), Arguments);
	if (MyState->IsNegated)
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
	MyState->IsNegated = !MyState->IsNegated;
}

void Silikego::BranchNode::PushLeft(Silikego::SyntaxTreeNode *NewChild)
{
	MyState->Children.push_front(NewChild);
}

void Silikego::BranchNode::PushRight(Silikego::SyntaxTreeNode *NewChild)
{
	MyState->Children.push_back(NewChild);
}

bool Silikego::BranchNode::GraftLeft(Silikego::SyntaxTreeNode *NewChild)
{
	if (MyState->Children.size() == 0)
	{
		return false;
	}
	else if (MyState->Children.front() == 0)
	{
		MyState->Children.front() = NewChild;
		return true;
	}
	else
	{
		if (BranchNode *ChildBranch = dynamic_cast<BranchNode*>(MyState->Children.front()))
			return ChildBranch->GraftLeft(NewChild);
		else
			return false;
	}
}

bool Silikego::BranchNode::GraftRight(Silikego::SyntaxTreeNode *NewChild)
{
	if (MyState->Children.size() == 0)
	{
		return false;
	}
	else if (MyState->Children.back() == 0)
	{
		MyState->Children.back() = NewChild;
		return true;
	}
	else
	{
		if (BranchNode *ChildBranch = dynamic_cast<BranchNode*>(MyState->Children.back()))
			return ChildBranch->GraftRight(NewChild);
		else
			return false;
	}
}

Silikego::SyntaxErrorNode::~SyntaxErrorNode()
{
}

Silikego::Value Silikego::SyntaxErrorNode::Evaluate()
{
	return Silikego::Value::SYNTAX_ERR;
}

void Silikego::SyntaxErrorNode::Negate()
{
}

Silikego::NothingNode::~NothingNode()
{
	// Do Nothing
}

Silikego::Value Silikego::NothingNode::Evaluate()
{
	return 0;
}

void Silikego::NothingNode::Negate()
{
	// Do Nothing
}
