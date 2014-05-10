/*
 * XaviTree.c: Functions to manipulate abstract syntax trees.
 * Copyright 2012, 2014 Vincent Damewood
 *
 * This file is part of Xavi.
 *
 * Xavi is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation, either version 3 of
 * the License, or (at your option) any later version.
 *
 * Xavi is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with Xavi. If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdlib.h>

#include "XaviTree.h"
#include "XaviFunctionId.h"
#include "XaviFunctionCall.h"

int XaviTreeGraftLeft(XaviTree * parent, XaviTree * left)
{
	if (parent->type = 'o')
	{
		if (parent->op->left == NULL)
		{
			parent->op->left = left;
			return -1;
		}
		else
		{
			return XaviTreeGraftLeft(parent->op->left, left);
		}
	}
	else
	{
		return 0;
	}
	
}

int XaviTreeNegate(XaviTree * tree)
{
	if (tree == NULL || tree->type != 'n')
		return 0;
	else if (tree->num->status == S_INTEGER)
		tree->num->i *= -1;
	else if (tree->num->status == S_FLOAT)
		tree->num->f *= -1.0;
	else
		return 0;
	return 1;
}

XaviTree * XaviTreeNewOperator(XaviOperatorSymbol symbol, XaviTree * left, XaviTree * right)
{
	XaviTree * rVal;
	XaviOperator * rValOp;
	XaviNumber * rValArgs;

	rVal = malloc(sizeof(XaviTree)); //if
	rValOp = malloc(sizeof(XaviOperator)); //if
	rValArgs = malloc(sizeof(XaviNumber) * 2); //if

	if (!rVal || !rValOp || !rValArgs) {
		free(rVal);
		free(rValOp);
		free(rValArgs);
		return NULL;
	}
	
	rVal->op = rValOp;
	rVal->op->args = rValArgs;
	
	rVal->type = 'o';
	rVal->op->symbol = symbol;

	rVal->op->left = left;

	rVal->op->right = right;
	return rVal;
}

XaviTree * XaviTreeNewInteger(int value)
{
	XaviTree * rVal;
	XaviNumber * rValNum;

	rVal = malloc(sizeof(XaviTree)); // FIXME: if
	rValNum = malloc(sizeof(XaviNumber)); // FIXME: if

	if (!rVal || !rValNum) {
		free(rVal);
		free(rValNum);
		return NULL;
	}
	rVal->num = rValNum;

	rVal->type = 'n';
	rVal->num->status = S_INTEGER;
	rVal->num->i = value;
	return rVal;
}

XaviTree * XaviTreeNewFloat(float value)
{
	XaviTree * rVal;
	XaviNumber * rValNum;

	rVal = malloc(sizeof(XaviTree)); // FIXME: if
	rValNum = malloc(sizeof(XaviNumber)); // FIXME: if

	if (!rVal || !rValNum) {
		free(rVal);
		free(rValNum);
		return NULL;
	}
	rVal->num = rValNum;

	rVal->type = 'n';
	rVal->num->status = S_FLOAT;
	rVal->num->f = value;
	return rVal;
}

XaviTree * XaviTreeNewFunction(char * name, int argc, XaviTree ** argv)
{
	XaviTree * rVal;
	XaviFunction * rValFunc;

	rVal = malloc(sizeof(XaviTree)); // FIXME: if
	rValFunc = malloc(sizeof(XaviFunction)); // FIXME: if

	if (!rVal || !rValFunc) {
		free(rVal);
		free(rValFunc);
		return NULL;
	}
	rVal->func = rValFunc;

	rVal->type = 'f';
	rVal->func->name = name;
	rVal->func->arg_count = argc;
	rVal->func->arg_vector = argv;
	
	return rVal;
}

XaviTree * XaviTreeNewSyntaxError(void)
{
	XaviTree * rVal;
	
	rVal = malloc(sizeof(XaviTree *));
	rVal->type = 'e';
	return rVal;
}

void XaviTreeDelete(XaviTree * tree)
{
	int i;
	if (tree) {
		switch (tree->type) {
			case 'n':
				free(tree->num);
				break;
			case 'o':
				XaviTreeDelete(tree->op->left);
				XaviTreeDelete(tree->op->right);
				free(tree->op->args);
				free(tree->op);
				break;
			case 'f':
				XaviFunctionIdDelete(tree->func->name);
				for (i = 0; i <  tree->func->arg_count; i++) {
					XaviTreeDelete(tree->func->arg_vector[i]);
				}
				free(tree->func->arg_vector);
				free(tree->func);
				break;
		}
		free(tree);
	}
}

// Fomerally XaviTreeEvaluate

static int IsNumber(XaviNumber n) {
	return n.status == S_INTEGER || n.status == S_FLOAT;
}

static XaviNumber EvaluateOperator(XaviOperator * op)
{
	XaviNumber opLeft;
	XaviNumber opRight;
	
	opLeft = XaviTreeEvaluate(op->left);
	if (!IsNumber(opLeft)) return opLeft;
	
	opRight = XaviTreeEvaluate(op->right);
	if (!IsNumber(opRight)) return opRight;
	
	return XaviOperatorCall(op->symbol, opLeft, opRight);
}

XaviNumber EvaluateFunction(XaviFunction * func)
{
	XaviNumber rVal;
	XaviNumber * arguments = NULL;
	int i;
	
	if (func->arg_count) {
		if (!(arguments = malloc(sizeof(XaviNumber) * func->arg_count))) {
			rVal.status = E_MEMORY;
			return rVal;
		}
		
		for(i = 0; i < func->arg_count; i++) {
			arguments[i] = XaviTreeEvaluate(func->arg_vector[i]);
			if(!IsNumber(arguments[i])) {
				rVal = arguments[i];
				free(arguments);
				return rVal;
			}
		}
	}
	
	rVal = XaviFunctionCall(func->name, func->arg_count, arguments);
	free(arguments);
	return rVal;
}

XaviNumber XaviTreeEvaluate(XaviTree * tree)
{
	XaviNumber rVal;
	
	if (!tree || tree->type == 'e') {
		rVal.status = E_SYNTAX;
		return rVal;
	}
	
	switch (tree->type) {
		case 'n': return *tree->num;
		case 'o': return EvaluateOperator(tree->op);
		case 'f': return EvaluateFunction(tree->func);
	}
	rVal.status = E_INTERNAL;
	return rVal;
}
