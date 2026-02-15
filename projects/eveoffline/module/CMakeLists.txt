# EveOffline Game Module
#
# This CMakeLists.txt demonstrates how an external game builds a module
# that links against AtlasEngine + AtlasGameplay.
#
# In a standalone game repo, the consumer would instead use:
#   find_package(AtlasEngine REQUIRED)
#   find_package(AtlasGameplay REQUIRED)
# and link via the imported targets.

# When built as part of the Atlas tree the targets already exist.
add_library(EveOfflineModule STATIC
    EveOfflineModule.cpp
)

target_include_directories(EveOfflineModule PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(EveOfflineModule PUBLIC AtlasEngine AtlasGameplay)
