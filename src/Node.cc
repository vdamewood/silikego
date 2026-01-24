// Copyright 2012-2026 Vincent Damewood
// SPDX-License-Identifier: LGPL-3.0-or-later

// This file is part of Silikego.

// Silikego is free software: you can redistribute it and/or modify it
// under the terms of the GNU Lesser General Public License as published
// by the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// Silikego is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
// Lesser General Public License for more details.

// You should have received a copy of the GNU Lesser General Public
// License along with Silikego. If not, see
// <http://www.gnu.org/licenses/>.


#include <deque>
#include <optional>
#include <string>
#include <utility>
#include <variant>
#include <sys/types.h>

#include <SilikegoCore/Node.h>
#include <SilikegoCore/Value.h>

namespace
{
	const int NothingIndex = 0;
	const int LeafIndex = 1;
	const int BranchIndex = 2;
}

namespace Silikego
{
	struct Branch
	{
		Branch(const std::string& new_id) : id(new_id) { }
		std::string id;
		std::deque<Node> children;
		bool is_negated = false;
	};

	class Node::Impl
	{
	public:
		Impl()
		{ }

		Impl(const Value& source):
			data(source)
		{ }

		Impl(Branch new_branch):
			data(new_branch)
		{ }

		Impl(const Impl& source)
		{
			switch (source.data.index())
			{
			case LeafIndex:
				data = source.data;
				break;
			case BranchIndex:
			{
				data = Branch{std::get<BranchIndex>(source.data).id};
				std::get<BranchIndex>(data).is_negated
					= std::get<BranchIndex>(source.data).is_negated;
				for(const auto& i : std::get<BranchIndex>(source.data).children)
					std::get<BranchIndex>(data).children.push_back(i);
				break;
			}
			}
		}

		int checkBounds(int index)
		{
			if (data.index() != BranchIndex
				|| index >= std::get<BranchIndex>(data).children.size())
			{
				return -1;
			}

			return index;
		}

		std::variant<std::monostate, Value, Branch> data;
	}; // Impl

	Node::Node(): _impl(new(std::nothrow) Impl()) {}
	Node::Node(int source): _impl(new(std::nothrow) Impl(Value(source))) {}
	Node::Node(long long int new_value): _impl(new(std::nothrow) Impl(Value(new_value))) {}
	Node::Node(double new_value): _impl(new(std::nothrow) Impl(Value(new_value))) {}
	Node::Node(Error new_error): _impl(new(std::nothrow) Impl(Value(new_error))) {}
	Node::Node(const Value& source): _impl(new(std::nothrow) Impl(source)) {}
	Node::Node(const std::string& new_id): _impl(new(std::nothrow) Impl(Branch(new_id))) {}

	Node::Node(const Node& new_node)
		: _impl(new(std::nothrow) Impl(*new_node._impl))
	{}

	Node::Node(Node&& old_node)
		: _impl(old_node._impl)
	{
		old_node._impl = nullptr;
	}

	Node::~Node() {
		delete _impl;
	}

	Node& Node::operator=(Error source)
	{
		_impl->data = Value{source};
		return *this;
	}

	Node& Node::operator=(int source)
	{
		_impl->data = Value{source};
		return *this;
	}

	Node& Node::operator=(long long int source)
	{
		_impl->data = Value{source};
		return *this;
	}

	Node& Node::operator=(double source)
	{
		_impl->data = Value{source};
		return *this;
	}

	Node& Node::operator=(const Value& source)
	{
		_impl->data = Value{source};
		return *this;
	}

	Node& Node::operator=(const std::string& source)
	{
		_impl->data = Branch{source};
		return *this;
	}

	Node& Node::operator=(const Node& source)
	{
		if (this != &source)
		{
			delete _impl;
			_impl = new(std::nothrow) Impl(*source._impl);
		}
		return *this;
	}

	Node& Node::operator=(Node&& right_side)
	{
		if (this != &right_side)
		{
			_impl = right_side._impl;
			right_side._impl = nullptr;
		}
		return *this;
	}

	const std::string* Node::id() const
	{
		if (isEmpty() || _impl->data.index() != BranchIndex)
			return nullptr;

		return &std::get<BranchIndex>(_impl->data).id;
	}

	bool Node::isNegated() const
	{
		if (isEmpty())
			return false;

		switch (_impl->data.index())
		{
		case LeafIndex:
			switch (std::get<LeafIndex>(_impl->data).status())
			{
			case ValueStatus::Integer:
				return std::get<LeafIndex>(_impl->data).integer() < 0;
			case ValueStatus::Real:
				return std::get<LeafIndex>(_impl->data).real() < 0.0;
			default:
				return false;
			}
		case BranchIndex:
			return std::get<BranchIndex>(_impl->data).is_negated;
		default:
			return false;
		}
	}

	void Node::negate()
	{
		if (isEmpty())
			return;

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

	NodeStatus Node::status() const
	{
		if (isEmpty())
			return NodeStatus::Nothing;

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

	const Value* Node::value() const
	{
		if (isEmpty() || _impl->data.index() != LeafIndex)
			return nullptr;

		return &std::get<LeafIndex>(_impl->data);
	}

	bool Node::pushLeft(const Node& new_child)
	{
		return pushLeft(Node{new_child});
	}

	bool Node::pushLeft(Node&& new_child)
	{
		if (isEmpty() || _impl->data.index() != BranchIndex)
			return false;

		std::get<BranchIndex>(_impl->data).children.push_front(new_child);
		return true;
	}

	bool Node::pushRight(const Node& new_child)
	{
		return pushRight(Node{new_child});
	}

	bool Node::pushRight(Node&& new_child)
	{
		if (isEmpty() || _impl->data.index() != BranchIndex)
			return false;

		std::get<BranchIndex>(_impl->data).children.push_back(new_child);
		return true;
	}

	bool Node::insert(int position, const Node& new_child)
	{
		return insert(position, Node{new_child});
	}

	bool Node::insert(int position, Node&& new_child)
	{
		if (isEmpty() || _impl->data.index() != BranchIndex)
			return false;

			position = _impl->checkBounds(position);
		if (position < 0)
			return false;

		std::get<BranchIndex>(_impl->data).children.insert(
			std::get<BranchIndex>(_impl->data).children.begin() + position,
			std::move(new_child));

		return true;
	}

	size_t Node::countChildren() const
	{
		if (isEmpty() || _impl->data.index() != BranchIndex)
			return 0;

		return std::get<BranchIndex>(_impl->data).children.size();
	}

	const Node* Node::fetchChild(int child_index) const
	{
		if (isEmpty())
			return nullptr;

		if ((child_index = _impl->checkBounds(child_index)) < 0)
			return nullptr;

		return &std::get<BranchIndex>(_impl->data).children[child_index];
	}

	std::optional<Node> Node::pruneChild(int child_index)
	{
		if (isEmpty())
			return std::nullopt;

		if ((child_index = _impl->checkBounds(child_index)) < 0)
			return std::nullopt;

		auto& children = std::get<BranchIndex>(_impl->data).children;
		Node child = children[child_index];
		children.erase(children.begin() + child_index);
		return child;
	}
}
