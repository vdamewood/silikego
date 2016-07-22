/* FunctionCaller.cc: Function handling
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

#include <unordered_map>

#include "FunctionCaller.h"
#include "Functions.h"

// This is initialized as a null pointer rather than a static object so that
// bindings can be created for language interpreters written in C.
std::unordered_map<std::string, Silikego::FunctionCaller::FunctionPointer> *lookup = 0;

bool Silikego::FunctionCaller::SetUp()
{
	try
	{
		lookup = new std::unordered_map<std::string,
			Silikego::FunctionCaller::FunctionPointer>();
	}
	catch (...)
	{
		lookup = 0;
		return false;
	}

	// Built-in operators
	Install("add", Silikego::Functions::add);
	Install("subtract", Silikego::Functions::subtract);
	Install("multiply", Silikego::Functions::multiply);
	Install("divide", Silikego::Functions::divide);
	Install("power", Silikego::Functions::power);
	Install("dice", Silikego::Functions::dice);

	// Math library functions
	Install("abs", Silikego::Functions::abs);
	Install("acos", Silikego::Functions::acos);
	Install("asin", Silikego::Functions::asin);
	Install("atan", Silikego::Functions::atan);
	Install("ceil", Silikego::Functions::ceil);
	Install("cos", Silikego::Functions::cos);
	Install("cosh", Silikego::Functions::cosh);
	Install("exp", Silikego::Functions::exp);
	Install("floor", Silikego::Functions::floor);
	Install("log", Silikego::Functions::log);
	Install("log10", Silikego::Functions::log10);
	Install("sin", Silikego::Functions::sin);
	Install("sinh", Silikego::Functions::sinh);
	Install("sqrt", Silikego::Functions::sqrt);
	Install("tan", Silikego::Functions::tan);
	Install("tanh", Silikego::Functions::tanh);

	return true;
}

void Silikego::FunctionCaller::TearDown()
{
	delete lookup;
	lookup = 0;
}

void Silikego::FunctionCaller::Install(const std::string &Name, FunctionPointer Function)
{
	(*lookup)[Name] = Function;
}

Silikego::Value Silikego::FunctionCaller::Call(const std::string &Name, std::vector<Silikego::Value> Args)
try
{
	return lookup->at(Name)(Args);
}
catch (const std::out_of_range &)
{
	return Silikego::Value::BAD_FUNCTION;
}
