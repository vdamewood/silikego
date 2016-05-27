/* Functions.cpp: Built-in functions
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

#include <cmath>
#include <cstdlib>
#include <ctime>
#include <limits>

#include "Functions.hpp"

Silikego::Value Silikego::Functions::add(std::vector<Silikego::Value> Args)
{
	if (!Args.size())
		return 0;

	Silikego::Value rVal = Args[0];
	for(int i = 1; i < Args.size(); i++)
		if (rVal.Status() == Silikego::Value::INTEGER)
			rVal = rVal.Integer() + Args[i].Integer();
		else
			rVal = rVal.Float() + Args[i].Float();

	return rVal;
}

Silikego::Value Silikego::Functions::subtract(std::vector<Silikego::Value> Args)
{
	if (!Args.size())
		return 0;

	Silikego::Value rVal = Args[0];
	for(int i = 1; i < Args.size(); i++)
		if (rVal.Status() == Silikego::Value::INTEGER)
			rVal = rVal.Integer() - Args[i].Integer();
		else
			rVal = rVal.Float() - Args[i].Float();

		return rVal;
}

Silikego::Value Silikego::Functions::multiply(std::vector<Silikego::Value> Args)
{
	if (!Args.size())
		return 0;

	Silikego::Value rVal = Args[0];
	for(int i = 1; i < Args.size(); i++)
		if (rVal.Status() == Silikego::Value::INTEGER)
			rVal = rVal.Integer() * Args[i].Integer();
		else
			rVal = rVal.Float() * Args[i].Float();

		return rVal;
}

Silikego::Value Silikego::Functions::divide(std::vector<Silikego::Value> Args)
{
	if (Args.size() < 2)
		return Silikego::Value::BAD_ARGUMENTS;

	Silikego::Value rVal = Args[0];

	for (int i = 1; i < Args.size(); i++)
	{
		/* Division-by-Zero Error */
		if ((Args[i].Status() == Silikego::Value::FLOAT && Args[i].Float() == 0.0)
			|| (Args[i].Status() == Silikego::Value::INTEGER && Args[i].Integer() == 0))
		{
			return Silikego::Value::ZERO_DIV_ERR;
		}

		if (rVal.Status() == Silikego::Value::FLOAT)
		{
			rVal = rVal.Float() / Args[i].Float();
		}
		else
		{
			if (Args[i].Status() == Silikego::Value::FLOAT
				|| rVal.Integer() % Args[i].Integer() != 0)
			{
				rVal = rVal.Float() / Args[i].Float();
			}
			else
			{
				rVal = rVal.Integer() / Args[i].Integer();
			}
		}
	}
	return rVal;
}

Silikego::Value Silikego::Functions::power(std::vector<Silikego::Value> Args)
{
	float runningValue;

	runningValue = Args[0].Float();

	for (std::vector<Silikego::Value>::iterator i = Args.begin(); i != Args.end(); i++)
		runningValue = std::pow(runningValue, i->Float());
	return runningValue;
}


Silikego::Value Silikego::Functions::dice(std::vector<Silikego::Value> Args)
{
	/* TODO: Make this function handle fractional dice. */
	static int hasSeeded = 0;

	if(Args.size() != 2)
		return Silikego::Value::BAD_ARGUMENTS;

	if (!hasSeeded)
	{
		hasSeeded = 1;
		std::srand((unsigned int)std::time(NULL));
	}

	int runningTotal = 0;
	for (int i = 1; i <= Args[0].Integer(); i++)
		runningTotal += (std::rand() % Args[1].Integer()) + 1;

	return runningTotal;
}

Silikego::Value Silikego::Functions::abs(std::vector<Silikego::Value> Args)
{
	if (Args.size() != 1)
		return Silikego::Value::BAD_ARGUMENTS;

	switch (Args[0].Status())
	{
	case Silikego::Value::FLOAT:
		return std::abs(Args[0].Float());
	case Silikego::Value::INTEGER:
		return std::abs(Args[0].Integer());
	default:
		return Args[0];
	}
}

