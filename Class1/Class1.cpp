#include "Class1.h"
#include <iostream>
#include <cstdint>
using namespace std;

#define Flag1(a,b) (a * b)

int Operation::InputOper()
{
  int m_userInput;
  cout << "Enter a number for multiplication: ";
  cin >> m_userInput;
  return m_userInput;
}
void Operation::results()
{
  int input1 = InputOper();
  int input2 = InputOper();
  int result = Flag1(input1, input2);
  cout << "Result: " << result << endl;
  if (result >= 35 || result >= 30)
  {
    cout << "OR" << endl;
  }
  else if ((result >= 10) ^ (result >= 15))
  {
    cout << "XOR" << endl;
  }
  else if (result <= 5 && result <= 10)
  {
    cout << "AND" << endl;
  }
}