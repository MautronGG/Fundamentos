cmake_minimum_required (VERSION 3.8)


add_library (DynamicLibraryRuntime SHARED dll_runtime.cpp)

target_compile_definitions(DynamicLibraryRuntime PRIVATE DYNAMIC_LIBRARY_EXPORTS)