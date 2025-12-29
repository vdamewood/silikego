/* Evaluate.cc: Functions to evaluate syntax trees with an engine
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

#include <SilikegoCore/Evaluate.h>

namespace Silikego
{
 	Value Evaluate(const Node& node, FunctionCaller& engine)
	{
		switch (node.status())
		{
		case NodeStatus::Nothing:
			return Error::Syntax;
		case NodeStatus::Leaf:
            return node.value();
		case NodeStatus::Branch:
		{
			std::vector<Value> arguments;

			for (int i = 0; i < node.countChildren(); i++)
			{
				Value current = Evaluate(*node.fetchChild(i), engine);
				if (current.status() == ValueStatus::Error)
					return current;
				arguments.push_back(current);
			}

			Value result{engine.call(node.id(), arguments)};
			if (node.isNegated())
				result.negate();

			return result;
		}
		default:
			return Error::Syntax;
		}
	}
};