Silikego::Value Silikego::Functions::acos(std::vector<Silikego::Value> Args)
{
	if (Args.size() != 1)
		return Silikego::Value::BAD_ARGUMENTS;

	if (Args[0].Float() < -1 || Args[0].Float() > 1)
		return Silikego::Value::DOMAIN_ERR;

	return std::acos(Args[0].Float());
}

Silikego::Value Silikego::Functions::asin(std::vector<Silikego::Value> Args)
{
	if (Args.size() != 1)
		return Silikego::Value::BAD_ARGUMENTS;

	if (Args[0].Float() < -1 || Args[0].Float() > 1)
		return Silikego::Value::DOMAIN_ERR;

	return std::asin(Args[0].Float());
}

Silikego::Value Silikego::Functions::atan(std::vector<Silikego::Value> Args)
{
	if (Args.size() != 1)
		return Silikego::Value::BAD_ARGUMENTS;

	return std::atan(Args[0].Float());
}

Silikego::Value Silikego::Functions::ceil(std::vector<Silikego::Value> Args)
{
	float result;

	if (Args.size() != 1)
		return Silikego::Value::BAD_ARGUMENTS;

	result = std::ceil(Args[0].Float());

	if (result <= std::numeric_limits<int>::max() && result >= std::numeric_limits<int>::min())
		return static_cast<int>(result);
	else
		return result;
}

Silikego::Value Silikego::Functions::cos(std::vector<Silikego::Value> Args)
{
	if (Args.size() != 1)
		return Silikego::Value::BAD_ARGUMENTS;

	return std::cos(Args[0].Float());
}

Silikego::Value Silikego::Functions::cosh(std::vector<Silikego::Value> Args)
{
	if (Args.size() != 1)
		return Silikego::Value::BAD_ARGUMENTS;

	return std::cosh(Args[0].Float());
}

Silikego::Value Silikego::Functions::exp(std::vector<Silikego::Value> Args)
{
	if (Args.size() != 1)
		return Silikego::Value::BAD_ARGUMENTS;

	return std::exp(Args[0].Float());
}

Silikego::Value Silikego::Functions::floor(std::vector<Silikego::Value> Args)
{
	float result;

	if (Args.size() != 1)
		return Silikego::Value::BAD_ARGUMENTS;

	result = std::floor(Args[0].Float());

	if (result <= std::numeric_limits<int>::max() && result >= std::numeric_limits<int>::min())
		return static_cast<int>(result);
	else
		return result;
}

Silikego::Value Silikego::Functions::log(std::vector<Silikego::Value> Args)
{
	if (Args.size() != 1)
		return Silikego::Value::BAD_ARGUMENTS;

	return std::log(Args[0].Float());
}

Silikego::Value Silikego::Functions::log10(std::vector<Silikego::Value> Args)
{
	if (Args.size() != 1)
		return Silikego::Value::BAD_ARGUMENTS;

	return std::log10(Args[0].Float());
}

Silikego::Value Silikego::Functions::sin(std::vector<Silikego::Value> Args)
{
	if (Args.size() != 1)
		return Silikego::Value::BAD_ARGUMENTS;

	return std::sin(Args[0].Float());
}

Silikego::Value Silikego::Functions::sinh(std::vector<Silikego::Value> Args)
{
	if (Args.size() != 1)
		return Silikego::Value::BAD_ARGUMENTS;

	return std::sinh(Args[0].Float());
}

Silikego::Value Silikego::Functions::sqrt(std::vector<Silikego::Value> Args)
{
	if (Args.size() != 1)
		return Silikego::Value::BAD_ARGUMENTS;

	if (Args[0].Float() < 0.0)
		return Silikego::Value::DOMAIN_ERR;

	return std::sqrt(Args[0].Float());
}

Silikego::Value Silikego::Functions::tan(std::vector<Silikego::Value> Args)
{
	// For some reason the C version of tan() doesn't throw an error for
	// input of pi/2 or 3*pi/2. Probably due to the imprecision of
	// floating point numbers.
	if (Args.size() != 1)
		return Silikego::Value::BAD_ARGUMENTS;

	return std::tan(Args[0].Float());
}

Silikego::Value Silikego::Functions::tanh(std::vector<Silikego::Value> Args)
{
	if (Args.size() != 1)
		return Silikego::Value::BAD_ARGUMENTS;

	return std::tanh(Args[0].Float());
}

