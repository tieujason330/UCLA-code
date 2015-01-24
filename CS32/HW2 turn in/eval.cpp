#include <iostream>
#include <string>
#include <stack>
#include <cassert>
#include <cctype>
using namespace std;

//Additional member functions 
bool isOperator(char Operator); //check if argument is an operator
int precedence(char Operator); //check for precedence
//bool operation(bool operand1, char Operator, bool operand2 = 0); //operation on operand1 and operand 2
bool isValid(stack<bool> test); //check valid operand stack


int evaluate(string infix, const bool values[], string& postfix, bool& result)
{
	bool valid = true; //initially sets validity of infix syntax to true
	//converts infix to prefix
	string pfix = "";//make an empty postfix string
	stack<char> operatorStack;//initiate a operator stack
	int infix_size = infix.size();
	for (int i = 0; i < infix_size; i++)
	{
		if (infix[i] != ' ')//while char isn't an empty char
		{
			switch (infix[i])//switch statement for char
			{
			case '(':
				operatorStack.push(infix[i]);//push onto operator Stack if char is a '('
				break;
			case ')':
				while (operatorStack.top() != '(')//while top of operator stack isn't a '('
				{
					pfix += operatorStack.top();//append top of stack to postfix string
					operatorStack.pop();//pop stack
				}
				operatorStack.pop();//pop off '(' from stack
				break;
			case '|': case '&': case '!'://if is an operator
				while (!operatorStack.empty() && (operatorStack.top() != '(')
					&& (precedence(infix[i]) <= precedence(operatorStack.top())))
					//while operator stack isn't empty and top isn't '('
					//and precedence of char is less than or equal to precedence of top of stack
				{
					pfix += operatorStack.top();//append top of stack to postfix string
					operatorStack.pop();//pop top
				}
				//checks for cases in infix where an operand is preceded by '('
				//this means there are not two arguments for infix operation
				//excludes '!'
				if (infix[i] != '!' && infix[i - 1] == '(')
					valid = false;
				operatorStack.push(infix[i]);//out of while loop: push operator to operator stack
				break;
			default://for numbers
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
	//cout << "infix = " << infix << endl;
	//cout << "postfix = " << postfix << endl;

	//evalute the postfix expression
	stack<bool> operandStack;
	int postfix_size = postfix.size();
	for (int i = 0; i < postfix_size; i++)
	{
		if (isdigit(postfix[i]))
		{
			//setting z to equal the difference between the positions of the char and '0' in order to get an int value
			//for i = 0, it would basically be int z = '4' - '0';
			int z = postfix[i] - '0';
			operandStack.push(values[z]);
			//cout << "postfix[0] = " << postfix[0] << endl; //postfix[0] = 4 in this case
			//cout << "4 - values[postfix[4]]= " << values[postfix[4]] << endl;//equals to false
			//cout << "i - values[postfix[" << i << "]] = " << values[postfix[i]] << endl;//equals to true
		}
		//if postfix char is an operator
		if (isOperator(postfix[i]))
		{
			//check if operator isn't '!' and enough operands to do operation
			if (postfix[i] != '!' && operandStack.size() >= 2)
			{
				//set operand2 to top, and operand1 to next top
				//do evaluation according to which boolean evaluation the char represents
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
				//push answer onto operand stack
				operandStack.push(ans);
			}
			//if char is '!'
			else if (postfix[i] == '!')
			{
				//set operand1 to top of stack and negate it
				//push result back onto operand stack
				bool operand1 = operandStack.top();
				operandStack.pop();

				bool ans = !operand1;
				//bool ans = operation(values[operand1], postfix[i]);
				operandStack.push(ans);
			}
			//if operandStack size < 2 when
			//operator that's not ! about to be used
			else
				valid = false;
		}
	}
	//if the result is valid and syntax for infix is valid
	if (isValid(operandStack) && valid == true)
	{
		//set result equal to top of operand stack
		result = operandStack.top();
		//cout << "result = " << result << endl;
		return 0;
	}
	else return 1;
}

//this helper function checks if char is an operator
bool isOperator(char Operator)
{
	if (!isdigit(Operator) && Operator != ')')
		return true;
	else
		return false;
}

//this helper function assigns an int value depending
//on which operator it is for precedence comparison
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
/*
bool operation(bool operand1, char Operator, bool operand2)
{
	if (Operator == '!')
		return !operand1;
	if (Operator == '&')
		return (operand1 && operand2);
	if (Operator == '|')
		return (operand1 || operand2);
}
*/

//this helper function checks validity of the operand stack at the end
bool isValid(stack<bool> test)
{
	//if operand stack is empty, then return false
	if (test.empty())
		return false;

	//assign potential result to top of stack
	//pop the stack
	//if it isn't empty, return false since it should be the only element there
	int pot_res;
	pot_res = test.top();
	//cout << pot_res;
	test.pop();
	if (!test.empty())
		return false;
	return true;
}
/*
int main()
{
	bool ba[10] = {
	//  0      1      2      3      4      5      6      7      8      9
		true, true, true, false, false, false, true, false, true, false
	};
	string pf;
	bool answer;
	
	cout << evaluate("(2| 3)", ba, pf, answer) << endl;
	cout << evaluate("8|", ba, pf, answer) << endl;
	cout << evaluate("4 5", ba, pf, answer) << endl;
	cout << evaluate("01", ba, pf, answer) << endl;
	cout << evaluate("()", ba, pf, answer) << endl;
	cout << evaluate("2(9|8)", ba, pf, answer) << endl;
	cout << evaluate("2(&8)", ba, pf, answer) << endl;
	cout << evaluate("2&(8)", ba, pf, answer) << endl;
	cout << evaluate("(6&(7|7)", ba, pf, answer) << endl;
	
	//cout << evaluate("4  |  !3 & (0&3) ", ba, pf, answer) << endl;
	
	cout << evaluate("", ba, pf, answer) << endl;
	cout << evaluate(" 9  ", ba, pf, answer) << endl;
	cout << evaluate("((9))", ba, pf, answer) << endl;
	cout << evaluate("2| 3", ba, pf, answer) << endl;
	
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
}
*/