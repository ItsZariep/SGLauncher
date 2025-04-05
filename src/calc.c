#include "calc.h"

void push(Stack *s, double val)
{
	if (s->top < MAX_LEN - 1) s->data[++s->top] = val;
}

double pop(Stack *s)
{
	return (s->top >= 0) ? s->data[s->top--] : 0;
}

int precedence(char op)
{
	if (op == '+' || op == '-') return 1;
	if (op == '*' || op == '/' || op == '%') return 2;
	if (op == '^' || op == 'n' || op == 'b') return 3;
	return 0;
}

double evaluateFunction(char func, double operand);

double evaluatePostfix(char postfix[][MAX_LEN], int count)
{
	Stack s = {.top = -1};

	for (int i = 0; i < count; i++)
	{
		if (isdigit(postfix[i][0]) || (postfix[i][0] == '-' && isdigit(postfix[i][1])))
		{
			push(&s, atof(postfix[i]));
		}
		else if (strlen(postfix[i]) == 1 && strchr("rlcstCSThH", postfix[i][0]))
		{
			if (s.top < 0) return NAN;  // Prevents segfault
			push(&s, evaluateFunction(postfix[i][0], pop(&s)));
		}
		else
		{
			if (s.top < 1) return NAN;  // Prevents pop() from empty stack
			double b = pop(&s), a = pop(&s);
			if (strcmp(postfix[i], "n") == 0) push(&s, pow(a, 1.0 / b));
			else if (strcmp(postfix[i], "b") == 0) push(&s, log(b) / log(a));
			else
			{
				switch (postfix[i][0])
				{
					case '+': push(&s, a + b); break;
					case '-': push(&s, a - b); break;
					case '*': push(&s, a * b); break;
					case '/': push(&s, (b != 0) ? a / b : NAN); break;
					case '^': push(&s, pow(a, b)); break;
					case '%': push(&s, (a / 100.0) * b); break;
					default: return NAN;
				}
			}
		}
	}
	return pop(&s);
}

void infixToPostfix(const char *infix, char postfix[][MAX_LEN], int *count)
{
	char stack[MAX_LEN];
	int top = -1, j = 0;

	for (int i = 0; infix[i] != '\0'; i++)
	{
		if (isdigit(infix[i]) || (infix[i] == '-' && (i == 0 || infix[i - 1] == '(' || strchr("+-*/^%nb", infix[i - 1]))))
		{
			int k = 0;
			postfix[j][k++] = infix[i++];
			while (isdigit(infix[i]) || infix[i] == '.')
			{
				postfix[j][k++] = infix[i++];
			}
			postfix[j][k] = '\0';
			j++; i--;
		} 
		else if (infix[i] == '(')
		{
			stack[++top] = '(';
		}
		else if (infix[i] == ')')
		{
			while (top >= 0 && stack[top] != '(')
			{
				sprintf(postfix[j++], "%c", stack[top--]);
			}
			top--;
		}
		else if (strchr("+-*/^%nb", infix[i]))
		{
			if (i > 0 && strchr("+-*/^%nb", infix[i - 1])) continue;

			while (top >= 0 && precedence(stack[top]) >= precedence(infix[i]))
			{
				sprintf(postfix[j++], "%c", stack[top--]);
			}
			stack[++top] = infix[i];
		}
	}

	while (top >= 0)
	{
		sprintf(postfix[j++], "%c", stack[top--]);
	}
	*count = j;
}

double evaluateFunction(char func, double operand)
{
	switch (func)
	{
		case 'r': return sqrt(operand);
		case 'l': return log(operand);
		case 'c': return cos(operand * M_PI / 180.0);
		case 's': return 1 / cos(operand * M_PI / 180.0);
		case 't': return tan(operand * M_PI / 180.0);
		case 'C': return acos(operand) * 180.0 / M_PI;
		case 'S': return asin(operand) * 180.0 / M_PI;
		case 'T': return atan(operand) * 180.0 / M_PI;
		case 'h': return cosh(operand);
		case 'H': return 1 / cosh(operand);
		default: return NAN;
	}
}

double evaluate(const char* expression)
{
	char postfix[MAX_LEN][MAX_LEN];
	int count = 0;

	infixToPostfix(expression, postfix, &count);
	return evaluatePostfix(postfix, count);
}
