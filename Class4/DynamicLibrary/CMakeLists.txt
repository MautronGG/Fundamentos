cmake_minimum_required (VERSION 3.8)


add_library (DynamicLibrary SHARED dynamic_library.cpp)

target_compile_definitions(DynamicLibrary PRIVATE DYNAMIC_LIBRARY_EXPORTS)