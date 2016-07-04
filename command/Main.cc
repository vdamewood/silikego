/* Main.cc: Entry point for command-line interface
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

#include <cstdlib>
#include <cstring>

#if defined USE_UNIX
#	include <unistd.h>
#	define ISATTY() (isatty(fileno(stdin)) && isatty(fileno(stdout)))
#else
#	define ISATTY() (-1)
#endif

#include <iostream>

#include "FunctionCaller.hh"
#include "InfixParser.hh"
#include "StringSource.hh"

#if HAVE_READLINE
#include <readline/readline.h>
#else
extern "C" char *readline(const char *);
extern "C" void add_history(char *);
#endif /* HAVE_READLINE */

int main(int argc, char *argv[])
{
	const char *prompt;
	const char *response;

	if (ISATTY())
	{
		prompt = "> ";
		response = "= ";
	}
	else
	{
		prompt = "";
		response = "";
	}

	Silikego::FunctionCaller::SetUp();

	char *expression = NULL;
	char *old_expression = NULL;
	while(-1)
	{
		expression = readline(prompt);

		if(!expression)
		{
			std::free(static_cast<void*>(old_expression));
			old_expression = NULL;
			break;
		}

		if(*expression && (!old_expression || std::strcmp(expression, old_expression) != 0))
			add_history(expression);

		std::free(static_cast<void*>(old_expression));
		old_expression = NULL;

		Silikego::SyntaxTreeNode *Tree
			= Silikego::ParseInfix(new StringSource(expression));

		Silikego::Value result = Tree->Evaluate();
		delete Tree;
		Tree = 0;

		char *ResultString = result.ToCString();
		std::cout << ResultString << std::endl;
		delete[] ResultString;
		old_expression = expression;
	}

	Silikego::FunctionCaller::TearDown();

	if (ISATTY())
	{
		fputc('\n', stdout);
		fflush(stdout);
	}
	return 0;
}
