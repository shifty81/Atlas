add_library(AtlasGameplay STATIC
    FactionSystem.cpp
    CombatFramework.cpp
    EconomySystem.cpp
)

target_include_directories(AtlasGameplay PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(AtlasGameplay PUBLIC AtlasEngine)
