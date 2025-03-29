// Class4.cpp : Defines the entry point for the application.
//

#include "Class4.h"
#include <windows.h>

typedef void (*printFoo)();

using namespace std;


void printStatic();
void printDynamic();
int main()
{
	cout << "Hello CMake." << endl;
  printStatic();
  printDynamic();

  HMODULE hDll = LoadLibrary("DynamicLibraryRuntime.dll");
  if (nullptr == hDll)
  {
    std::cout << "Failed to load library!" << std::endl;
    return 1;
  }

  printFoo foo = reinterpret_cast<printFoo>(GetProcAddress(hDll, "printDynamicRuntime"));
  if (nullptr == foo) {
    std::cerr << "Failed to load function!" << std::endl;
    return 1;
  }

  foo();
  FreeLibrary(hDll);
  std::cin.get();

	return 0;
}
