cmake_minimum_required(VERSION 3.8)

add_executable(PacMan_exe main.cpp)

target_include_directories(PacMan_exe PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/../GameFramework/include
)

target_compile_features(PacMan_exe PRIVATE ${COMMON_COMPILE_FEATURES})
target_link_libraries(PacMan_exe PRIVATE GameFramework)
#set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -rdynamic")
