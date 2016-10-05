/* App.c: App instance using MFC Framework
 * Copyright 2015, 2016 Vincent Damewood
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

#include <afxwin.h>

#include "FunctionCaller.h"
#include "EvalWindow.h"

class SilikegoApp : public CWinApp
{
public:
	BOOL InitInstance();
	int ExitInstance();
};

BOOL SilikegoApp::InitInstance()
{
	CWinApp::InitInstance();

	Silikego::FunctionCaller::SetUp();

	if (!(m_pMainWnd = new EvalWindow))
		return FALSE;

	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();
	return TRUE;
}

int SilikegoApp::ExitInstance()
{
	Silikego::FunctionCaller::TearDown();

	return CWinApp::ExitInstance();
}

SilikegoApp theApp;
