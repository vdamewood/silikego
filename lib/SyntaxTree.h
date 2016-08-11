/* SyntaxTree.h: Abstract syntax tree classes
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

#if !defined SILIKEGO_SYNTAX_TREE_H
#define SILIKEGO_SYNTAX_TREE_H

#include "W32Dll.h"
#include "Value.h"

namespace Silikego
{
	class SILIKEGO_API SyntaxTreeNode
	{
	public:
		virtual ~SyntaxTreeNode();
		virtual Silikego::Value Evaluate() = 0;
		virtual void Negate() = 0;
	};

	class SILIKEGO_API IntegerNode : public SyntaxTreeNode
	{
	public:
		IntegerNode(int NewValue);
		IntegerNode(const IntegerNode&);
		virtual ~IntegerNode();

		const IntegerNode& operator=(const IntegerNode&);

		virtual Silikego::Value Evaluate();
		virtual void Negate();
	private:
		class State;
		State *S;
	};

	class SILIKEGO_API FloatNode : public SyntaxTreeNode
	{
	public:
		FloatNode(double NewValue);
		FloatNode(const FloatNode&);
		virtual ~FloatNode();

		const FloatNode& operator=(const FloatNode&);

		virtual Silikego::Value Evaluate();
		virtual void Negate();
	private:
		class State;
		State *S;
	};


	class SILIKEGO_API BranchNode : public SyntaxTreeNode
	{
	public:
		BranchNode(const char *NewFunctionId);
		virtual ~BranchNode();

		virtual Silikego::Value Evaluate();
		virtual void Negate();

		void PushLeft(SyntaxTreeNode *);
		void PushRight(SyntaxTreeNode *);
		bool GraftLeft(SyntaxTreeNode *);
		bool GraftRight(SyntaxTreeNode *);
	private:
		BranchNode(const BranchNode&);
		const BranchNode& operator=(const BranchNode&);
		class State;
		State *S;
	};

	class SILIKEGO_API SyntaxErrorNode : public SyntaxTreeNode
	{
	public:
		virtual ~SyntaxErrorNode();
		virtual Silikego::Value Evaluate();
		virtual void Negate();
	};

	class SILIKEGO_API NothingNode : public SyntaxTreeNode
	{
	public:
		virtual ~NothingNode();
		virtual Silikego::Value Evaluate();
		virtual void Negate();
	};
};

#endif // SILIKEGO_SYNTAX_TREE_H
