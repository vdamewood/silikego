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

#if !defined SILIKEGO_SYNTAX_TREE_H
#define SILIKEGO_SYNTAX_TREE_H

#include <SilikegoCore/Api.h>
#include <SilikegoCore/FunctionCaller.h>
#include <SilikegoCore/Value.h>

namespace Silikego
{
	class SILIKEGOCORE_EXPORT SyntaxTreeNode
	{
	public:
		SyntaxTreeNode();
		SyntaxTreeNode(long long int);
		SyntaxTreeNode(double);
		SyntaxTreeNode(ValueStatus);
		SyntaxTreeNode(Value);
		SyntaxTreeNode(const std::string&);

		SyntaxTreeNode(const SyntaxTreeNode&);
		SyntaxTreeNode(SyntaxTreeNode&&);
		SyntaxTreeNode& operator=(const SyntaxTreeNode&);
		SyntaxTreeNode& operator=(SyntaxTreeNode&&);
		~SyntaxTreeNode();

		Silikego::Value Evaluate(FunctionCaller&);
		void Negate();
		bool IsError();
		bool IsBranch();
		bool IsLeaf();
		bool IsNothing();
		void PushLeft(SyntaxTreeNode&&);
		void PushRight(SyntaxTreeNode&&);
	private:
		class Impl;
		Impl *I;
	};
};

#endif // SILIKEGO_SYNTAX_TREE_H
