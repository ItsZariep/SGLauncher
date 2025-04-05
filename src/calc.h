#ifndef CALC_H
#define CALC_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <ctype.h>

#define MAX_LEN 100

typedef struct
{
	double data[MAX_LEN];
	int top;
} Stack;

void push(Stack *s, double val);
double pop(Stack *s);
int precedence(char op);
double evaluateFunction(char func, double operand);
double evaluatePostfix(char postfix[][MAX_LEN], int count);
void infixToPostfix(const char *infix, char postfix[][MAX_LEN], int *count);
double evaluate(const char* expression);

#endif
