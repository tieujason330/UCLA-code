#include <iostream>
#include <string>
#include <stack>
#include <cassert>
#include <cctype>
using namespace std;

//Additional member functions 
bool isOperator(char Operator); //check if is operator
int precedence(char Operator); //check for precedence
bool operation(bool operand1, char Operator, bool operand2 = 0); //operation on operand1 and operand 2
bool isValid(stack<bool> test); //check valid operand stack


int evaluate(string infix, const bool values[], string& postfix, bool& result)
{
	bool valid = true;
	//converts infix to prefix
	string pfix = "";
	stack<char> operatorStack;
	for (int i = 0; infix[i] != NULL; i++)
	{
		if (infix[i] != ' ')
		{
			switch (infix[i])
			{
			case '(':
				operatorStack.push(infix[i]);
				break;
			case ')':
				while (operatorStack.top() != '(')
				{
					pfix += operatorStack.top();
					operatorStack.pop();
				}
				operatorStack.pop();
				break;
			case '|': case '&': case '!':
				while (!operatorStack.empty() && (operatorStack.top() != '(')
					&& (precedence(infix[i]) <= precedence(operatorStack.top())))
				{
					pfix += operatorStack.top();
					operatorStack.pop();
				}
				//checks for cases in infix where an operand is preceded by '('
				//this means there are not two arguments for infix operation
				//excludes '!'
				if (infix[i] != '!' && infix[i - 1] == '(')
					valid = false;
				operatorStack.push(infix[i]);
				break;
			default:
				pfix += infix[i];
				break;
			}
		}
	}
	while (!operatorStack.empty())
	{
		//checks for unequal amounts of '(' and ')'
		if (operatorStack.top() == '(')
			valid = false;
		pfix += operatorStack.top();
		operatorStack.pop();
	}
	postfix = pfix;
	cout << "infix = " << infix << endl;
	cout << "postfix = " << postfix << endl;

	//evalute the postfix expression
	stack<bool> operandStack;
	for (int i = 0; postfix[i] != NULL; i++)
	{
		if (isdigit(postfix[i]))
		{
			//////////
			operandStack.push(values[postfix[i]]);
			cout << "postfix[0] = " << postfix[0] << endl; //postfix[0] = 4 in this case
			cout << "4 - values[postfix[4]]= " << values[postfix[4]] << endl;//equals to false
			cout << "i - values[postfix[" << i << "]] = " << values[postfix[i]] << endl;//equals to true
			//////////
		}
		if (isOperator(postfix[i]))
		{
			if (postfix[i] != '!' && operandStack.size() >= 2)
			{
				bool operand2 = operandStack.top();
				operandStack.pop();
				bool operand1 = operandStack.top();
				operandStack.pop();
				bool ans;
				if (postfix[i] == '&')
					ans = (operand1 && operand2);
				if (postfix[i] == '|')
					ans = (operand1 || operand2);
				/*
				bool ans = operation(values[operand1], postfix[i], values[operand2]);
				*/
				operandStack.push(ans);
			}
			else if (postfix[i] == '!')
			{
				bool operand1 = operandStack.top();
				operandStack.pop();

				bool ans = !operand1;
				//bool ans = operation(values[operand1], postfix[i]);
				operandStack.push(ans);
			}
			//checks operandStack size > 2 when
			//operator that's not ! about to be used
			else
				valid = false;
		}
	}
	if (isValid(operandStack) && valid == true)
	{
		result = operandStack.top();
		cout << "result = " << result << endl;
		return 0;
	}
	else return 1;
}


bool isOperator(char Operator)
{
	if (!isdigit(Operator) && Operator != ')')
		return true;
	else
		return false;
}

int precedence(char Operator)
{
	int value = -1;
	if (Operator == '!')
		value = 3;
	if (Operator == '&')
		value = 2;
	if (Operator == '|')
		value = 1;
	return value;
}

bool operation(bool operand1, char Operator, bool operand2)
{
	if (Operator == '!')
		return !operand1;
	if (Operator == '&')
		return (operand1 && operand2);
	if (Operator == '|')
		return (operand1 || operand2);
}

bool isValid(stack<bool> test)
{
	if (test.empty())
		return false;
	int pot_res;

	pot_res = test.top();
	test.pop();
	if (!test.empty())
		return false;
	return true;
}

