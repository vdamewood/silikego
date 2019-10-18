/* EvalWindow.cc: Expression evaluation window
 * Copyright 2015, 2016, 2018 Vincent Damewood
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

#include <gtkmm.h>

#include <Silikego/StringSource.h>
#include <Silikego/InfixParser.h>
#include <Silikego/Value.h>

#include "EvalWindow.h"

void EvalWindow::Calculate()
{
	std::unique_ptr<Silikego::SyntaxTreeNode> ResultTree =
		Silikego::ParseInfix(
		std::unique_ptr<Silikego::DataSource>(new Silikego::StringSource(
			MyInput->get_text().c_str())));
	Silikego::Value Value = ResultTree->Evaluate();

	std::string ResultString = Value.ToString();
	MyOutput->set_text(ResultString.c_str());
}

EvalWindow::EvalWindow()
	:MyBuilder(0),
	MyWindow(0),
	MyButton(0),
	MyInput(0),
	MyOutput(0)
{
	MyBuilder = Gtk::Builder::create_from_resource("/com/vdamewood/SilikegujoForUnix/EvalWindow.glade");
	MyBuilder->get_widget("EvalWindow", MyWindow);
	MyBuilder->get_widget("CalculateButton", MyButton);
	MyBuilder->get_widget("Input", MyInput);
	MyBuilder->get_widget("Output", MyOutput);

	MyButton->signal_clicked().connect(
		sigc::mem_fun(
		*this,
		&EvalWindow::Calculate));
	MyInput->signal_activate().connect(
		sigc::mem_fun(
		*this,
		&EvalWindow::Calculate));
	MyWindow->show();
}
