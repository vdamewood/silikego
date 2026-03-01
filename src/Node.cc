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


#include <array>
#include <deque>
#include <optional>
#include <string>
#include <utility>
#include <variant>

#include <SilikegoCore/Node.h>
#include <SilikegoCore/Value.h>

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
			if (std::holds_alternative<Branch>(data))
			{
				data = Branch{std::get<Branch>(source.data).id};
				std::get<Branch>(data).is_negated
					= std::get<Branch>(source.data).is_negated;
				for(const auto& i
						: std::get<Branch>(source.data).children)
					std::get<Branch>(data).children.push_back(i);
			}
			else
			{
				data = source.data;
			}
		}

		bool OutOfBounds(size_t index)
		{
			return (!std::holds_alternative<Branch>(data))
				|| index >= std::get<Branch>(data).children.size();
		}

		using data_v = std::variant<std::monostate, Value, Branch>;
		data_v data;
		std::array<NodeStatus, std::variant_size_v<data_v>> conversion
		{
			NodeStatus::Nothing,
			NodeStatus::Leaf,
			NodeStatus::Branch
		};
	};

	Node::Node() : _impl(new(std::nothrow) Impl()) {}
	Node::Node(int source)
		: _impl(new(std::nothrow) Impl(Value(source))) {}
	Node::Node(long long int new_value)
		: _impl(new(std::nothrow) Impl(Value(new_value))) {}
	Node::Node(double new_value)
		: _impl(new(std::nothrow) Impl(Value(new_value))) {}
	Node::Node(Error new_error)
		: _impl(new(std::nothrow) Impl(Value(new_error))) {}
	Node::Node(const Value& source)
		: _impl(new(std::nothrow) Impl(source)) {}
	Node::Node(const std::string& new_id)
		: _impl(new(std::nothrow) Impl(Branch(new_id))) {}

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
		if (isEmpty())
			_impl = new(std::nothrow) Impl{Value{source}};
		else
			_impl->data = Value{source};
		return *this;
	}

	Node& Node::operator=(int source)
	{
		if (isEmpty())
			_impl = new(std::nothrow) Impl{Value{source}};
		else
			_impl->data = Value{source};
		return *this;
	}

	Node& Node::operator=(long long int source)
	{
		if (isEmpty())
			_impl = new(std::nothrow) Impl{Value{source}};
		else
			_impl->data = Value{source};
		return *this;
	}

	Node& Node::operator=(double source)
	{
		if (isEmpty())
			_impl = new(std::nothrow) Impl{Value{source}};
		else
			_impl->data = Value{source};
		return *this;
	}

	Node& Node::operator=(const Value& source)
	{
		if (isEmpty())
			_impl = new(std::nothrow) Impl{Value{source}};
		else
			_impl->data = source;
		return *this;
	}

	Node& Node::operator=(const std::string& source)
	{
		if (isEmpty())
			_impl = new(std::nothrow) Impl{Branch{source}};
		else
			_impl->data = Branch{source};
		return *this;
	}

	Node& Node::operator=(const Node& source)
	{
		if (this != &source)
		{
			delete _impl;
			_impl = new(std::nothrow) Impl{*source._impl};
		}
		return *this;
	}

	Node& Node::operator=(Node&& right_side)
	{
		if (this != &right_side)
		{
			delete _impl;
			_impl = right_side._impl;
			right_side._impl = nullptr;
		}
		return *this;
	}

	size_t Node::countChildren() const
	{
		if (isEmpty() || !std::holds_alternative<Branch>(_impl->data))
			return 0;

		return std::get<Branch>(_impl->data).children.size();
	}

	const Node* Node::fetchChild(size_t child_index) const
	{
		if (isEmpty() || _impl->OutOfBounds(child_index))
			return nullptr;

		return &std::get<Branch>(_impl->data)
			.children[child_index];
	}

	const std::string* Node::id() const
	{
		if (isEmpty() || !std::holds_alternative<Branch>(_impl->data))
			return nullptr;

		return &std::get<Branch>(_impl->data).id;
	}

	bool Node::insert(size_t position, const Node& new_child)
	{
		return insert(position, Node{new_child});
	}

	bool Node::insert(size_t position, Node&& new_child)
	{
		if (isEmpty() || _impl->OutOfBounds(position))
			return false;

		std::get<Branch>(_impl->data).children.insert(
			std::get<Branch>(_impl->data)
				.children.begin() + position,
			std::move(new_child));

		return true;
	}

	bool Node::isNegated() const
	{
		if (isEmpty())
			return false;

		return std::visit([](auto&& data)
		{
			using T = std::decay_t<decltype(data)>;
			if constexpr (std::is_same_v<T, std::monostate>)
				return false;
			if constexpr (std::is_same_v<T, Value>)
				switch (data.status())
				{
				case ValueStatus::Error:
					return false;
				case ValueStatus::Integer:
					return data.integer() < 0;
				case ValueStatus::Real:
					return data.real() < 0.0;
				}
			if constexpr (std::is_same_v<T, Branch>)
				return data.is_negated;
		}, _impl->data);
	}

	void Node::negate()
	{
		if (isEmpty())
			return;

		std::visit([](auto&& data)
		{
			using T = std::decay_t<decltype(data)>;
			if constexpr (std::is_same_v<T, std::monostate>)
				; // Do nothing.
			if constexpr (std::is_same_v<T, Value>)
				data.negate();
			if constexpr (std::is_same_v<T, Branch>)
				data.is_negated = !data.is_negated;
		}, _impl->data);
	}

	std::optional<Node> Node::pruneChild(size_t child_index)
	{
		if (isEmpty() || _impl->OutOfBounds(child_index))
			return std::nullopt;

		auto& children = std::get<Branch>(_impl->data).children;
		Node child = children[child_index];
		children.erase(children.begin() + child_index);
		return child;
	}

	bool Node::pushLeft(const Node& new_child)
	{
		return pushLeft(Node{new_child});
	}

	bool Node::pushLeft(Node&& new_child)
	{
		if (isEmpty() || !std::holds_alternative<Branch>(_impl->data))
			return false;

		std::get<Branch>(_impl->data)
			.children.push_front(new_child);
		return true;
	}

	bool Node::pushRight(const Node& new_child)
	{
		return pushRight(Node{new_child});
	}

	bool Node::pushRight(Node&& new_child)
	{
		if (isEmpty() || !std::holds_alternative<Branch>(_impl->data))
			return false;

		std::get<Branch>(_impl->data)
			.children.push_back(new_child);
		return true;
	}

	NodeStatus Node::status() const
	{
		if (isEmpty())
			return NodeStatus::Nothing;
		return _impl->conversion[_impl->data.index()];
	}

	const Value* Node::value() const
	{
		if (isEmpty() || !std::holds_alternative<Value>(_impl->data))
			return nullptr;

		return &std::get<Value>(_impl->data);
	}
}
