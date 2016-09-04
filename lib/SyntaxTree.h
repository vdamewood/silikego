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

#include <memory>

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
		virtual bool IsError() = 0;
	};

	class SILIKEGO_API IntegerNode : public SyntaxTreeNode
	{
	public:
		IntegerNode(short int NewValue);
		IntegerNode(int NewValue);
		IntegerNode(long int NewValue);
		IntegerNode(long long int NewValue);
		IntegerNode(const IntegerNode&);
		virtual ~IntegerNode();

		const IntegerNode& operator=(const IntegerNode&);

		virtual Silikego::Value Evaluate();
		virtual void Negate();
		virtual bool IsError();
	private:
		class State;
		State *S;
	};

	class SILIKEGO_API FloatNode : public SyntaxTreeNode
	{
	public:
		FloatNode(float NewValue);
		FloatNode(double NewValue);
		FloatNode(const FloatNode&);
		virtual ~FloatNode();

		const FloatNode& operator=(const FloatNode&);

		virtual Silikego::Value Evaluate();
		virtual void Negate();
		virtual bool IsError();
	private:
		class State;
		State *S;
	};


	class SILIKEGO_API BranchNode : public SyntaxTreeNode
	{
	public:
		BranchNode(const std::string& NewFunctionId);
		virtual ~BranchNode();

		virtual Silikego::Value Evaluate();
		virtual void Negate();
		virtual bool IsError();

		void PushLeft(std::unique_ptr<SyntaxTreeNode>);
		void PushRight(std::unique_ptr<SyntaxTreeNode>);
		bool GraftLeft(std::unique_ptr<SyntaxTreeNode>);
		bool GraftRight(std::unique_ptr<SyntaxTreeNode>);
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
		virtual bool IsError();
	};
};

#endif // SILIKEGO_SYNTAX_TREE_H
