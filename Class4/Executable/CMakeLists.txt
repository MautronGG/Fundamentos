cmake_minimum_required (VERSION 3.8)


add_executable (Class4 "Class4.cpp" "Class4.h")

set_target_properties(Class4 PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
)


target_include_directories(Class4 PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(Class4 PRIVATE StaticLibrary DynamicLibrary)

#add_dependencies(Class4 DynamicLibraryRuntime)

#add_custom_command(TARGET Class4 POST_BUILD
#    COMMAND ${CMAKE_COMMAND} -E copy_if_different
#    $<TARGET_FILE:DynamicLibrary>
#    $<TARGET_FILE_DIR:Class4>
#)