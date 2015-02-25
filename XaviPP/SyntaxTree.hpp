/*
 * SyntaxTree.hpp: Abstract syntax tree classes
 * Copyright 2012, 2014, 2015 Vincent Damewood
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

#if !defined XAVI_SYNTAX_TREE_HPP
#define XAVI_SYNTAX_TREE_HPP

#include <list>

#include "W32Dll.hpp"
#include "Value.hpp"

// Windows requires this trickery.
namespace Xavi { class SyntaxTreeNode; };
XAVIPP_EXTERN template class XAVIPP_API std::list<Xavi::SyntaxTreeNode *>;

namespace Xavi
{
	class XAVIPP_API SyntaxTreeNode
	{
	public:
		virtual ~SyntaxTreeNode(void) { };
		virtual Xavi::Value Evaluate(void) = 0;
		virtual void Negate(void) = 0;
	};

	class IntegerNode : public SyntaxTreeNode
	{
	public:
		IntegerNode(int NewValue);
		virtual Xavi::Value Evaluate(void);
		virtual void Negate(void);
	private:
		int MyInteger;
	};

	class XAVIPP_API FloatNode : public SyntaxTreeNode
	{
	public:
		FloatNode(float NewValue);
		virtual Xavi::Value Evaluate(void);
		virtual void Negate(void);
	private:
		float MyFloat;
	};

	class XAVIPP_API BranchNode : public SyntaxTreeNode
	{
	public:
		BranchNode(const char *NewFunctionId);
		virtual ~BranchNode();
		virtual Xavi::Value Evaluate(void);
		virtual void Negate(void);

		void PushLeft(SyntaxTreeNode *);
		void PushRight(SyntaxTreeNode *);
		bool GraftLeft(SyntaxTreeNode *);
		bool GraftRight(SyntaxTreeNode *);
	private:
		bool IsNegated;
		char *MyId;
		std::list<SyntaxTreeNode *> MyChildren;
	};

	class XAVIPP_API SyntaxErrorNode : public SyntaxTreeNode
	{
	public:
		virtual ~SyntaxErrorNode(void);
		virtual Xavi::Value Evaluate(void);
		virtual void Negate(void);
	};

	class XAVIPP_API NothingNode : public SyntaxTreeNode
	{
	public:
		virtual ~NothingNode(void);
		virtual Xavi::Value Evaluate(void);
		virtual void Negate(void);
	};
};

#endif // XAVI_SYNTAX_TREE_HPP