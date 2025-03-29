// MathTest.cpp : Defines the entry point for the application.
//
#include <functional>
#include "MathTest.h"
int Add(int a, int b)
{
  return a + b;
}

class Math
{
public:
  int Add(int a, int b)
  {
    return a + b;
  }
};

int main()
{
  Math* calculator = new Math();
  std::function<int(int, int)> addMember = std::bind(&Math::Add, calculator, std::placeholders::_1, std::placeholders::_2);

  delete calculator;
  std::cout << addMember(2, 3) << std::endl;
  return 0;
}
