/* TestFunctions.cc: Program to test function-calling code.
 * Copyright 2012, 2014, 2015, 2016 Vincent Damewood
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <iostream>

#include "FunctionCaller.hh"

int main()
{
	std::vector<Silikego::Value> Inputs; // Arguments to functions.
	Silikego::Value *InputValue = 0; // Used to add values to Inputs.
	Silikego::Value Result; // Return value of function.
	int Total = 0;    // Total number of tests run.
	int Success = 0;  // Number of successful tests.

	Silikego::FunctionCaller::SetUp();

	////////////////////////////////////////////////////////////////////

	std::cout << "Test 1: abs(-400) returns 400... ";

	//InputValue = new Silikego::Value(400);
	Inputs.push_back(Silikego::Value(400));
	//Inputs.push_back(*InputValue);
	//delete InputValue;
	Total++;
	Result = Silikego::FunctionCaller::Call("abs", Inputs);
	if (Result.Status() == Silikego::Value::INTEGER)
	{
		if (Result.Integer() == 400)
		{
			std::cout << "success." << std::endl;
			Success++;
		}
		else
		{
			std::cout << "failed. Value returned: "
				<< Result.Integer() << std::endl;
		}
	}
	else if (Result.Status() == Silikego::Value::FLOAT)
	{
		std::cout << "failed. Value returned: "
			<< Result.Float() << std::endl;
	}
	else
	{
		std::cout << "failed. Error code returned: "
			<< Result.Status() << std::endl;
	}

	////////////////////////////////////////////////////////////////////

	Silikego::FunctionCaller::TearDown();

	// Summary
	std::cout << "Tests passed/run: " << Success << "/" << Total << std::endl
		<< "Percentage: " << (((float) Success / (float) Total) * 100.00)
		<< "%" << std::endl;
	return Total - Success;
}
