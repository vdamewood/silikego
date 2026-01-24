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


#if !defined SILIKEGO_CORE_NODE_H
#define SILIKEGO_CORE_NODE_H

#include <optional>
#include <string>

#include <SilikegoCore/Api.h>
#include <SilikegoCore/Value.h>

namespace Silikego
{
	enum class NodeStatus
	{
		Nothing,
		Leaf,
		Branch
	};

	class SILIKEGOCORE_EXPORT Node
	{
		class Impl;
		Impl* _impl;
	public:
		Node();
		Node(Error source);
		Node(int source);
		Node(long long int source);
		Node(double source);
		Node(const Value& source);
		Node(const std::string& source);
		Node(const Node& source);
		Node(Node&& source);
		~Node();

		Node& operator=(Error source);
		Node& operator=(int source);
		Node& operator=(long long int source);
		Node& operator=(double source);
		Node& operator=(const Value& source);
		Node& operator=(const std::string& source);
		Node& operator=(const Node& source);
		Node& operator=(Node&& source);

		inline bool isEmpty() const
		{
			return _impl == nullptr;
		};

		size_t countChildren() const;
		const Node* fetchChild(size_t child_index) const;
		const std::string* id() const;
		bool insert(size_t position, const Node& new_child);
		bool insert(size_t position, Node&& new_child);
		bool isNegated() const;
		void negate();
		std::optional<Node> pruneChild(size_t child_index);
		bool pushLeft(const Node& new_child);
		bool pushLeft(Node&& new_child);
		bool pushRight(const Node& new_child);
		bool pushRight(Node&& new_child);
		NodeStatus status() const;
		const Value* value() const;
	};
};

#endif // SILIKEGO_CORE_NODE_H
