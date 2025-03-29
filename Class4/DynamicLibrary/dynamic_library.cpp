#include <iostream>

using namespace std;

#ifdef DYNAMIC_LIBRARY_EXPORTS
#define DYNAMIC_LIBRARY_API __declspec(dllexport)
#else
#define DYNAMIC_LIBRARY_API __declspec(dllimport)
#endif

void DYNAMIC_LIBRARY_API printDynamic()
{
  cout << "Hello Dynamic." << endl;
}