cmake_minimum_required (VERSION 3.8)

add_library (StaticLibrary STATIC static_library.cpp)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)