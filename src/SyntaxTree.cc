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
	const int NothingIndex = 0;
	const int LeafIndex = 1;
	const int BranchIndex = 2;
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
			if (data.index() != BranchIndex
					|| index >= std::get<BranchIndex>(data).children.size()
					|| index < -std::get<BranchIndex>(data).children.size()
			)
				return -1;

			if (index < 0)
				return index + std::get<BranchIndex>(data).children.size();

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
		case NothingIndex:
			return Error::Syntax;
		case LeafIndex:
			return std::get<LeafIndex>(_impl->data);
		case BranchIndex:
		{
			std::vector<Value> Arguments;

			for (auto& i : std::get<BranchIndex>(_impl->data).children)
			{
				Value current = i.evaluate(caller);
				if (current.status() == ValueStatus::Error)
					return current;
				Arguments.push_back(current);
			}

			Value result(caller.call(std::get<BranchIndex>(_impl->data).id, Arguments));
			if (std::get<BranchIndex>(_impl->data).is_negated)
				result.negate();

			return result;
		}
		default:
			return Error::Syntax;
		}
	}

	bool SyntaxTreeNode::collapse(FunctionCaller& caller)
	{
		if(_impl->data.index() != BranchIndex)
			return false;
		
		_impl->data = evaluate(caller);
		return true;
	}

	void SyntaxTreeNode::negate()
	{
		// std::visit
		switch(_impl->data.index())
		{
		case NothingIndex:
			break;
		case LeafIndex:
			std::get<LeafIndex>(_impl->data).negate();
			break;
		case BranchIndex:
			std::get<BranchIndex>(_impl->data).is_negated
				= !std::get<BranchIndex>(_impl->data).is_negated;
			break;
		}
	}

	bool SyntaxTreeNode::isError()
	{
		return (_impl->data.index() == LeafIndex)
			? (std::get<LeafIndex>(_impl->data).status()
				== ValueStatus::Error)
			: false;
	}

	NodeStatus SyntaxTreeNode::status()
	{
		switch (_impl->data.index())
		{
		case NothingIndex:
			return NodeStatus::Nothing;
		case LeafIndex:
			return NodeStatus::Leaf;
		case BranchIndex:
			return NodeStatus::Branch;
		default:
			throw; // shouldn't happen
		}
	}

	bool SyntaxTreeNode::pushLeft(SyntaxTreeNode&& new_child)
	{
		if (_impl->data.index() != BranchIndex)
			return false;

		std::get<BranchIndex>(_impl->data).children.push_front(new_child);
		return true;
	}

	bool SyntaxTreeNode::pushRight(SyntaxTreeNode&& new_child)
	{
		if (_impl->data.index() != BranchIndex)
			return false;

		std::get<BranchIndex>(_impl->data).children.push_back(new_child);
		return true;
	}

	SyntaxTreeNode* SyntaxTreeNode::fetchChild(int child_index)
	{
		if ((child_index = _impl->checkBounds(child_index)) < 0)
			return nullptr;

		return &std::get<BranchIndex>(_impl->data).children[child_index];
	}

	std::optional<SyntaxTreeNode> SyntaxTreeNode::pruneChild(int child_index)
	{
		if ((child_index = _impl->checkBounds(child_index)) < 0)
			return std::nullopt;

		auto& children = std::get<BranchIndex>(_impl->data).children;
		SyntaxTreeNode child = children[child_index];
		children.erase(children.begin() + child_index);
		return child;
	}
}