int main()
{
	bool ba[10] = {
	//  0      1      2      3      4      5      6      7      8      9
		true, true, true, false, false, false, true, false, true, false
	};
	string pf;
	bool answer;
	/*
	cout << evaluate("(2| 3)", ba, pf, answer) << endl;
	cout << evaluate("8|", ba, pf, answer) << endl;
	cout << evaluate("4 5", ba, pf, answer) << endl;
	cout << evaluate("01", ba, pf, answer) << endl;
	cout << evaluate("()", ba, pf, answer) << endl;
	cout << evaluate("2(9|8)", ba, pf, answer) << endl;
	cout << evaluate("2(&8)", ba, pf, answer) << endl;
	cout << evaluate("2&(8)", ba, pf, answer) << endl;
	cout << evaluate("(6&(7|7)", ba, pf, answer) << endl;
	*/
	cout << evaluate("4  |  !3 & (0&3) ", ba, pf, answer) << endl;
	/*
	cout << evaluate("", ba, pf, answer) << endl;
	cout << evaluate(" 9  ", ba, pf, answer) << endl;
	cout << evaluate("((9))", ba, pf, answer) << endl;
	cout << evaluate("2| 3", ba, pf, answer) << endl;
	
	/*
	assert(evaluate("2| 3", ba, pf, answer) == 0 && pf == "23|" &&  answer);
	assert(evaluate("8|", ba, pf, answer) == 1);
	assert(evaluate("4 5", ba, pf, answer) == 1);
	assert(evaluate("01", ba, pf, answer) == 1);
	assert(evaluate("()", ba, pf, answer) == 1);
	assert(evaluate("2(9|8)", ba, pf, answer) == 1);
	assert(evaluate("2(&8)", ba, pf, answer) == 1);
	assert(evaluate("(6&(7|7)", ba, pf, answer) == 1);
	assert(evaluate("4  |  !3 & (0&3) ", ba, pf, answer) == 0
		&& pf == "43!03&&|"  &&  !answer);
	assert(evaluate("", ba, pf, answer) == 1);
	assert(evaluate(" 9  ", ba, pf, answer) == 0 && pf == "9"  &&  !answer);
	ba[2] = false;
	ba[9] = true;
	assert(evaluate("((9))", ba, pf, answer) == 0 && pf == "9"  &&  answer);
	assert(evaluate("2| 3", ba, pf, answer) == 0 && pf == "23|" &&  !answer);
	cout << "Passed all tests" << endl;
	*/
}


/*
//converts infix to postfix
postfix = "";
stack<char> operatorStack;
for (int i = 0; infix[i] != NULL; i++)
{
	if (infix[i] != ' ')
	{
		if (isdigit(infix[i]))
		{
			postfix += (infix[i]);
		}
		else
		{
			if (infix[i] == '(')
				operatorStack.push(infix[i]);
			if (infix[i] == ')')
			{
				while (operatorStack.top() != '(')
				{
					postfix += operatorStack.top();
					operatorStack.pop();
				}
				//pop the '('
				operatorStack.pop();
			}
			if (isOperator(infix[i]))
			{
				while (!operatorStack.empty() && operatorStack.top() != '('
					&& (precedence(infix[i]) <= precedence(operatorStack.top())))
				{
					postfix += operatorStack.top();
					operatorStack.pop();
				}
				operatorStack.push(infix[i]);
			}
		}
	}
}
while (!operatorStack.empty())
{
	postfix += operatorStack.top();
	operatorStack.pop();
}
*/
/*
//evaluation of postfix expression

stack<int> operandStack;
for (int i = 0; postfix[i] != NULL; i++)
{
if (isdigit(postfix[i]))
operandStack.push((int)postfix[i]);
else
{
if (postfix[i] != '!' && operandStack.size() >= 2)
{
int x1 = operandStack.top();
operandStack.pop();
int x2 = operandStack.top();
operandStack.pop();
int answer = operation(values[x1], postfix[i], values[x2]);
operandStack.push(answer);
}
if (postfix[i] == '!')
{
int x1 = operandStack.top();
operandStack.pop();
int answer = operation(values[x1], postfix[i]);
operandStack.push(answer);
}
else
return 1;
}
}

//if (operandStack.size() != 1)
//return 1;

result = operandStack.top();
return 0;
*/