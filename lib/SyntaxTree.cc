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

#include <cstring>
#include <list>

#include "SyntaxTree.h"
#include "FunctionCaller.h"

namespace Silikego
{
	SyntaxTreeNode::~SyntaxTreeNode() { }

	class IntegerNode::State
	{
	public:
		State(int NewValue) : IntegerValue(NewValue) { }
		int IntegerValue;
	};

	IntegerNode::IntegerNode(int NewValue) : S(new State(NewValue)) { }

	IntegerNode::~IntegerNode()
	{
		delete S;
	}

	Value IntegerNode::Evaluate()
	{
		return S->IntegerValue;
	}

	void IntegerNode::Negate()
	{
		S->IntegerValue *= -1;
	}

	class FloatNode::State
	{
	public:
		State(double NewValue) : FloatValue(NewValue) { }
		double FloatValue;
	};

	FloatNode::FloatNode(double NewValue) : S(new State(NewValue)) { }

	FloatNode::~FloatNode()
	{
		delete S;
	}

	Value FloatNode::Evaluate()
	{
		return S->FloatValue;
	}

	void FloatNode::Negate()
	{
		S->FloatValue *= -1.0;
	}

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
		std::list<SyntaxTreeNode *> Children = std::list<SyntaxTreeNode *>();
	};

	BranchNode::BranchNode(const char *NewId) : S(new State(NewId))
	{
	}

	BranchNode::~BranchNode()
	{
		delete S;
	}

	Value BranchNode::Evaluate()
	{
		Value rVal;
		std::vector<Value> Arguments;

		if (S->Children.size())
		{
			std::list<SyntaxTreeNode *>::iterator i;
			for (i = S->Children.begin(); i != S->Children.end(); i++)
			{
				Value Current = (*i)->Evaluate();
				if (!Current.IsNumber())
					return Current;

				Arguments.push_back(Current);
			}
		}

		rVal = FunctionCaller::Call(S->Id.c_str(), Arguments);
		if (S->IsNegated)
		{
			if (rVal.Status() == Value::INTEGER)
				rVal = rVal.Integer() * -1;
			else if (rVal.Status() == Value::FLOAT)
				rVal = rVal.Float() * (float)-1.0;
		}

		return rVal;
	}

	void BranchNode::Negate()
	{
		S->IsNegated = !S->IsNegated;
	}

	void BranchNode::PushLeft(SyntaxTreeNode *NewChild)
	{
		S->Children.push_front(NewChild);
	}

	void BranchNode::PushRight(SyntaxTreeNode *NewChild)
	{
		S->Children.push_back(NewChild);
	}

	bool BranchNode::GraftLeft(SyntaxTreeNode *NewChild)
	{
		if (S->Children.size() == 0)
		{
			return false;
		}
		else if (S->Children.front() == 0)
		{
			S->Children.front() = NewChild;
			return true;
		}
		else
		{
			if (BranchNode *ChildBranch = dynamic_cast<BranchNode*>(S->Children.front()))
				return ChildBranch->GraftLeft(NewChild);
			else
				return false;
		}
	}

	bool BranchNode::GraftRight(SyntaxTreeNode *NewChild)
	{
		if (S->Children.size() == 0)
		{
			return false;
		}
		else if (S->Children.back() == 0)
		{
			S->Children.back() = NewChild;
			return true;
		}
		else
		{
			if (BranchNode *ChildBranch = dynamic_cast<BranchNode*>(S->Children.back()))
				return ChildBranch->GraftRight(NewChild);
			else
				return false;
		}
	}

	SyntaxErrorNode::~SyntaxErrorNode()
	{
	}

	Value SyntaxErrorNode::Evaluate()
	{
		return Value::SYNTAX_ERR;
	}

	void SyntaxErrorNode::Negate()
	{
	}

	NothingNode::~NothingNode()
	{
		// Do Nothing
	}

	Value NothingNode::Evaluate()
	{
		return 0;
	}

	void NothingNode::Negate()
	{
		// Do Nothing
	}
}