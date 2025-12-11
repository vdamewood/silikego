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

#include <deque>
#include <optional>
#include <string>
#include <variant>
#include <vector>

#include <SilikegoCore/SyntaxTree.h>
#include <SilikegoCore/FunctionCaller.h>
#include <SilikegoCore/Value.h>

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

	SyntaxTreeNode::SyntaxTreeNode(): _impl(new Impl()) {}
	SyntaxTreeNode::SyntaxTreeNode(long long int new_value): _impl(new Impl(Value(new_value))) {}
	SyntaxTreeNode::SyntaxTreeNode(double new_value): _impl(new Impl(Value(new_value))) {}
	SyntaxTreeNode::SyntaxTreeNode(Error new_error): _impl(new Impl(Value(new_error))) {}
	SyntaxTreeNode::SyntaxTreeNode(Value new_value): _impl(new Impl(new_value)) {}
	SyntaxTreeNode::SyntaxTreeNode(const std::string& new_id): _impl(new Impl(NodeBranch(new_id))) {}

	SyntaxTreeNode::SyntaxTreeNode(const SyntaxTreeNode& new_node)
		: _impl(new Impl(*new_node._impl))
	{}

	SyntaxTreeNode::SyntaxTreeNode(SyntaxTreeNode&& old_node)
		: _impl(old_node._impl)
	{
		old_node._impl = nullptr;
	}

	SyntaxTreeNode::~SyntaxTreeNode() {
		delete _impl;
	}

	SyntaxTreeNode& SyntaxTreeNode::operator=(const SyntaxTreeNode& right_side)
	{
		delete _impl;
		_impl = new Impl(*right_side._impl);
		return *this;
	}

	SyntaxTreeNode& SyntaxTreeNode::operator=(SyntaxTreeNode&& right_side)
	{
		_impl = right_side._impl;
		right_side._impl = nullptr;
		return *this;
	}

	Value SyntaxTreeNode::evaluate(FunctionCaller& caller)
	{
		// std::visit
		switch (_impl->data.index())
		{
		case Nothing:
			return Error::Syntax;
		case Leaf:
			return std::get<Value>(_impl->data);
		case Branch:
		{
			std::vector<Value> Arguments;

			for (auto& i : std::get<NodeBranch>(_impl->data).children)
			{
				Value current = i.evaluate(caller);
				if (current.isError())
					return current;
				Arguments.push_back(current);
			}

			Value result(caller.call(std::get<NodeBranch>(_impl->data).id, Arguments));
			if (std::get<NodeBranch>(_impl->data).is_negated)
				result.negate();

			return result;
		}
		default:
			return Error::Syntax;
		}
	}

	bool SyntaxTreeNode::collapse(FunctionCaller& caller)
	{
		if(_impl->data.index() != Branch)
			return false;
		
		_impl->data = evaluate(caller);
		return true;
	}

	void SyntaxTreeNode::negate()
	{
		// std::visit
		switch(_impl->data.index())
		{
		case Nothing:
			break;
		case Leaf:
			std::get<Value>(_impl->data).negate();
			break;
		case Branch:
			std::get<NodeBranch>(_impl->data).is_negated
				= !std::get<NodeBranch>(_impl->data).is_negated;
			break;
		}
	}

	bool SyntaxTreeNode::isError()
	{
		return (_impl->data.index() == Leaf)
			? std::get<Value>(_impl->data).isError()
			: false;
	}

	bool SyntaxTreeNode::isBranch()
	{
		return _impl->data.index() == Branch;
	}

	bool SyntaxTreeNode::isLeaf()
	{
		return _impl->data.index() == Leaf;
	}

	bool SyntaxTreeNode::isNothing()
	{
		return _impl->data.index() == Nothing;
	}

	bool SyntaxTreeNode::pushLeft(SyntaxTreeNode&& new_child)
	{
		if (_impl->data.index() != Branch)
			return false;

		std::get<NodeBranch>(_impl->data).children.push_front(new_child);
		return true;
	}

	bool SyntaxTreeNode::pushRight(SyntaxTreeNode&& new_child)
	{
		if (_impl->data.index() != Branch)
			return false;

		std::get<NodeBranch>(_impl->data).children.push_back(new_child);
		return true;
	}

	SyntaxTreeNode* SyntaxTreeNode::child(int child_index)
	{
		if ((child_index = _impl->checkBounds(child_index)) < 0)
			return nullptr;

		return &std::get<Branch>(_impl->data).children[child_index];
	}

	std::optional<SyntaxTreeNode> SyntaxTreeNode::pruneChild(int child_index)
	{
		if ((child_index = _impl->checkBounds(child_index)) < 0)
			return std::nullopt;

		auto& children = std::get<Branch>(_impl->data).children;
		SyntaxTreeNode child = children[child_index];
		children.erase(children.begin() + child_index);
		return child;
	}
}
