#include "Class1.h"
#include <iostream>
#include <cstdint>
#include <bitset>
#include <array>
using namespace std;

#define MAIN1
float InputNum()
{
  float userInput;
  cout << "Enter a number: ";
  cin >> userInput;
  return userInput;
}
float SUM(float a, float b)
{
  return a + b;
}
struct Padding
{
  //Shows a wrong order of variables that increases the size of the struct
	char a;  //1 byte
  int b;   //4 bytes
  short c; //2 bytes
}; 
   //char a  = 4 bytes
   //int b   = 4 bytes
   //short c = 4 bytes
   //Total: 12 bytes

//Shows a correct order of variables that decreases the size of the struct
struct Padding2
{
  char a;  //1 byte
  short c; //2 bytes
  int b;   //4 bytes
}; 
   //char a  = 2 bytes
   //int b   = 2 bytes
   //short c = 4 bytes
   //Total: 8 bytes

int Bitwise(int a1)
{
  //shift bits to the left
  int a = a1; // 5 = 0101
  int b = a << 1; //1010 << 0101 = 10

  //shift bits to the right
  int c = a1; // 5 = 0101
  int d = c >> 2; // 0101 >> 0001 = 1

  int mask = 0xF0; // 11110000 in binary
  int significant_bits = (a1 & mask) >> 4;

  //shows the results in hexadecimal and decimal
  cout << "int a hex = " << hex << a << endl;
  cout << "int a = " << dec << a << endl;

  cout << "int b hex = " << hex << b << endl;
  cout << "int b = " << dec << b << endl;

  cout << "int c hex = " << hex << c << endl;
  cout << "int c = " << dec << c << endl;

  cout << "int d hex = " << hex << d << endl;
  cout << "int d = " << dec << d << endl;

  //shows most significant 4 bits of a1
  cout << hex << "Most significant 4 bits of a1 = " << significant_bits << endl;
  //shows most significant 4 bits of a1 in binary
  cout << "Most significant 4 bits of a1 in binary = " << bitset<4>(significant_bits) << endl;

  return 0;
}
#ifdef MAIN1
int main()
{
  Operation op;
  array<int, 5> arr = { 1, 2, 3, 4, 5 };
  //runs only in DEBUG mode
#if DEBUG
  cout << "Debug on" << endl;
#endif
  //prints Padding and Padding2 sizes
  cout << "Size of Padding a: " << sizeof(Padding::a) << endl;  
  cout << "Size of Padding b: " << sizeof(Padding::b) << endl;
  cout << "Size of Padding c: " << sizeof(Padding::c) << endl;
  cout << "Total Size of Padding: " << sizeof(Padding) << endl;

  cout << "Size of Padding2 a: " << sizeof(Padding2::a) << endl;
  cout << "Size of Padding2 b: " << sizeof(Padding2::b) << endl;
  cout << "Size of Padding2 c: " << sizeof(Padding2::c) << endl;
  cout << "Total Size of Padding2: " << sizeof(Padding2) << endl;   
  
  op.results();

  //user enters a number for Bitwise function (shifting bits)
  int userInput = InputNum();
  if (cin.fail())
  {
    cout << "You entered Invalid Number: " << endl;
  }
  else
  {
    Bitwise(userInput);
  }
  float userFloat = InputNum();
  if (cin.fail() || static_cast<int>(userFloat) > arr.size() - 1)
  {
    cout << "You entered Invalid Number: " << endl;
  }
  else
  {
  cout << "SUM of: " << arr[static_cast<int>(userFloat)] << " and " << userFloat << " = " << SUM(arr[static_cast<int>(userFloat)], userFloat) << endl;
  }

	return 0;
};   
#endif