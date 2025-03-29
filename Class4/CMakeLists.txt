# CMakeList.txt : CMake project for Class4, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

# Enable Hot Reload for MSVC compilers if supported.
#if (POLICY CMP0141)
#  cmake_policy(SET CMP0141 NEW)
#  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
#endif()

project ("Class4")

# Add source to this project's executable.

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)


#if (CMAKE_VERSION VERSION_GREATER 3.12)
#  set_property(TARGET Class4 PROPERTY CXX_STANDARD 20)
#endif()

add_subdirectory(Executable)
add_subdirectory(StaticLibrary)
add_subdirectory(DynamicLibrary)
add_subdirectory(DynamicLibraryRuntime)
# TODO: Add tests and install targets if needed.
