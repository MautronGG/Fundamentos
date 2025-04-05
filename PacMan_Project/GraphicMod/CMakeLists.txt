add_library(GraphicMod SHARED EntryPoint.cpp)

target_include_directories(GraphicMod PUBLIC 
  ${CMAKE_CURRENT_SOURCE_DIR}/include
  ${CMAKE_CURRENT_SOURCE_DIR}/../GameFramework/include
)

target_link_libraries(GraphicMod PUBLIC
  SFML::Graphics
)

target_compile_features(GraphicMod PRIVATE ${COMMON_COMPILE_FEAUTRES})
target_compile_definitions(GraphicMod PRIVATE DYNAMIC_LIBRARY_EXPORTS)