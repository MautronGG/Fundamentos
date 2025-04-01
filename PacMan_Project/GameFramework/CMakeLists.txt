add_library(GameFramework STATIC
    src/Entity.cpp
    src/Scene.cpp
    src/GraphicsComponent.cpp
    src/TransformComponent.cpp 
    src/Map.cpp 
    src/Tile.cpp 
    src/Ghost.cpp 
    #src/GameManager.cpp 
    src/Pac.cpp
)

target_include_directories(GameFramework PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)
    
target_link_libraries(GameFramework PUBLIC
    SFML::Graphics
)

target_compile_features(GameFramework PRIVATE
    ${COMMON_COMPILE_FEATURES}
)