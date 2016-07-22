/* SyntaxTree.hh: Abstract syntax tree classes
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

#if !defined SILIKEGO_SYNTAX_TREE_HH
#define SILIKEGO_SYNTAX_TREE_HH

#include "W32Dll.hh"
#include "Value.hh"

namespace Silikego
{
	class SILIKEGO_API SyntaxTreeNode
	{
	public:
		virtual ~SyntaxTreeNode() { };
		virtual Silikego::Value Evaluate() = 0;
		virtual void Negate() = 0;
	};

	class IntegerNode : public SyntaxTreeNode
	{
	public:
		IntegerNode(int NewValue);
		virtual Silikego::Value Evaluate();
		virtual void Negate();
	private:
		int MyInteger;
	};

	class SILIKEGO_API FloatNode : public SyntaxTreeNode
	{
	public:
		FloatNode(float NewValue);
		virtual Silikego::Value Evaluate();
		virtual void Negate();
	private:
		float MyFloat;
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
		class State;
		State* MyState;
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

#endif // SILIKEGO_SYNTAX_TREE_HH
