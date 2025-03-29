#include <iostream>

using namespace std;

#ifdef DYNAMIC_LIBRARY_EXPORTS
#define DYNAMIC_LIBRARY_API __declspec(dllexport)
#else
#define DYNAMIC_LIBRARY_API __declspec(dllimport)
#endif

extern "C" void DYNAMIC_LIBRARY_API printDynamicRuntime()
{
  cout << "Hello Dynamic in Runtime." << endl;
}