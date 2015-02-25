/*
 * EvalWindow.cpp: Expression evaluation window
 * Copyright 2012, 2014, 2015 Vincent Damewood
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

#include <XaviPP/FunctionCaller.hpp>
#include <XaviPP/InfixParser.hpp>

#include "QStringSource.hpp"
#include "EvalWindow.hpp"

EvalWindow::EvalWindow(QWidget * parent)
	: QMainWindow(parent)
{
	Xavi::FunctionCaller::SetUp();
	setupUi(this);
	output->setNum(0);
	connect(calculateButton, SIGNAL(clicked(bool)),
		this, SLOT(calculate()));
}

EvalWindow::~EvalWindow()
{
	Xavi::FunctionCaller::TearDown();
}

void EvalWindow::calculate()
{
	Xavi::SyntaxTreeNode *MyTree = Xavi::ParseInfix(
		new QStringSource(input->text()));

	Xavi::Value result = MyTree->Evaluate();
	switch (result.Status())
	{
		case Xavi::Value::INTEGER:
			output->setNum(result.Integer());
			break;
		case Xavi::Value::FLOAT:
			output->setNum(result.Float());
			break;
		case Xavi::Value::MEMORY_ERR:
			output->setText("Out of memory.\n");
			break;
		case Xavi::Value::SYNTAX_ERR:
			output->setText("Syntax error.\n");
			break;
		case Xavi::Value::ZERO_DIV_ERR:
			output->setText("Division by zero error.\n");
			break;
		case Xavi::Value::BAD_FUNCTION:
			output->setText("Function not found.\n");
			break;
		case Xavi::Value::BAD_ARGUMENTS:
			output->setText("Bad argument count.\n");
			break;
		case Xavi::Value::DOMAIN_ERR:
			output->setText("Domain error.\n");
			break;
		case Xavi::Value::RANGE_ERR:
			output->setText("Range error.\n");
			break;
	}
}