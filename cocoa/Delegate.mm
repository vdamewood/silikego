/* Delegate.mm: Delegate for Cocoa
 * Copyright 2014, 2015, 2016 Vincent Damewood
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

#import "Delegate.h"

#include "InfixParser.h"
#include "SyntaxTree.h"
#include "FunctionCaller.h"
#include "CStringSource.h"

#import <Cocoa/Cocoa.h>

@implementation SilikegoDelegate


- (void)applicationDidFinishLaunching:(NSNotification *)aNotification
{
	Silikego::FunctionCaller::SetUp();
}

- (BOOL)applicationShouldTerminateAfterLastWindowClosed:(NSApplication *)theApplication
{
	return YES;
}

- (void)applicationWillTerminate:(NSNotification *)aNotification
{
	Silikego::FunctionCaller::TearDown();
}

- (IBAction) Calculate:(id)sender
{
	Silikego::SyntaxTreeNode *Ast;
	Silikego::Value Result;

	Ast = Silikego::ParseInfix(
		new Silikego::CStringSource(
			[[self.input stringValue] UTF8String]));
	Result = Ast->Evaluate();
	delete Ast;

	char *ResultCString = Result.ToCString();
	NSString *ResultString = [[NSString alloc] initWithUTF8String: ResultCString];
	delete[] ResultCString;
	[self.output setStringValue: ResultString];
}
@end
