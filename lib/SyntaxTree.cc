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
#include <memory>

#include "SyntaxTree.h"
#include "FunctionCaller.h"

namespace Silikego
{
	SyntaxTreeNode::~SyntaxTreeNode() { }

	class IntegerNode::State
	{
	public:
		State(long long int NewValue) : IntegerValue(NewValue) { }
		State(const State& RightSide) : IntegerValue(RightSide.IntegerValue) { }
		State& operator=(const State& RightSide)
		{
			IntegerValue = RightSide.IntegerValue;
			return *this;
		}
		long long int IntegerValue;
	};

	IntegerNode::IntegerNode(short int NewValue) : S(new State(static_cast<long long int>(NewValue))) { }
	IntegerNode::IntegerNode(int NewValue) : S(new State(static_cast<long long int>(NewValue))) { }
	IntegerNode::IntegerNode(long int NewValue) : S(new State(static_cast<long long int>(NewValue))) { }
	IntegerNode::IntegerNode(long long int NewValue) : S(new State(static_cast<long long int>(NewValue))) { }

	IntegerNode::IntegerNode(const IntegerNode& RightSide)
	{
		S = new State(*RightSide.S);
	}

	IntegerNode::~IntegerNode()
	{
		delete S;
	}

	const IntegerNode& IntegerNode::operator=(const IntegerNode& RightSide)
	{
		*S = *RightSide.S;
		return *this;
	}


	Value IntegerNode::Evaluate()
	{
		return S->IntegerValue;
	}

	void IntegerNode::Negate()
	{
		S->IntegerValue *= -1;
	}

	bool IntegerNode::IsError()
	{
		return false;
	}

	class FloatNode::State
	{
	public:
		State(double NewValue) : FloatValue(NewValue) { }
		State(const State& RightSide) : FloatValue(RightSide.FloatValue) { }
		State& operator=(const State& RightSide)
		{
			FloatValue = RightSide.FloatValue;
			return *this;
		}
		double FloatValue;
	};

	FloatNode::FloatNode(float NewValue) : S(new State(static_cast<double>(NewValue))) { }
	FloatNode::FloatNode(double NewValue) : S(new State(NewValue)) { }

	FloatNode::FloatNode(const FloatNode& RightSide)
	{
		S = new State(*RightSide.S);
	}

	FloatNode::~FloatNode()
	{
		delete S;
	}

	const FloatNode& FloatNode::operator=(const FloatNode& RightSide)
	{
		*S = *RightSide.S;
		return *this;
	}

	Value FloatNode::Evaluate()
	{
		return S->FloatValue;
	}

	void FloatNode::Negate()
	{
		S->FloatValue *= -1.0;
	}

	bool FloatNode::IsError()
	{
		return false;
	}

	class BranchNode::State
	{
	public:
		State(const std::string& newName) : Id(newName) {}

		bool IsNegated = false;
		std::string Id;
		std::list< std::unique_ptr<SyntaxTreeNode> > Children;
	};

	BranchNode::BranchNode(const std::string& NewId) : S(new State(NewId))
	{
	}

	BranchNode::~BranchNode()
	{
		delete S;
	}

	Value BranchNode::Evaluate()
	{
		std::vector<Value> Arguments;

		if (S->Children.size())
			for (auto& i : S->Children)
			{
				Value Current = i->Evaluate();
				if (!Current.IsNumber())
					return Current;

				Arguments.push_back(Current);
			}

		Value rVal(FunctionCaller::Call(S->Id.c_str(), Arguments));
		if (S->IsNegated)
		{
			if (rVal.Status() == Value::INTEGER)
				rVal = rVal.Integer() * -1;
			else if (rVal.Status() == Value::FLOAT)
				rVal = rVal.Float() * -1.0;
		}

		return rVal;
	}

	void BranchNode::Negate()
	{
		S->IsNegated = !S->IsNegated;
	}

	bool BranchNode::IsError()
	{
		return false;
	}

	void BranchNode::PushLeft(std::unique_ptr<SyntaxTreeNode> NewChild)
	{
		S->Children.push_front(std::move(NewChild));
	}

	void BranchNode::PushRight(std::unique_ptr<SyntaxTreeNode> NewChild)
	{
		S->Children.push_back(std::move(NewChild));
	}

	bool BranchNode::GraftLeft(std::unique_ptr<SyntaxTreeNode> NewChild)
	{
		if (S->Children.size() == 0)
		{
			return false;
		}
		else if (S->Children.front() == nullptr)
		{
            S->Children.front() = std::move(NewChild);
			return true;
		}
		else
		{
			if (BranchNode *ChildBranch = dynamic_cast<BranchNode*>(S->Children.front().get()))
                return ChildBranch->GraftLeft(std::move(NewChild));
			else
				return false;
		}
	}

	bool BranchNode::GraftRight(std::unique_ptr<SyntaxTreeNode> NewChild)
	{
		if (S->Children.size() == 0)
		{
			return false;
		}
		else if (S->Children.back() == nullptr)
		{
			S->Children.back() = std::move(NewChild);
			return true;
		}
		else
		{
			if (BranchNode *ChildBranch = dynamic_cast<BranchNode*>(S->Children.back().get()))
				return ChildBranch->GraftRight(std::move(NewChild));
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
		// Do nothing
	}

	bool SyntaxErrorNode::IsError()
	{
		return true;
	}

}