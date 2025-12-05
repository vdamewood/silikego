/* SyntaxTree.cc: Abstract syntax tree classes
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

#include <string>
#include <vector>
#include <deque>
#include <variant>

#include <SilikegoCore/SyntaxTree.h>
#include <SilikegoCore/FunctionCaller.h>

namespace
{
	const int Nothing = 0;
	const int Leaf = 1;
	const int Branch = 2;
}

namespace Silikego
{
	struct NodeBranch
	{
		NodeBranch(const std::string& new_id) : id(new_id) { }
		std::string id;
		std::deque<SyntaxTreeNode> children;
		bool is_negated = false;
	};

	class SyntaxTreeNode::Impl
	{
	public:
		Impl()
		{ }

		Impl(Value new_value):
			data(new_value)
		{ }

		Impl(NodeBranch new_branch):
			data(new_branch)
		{ }

		int checkBounds(int index)
		{
			if (data.index() != Branch
					|| index >= std::get<Branch>(data).children.size()
					|| index < -std::get<Branch>(data).children.size()
			)
				return -1;

			if (index < 0)
				return index + std::get<Branch>(data).children.size();

			return index;
		}

		std::variant<std::monostate, Value, NodeBranch> data;
	}; // Impl

	SyntaxTreeNode::SyntaxTreeNode(): I(new Impl()) {}
	SyntaxTreeNode::SyntaxTreeNode(long long int new_value): I(new Impl(Value(new_value))) {}
	SyntaxTreeNode::SyntaxTreeNode(double new_value): I(new Impl(Value(new_value))) {}
	SyntaxTreeNode::SyntaxTreeNode(Error new_error): I(new Impl(Value(new_error))) {}
	SyntaxTreeNode::SyntaxTreeNode(Value new_value): I(new Impl(new_value)) {}
	SyntaxTreeNode::SyntaxTreeNode(const std::string& new_id): I(new Impl(NodeBranch(new_id))) {}

	SyntaxTreeNode::SyntaxTreeNode(const SyntaxTreeNode& new_node)
		: I(new Impl(*new_node.I))
	{}

	SyntaxTreeNode::SyntaxTreeNode(SyntaxTreeNode&& old_node)
		: I(old_node.I)
	{
		old_node.I = nullptr;
	}

	SyntaxTreeNode::~SyntaxTreeNode() {
		delete I;
	}

	SyntaxTreeNode& SyntaxTreeNode::operator=(const SyntaxTreeNode& right_side)
	{
		delete I;
		I = new Impl(*right_side.I);
		return *this;
	}

	SyntaxTreeNode& SyntaxTreeNode::operator=(SyntaxTreeNode&& right_side)
	{
		I = right_side.I;
		right_side.I = nullptr;
		return *this;
	}

	Value SyntaxTreeNode::Evaluate(FunctionCaller& caller)
	{
		// std::visit
		switch (I->data.index())
		{
		case Nothing:
			return Error::Syntax;
		case Leaf:
			return std::get<Value>(I->data);
		case Branch:
		{
			std::vector<Value> Arguments;

			for (auto& i : std::get<NodeBranch>(I->data).children)
			{
				Value current = i.Evaluate(caller);
				if (current.isError())
					return current;
				Arguments.push_back(current);
			}

			Value result(caller.Call(std::get<NodeBranch>(I->data).id.c_str(), Arguments));
			if (std::get<NodeBranch>(I->data).is_negated)
				result.negate();

			return result;
		}
		default:
			return Error::Syntax;
		}
	}

	SyntaxTreeNode SyntaxTreeNode::collapse(FunctionCaller& caller)
	{
		return Evaluate(caller);
	}

	void SyntaxTreeNode::Negate()
	{
		// std::visit
		switch(I->data.index())
		{
		case Nothing:
			break;
		case Leaf:
			std::get<Value>(I->data).negate();
			break;
		case Branch:
			std::get<NodeBranch>(I->data).is_negated
				= !std::get<NodeBranch>(I->data).is_negated;
			break;
		}
	}

	bool SyntaxTreeNode::IsError()
	{
		return (I->data.index() == Leaf)
			? std::get<Value>(I->data).isError()
			: false;
	}

	bool SyntaxTreeNode::IsBranch()
	{
		return I->data.index() == Branch;
	}

	bool SyntaxTreeNode::IsLeaf()
	{
		return I->data.index() == Leaf;
	}

	bool SyntaxTreeNode::IsNothing()
	{
		return I->data.index() == Nothing;
	}

	bool SyntaxTreeNode::PushLeft(SyntaxTreeNode&& new_child)
	{
		if (I->data.index() != Branch)
			return false;

		std::get<NodeBranch>(I->data).children.push_front(new_child);
		return true;
	}

	bool SyntaxTreeNode::PushRight(SyntaxTreeNode&& new_child)
	{
		if (I->data.index() != Branch)
			return false;

		std::get<NodeBranch>(I->data).children.push_back(new_child);
		return true;
	}

	SyntaxTreeNode* SyntaxTreeNode::getChild(int child_index)
	{
		if ((child_index = I->checkBounds(child_index)) < 0)
			return nullptr;

		return &std::get<Branch>(I->data).children[child_index];
	}

	std::optional<SyntaxTreeNode> SyntaxTreeNode::pruneChild(int child_index)
	{
		if ((child_index = I->checkBounds(child_index)) < 0)
			return std::nullopt;

		auto& children = std::get<Branch>(I->data).children;
		SyntaxTreeNode child = children[child_index];
		children.erase(children.begin() + child_index);
		return child;
	}

	bool SyntaxTreeNode::collapseChild(int child_index, FunctionCaller& caller)
	{
		if ((child_index = I->checkBounds(child_index)) < 0)
			return false;

		auto& children = std::get<Branch>(I->data).children;
		children[child_index] = children[child_index].collapse(caller);
		return true;
	}
}
