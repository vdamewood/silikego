/* SyntaxTree.h: Abstract syntax tree classes
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

#if !defined SILIKEGO_CORE_SYNTAX_TREE_H
#define SILIKEGO_CORE_SYNTAX_TREE_H

#include <optional>
#include <string>

#include <SilikegoCore/Api.h>
#include <SilikegoCore/FunctionCaller.h>
#include <SilikegoCore/Value.h>

namespace Silikego
{
	enum class NodeStatus
	{
		Nothing,
		Leaf,
		Branch
	};

	class SILIKEGOCORE_EXPORT SyntaxTreeNode
	{
	public:
		SyntaxTreeNode();

		SyntaxTreeNode(Error);
		SyntaxTreeNode(long long int);
		SyntaxTreeNode(double);
		SyntaxTreeNode(Value);
		SyntaxTreeNode(const std::string&);

		SyntaxTreeNode(const SyntaxTreeNode&);
		SyntaxTreeNode(SyntaxTreeNode&&);
		~SyntaxTreeNode();

		SyntaxTreeNode& operator=(const SyntaxTreeNode&);
		SyntaxTreeNode& operator=(SyntaxTreeNode&&);

		SyntaxTreeNode* fetchChild(int child_index);
		bool collapse(FunctionCaller&);
		Value evaluate(FunctionCaller&);
		NodeStatus status();
		bool isError();
		void negate();
		std::optional<SyntaxTreeNode> pruneChild(int child_index);
		bool pushLeft(SyntaxTreeNode&&);
		bool pushRight(SyntaxTreeNode&&);

	private:
		class Impl;
		Impl* _impl;
	};
};

#endif // SILIKEGO_CORE_SYNTAX_TREE_H
