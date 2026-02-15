#include <iostream>

// GraphVM tests
void test_basic_arithmetic();
void test_subtraction();
void test_multiplication();
void test_division();
void test_division_by_zero();
void test_comparison();
void test_conditional_jump();
void test_variables();

// ECS tests
void test_create_entity();
void test_destroy_entity();
void test_tick_callback();

// ECS Component tests
void test_add_and_get_component();
void test_has_component();
void test_remove_component();
void test_multiple_components();
void test_destroy_entity_removes_components();
void test_component_update();

// Asset tests
void test_asset_binary_roundtrip();
void test_asset_registry_scan();

// Network tests
void test_net_init();
void test_net_authority();
void test_net_shutdown();

// Network queue tests
void test_net_add_peer();
void test_net_remove_peer();
void test_net_send_receive();
void test_net_broadcast_receive();
void test_net_shutdown_clears_queues();

// World tests
void test_cube_sphere_projection();
void test_cube_sphere_chunk_roundtrip();
void test_cube_sphere_neighbors();
void test_cube_sphere_lod();
void test_voxel_chunk_roundtrip();
void test_voxel_neighbors();

// Terrain mesh tests
void test_terrain_flat_mesh();
void test_terrain_heightmap_mesh();
void test_terrain_normals_computed();
void test_terrain_resolution_one();

// Noise tests
void test_perlin_range();
void test_perlin_deterministic();
void test_perlin_seed_variation();
void test_fbm_octaves();
void test_fbm_deterministic();
void test_perlin_spatial_variation();

// Streaming tests
void test_streamer_request_load();
void test_streamer_set_chunk_data();
void test_streamer_unload_chunk();
void test_streamer_get_loaded_chunks();
void test_streamer_disk_cache();
void test_streamer_duplicate_request();

// Galaxy tests
void test_galaxy_system_count();
void test_galaxy_deterministic();
void test_galaxy_seed_variation();
void test_galaxy_region_filter();
void test_galaxy_unique_ids();
void test_galaxy_star_classes();

// Compiler tests
void test_compile_constants_and_add();
void test_compile_and_execute_full();
void test_compile_multiply();

// Engine tests
void test_engine_init_and_shutdown();
void test_engine_run_loop_ticks();
void test_engine_capabilities();
void test_engine_net_mode_from_config();

// Console tests
void test_console_spawn_entity();
void test_console_ecs_dump();
void test_console_set_tickrate();
void test_console_net_mode();
void test_console_help();
void test_console_unknown_command();

int main() {
    std::cout << "=== Atlas Engine Tests ===" << std::endl;

    // GraphVM
    std::cout << "\n--- Graph VM ---" << std::endl;
    test_basic_arithmetic();
    test_subtraction();
    test_multiplication();
    test_division();
    test_division_by_zero();
    test_comparison();
    test_conditional_jump();
    test_variables();

    // ECS
    std::cout << "\n--- ECS ---" << std::endl;
    test_create_entity();
    test_destroy_entity();
    test_tick_callback();

    // ECS Components
    std::cout << "\n--- ECS Components ---" << std::endl;
    test_add_and_get_component();
    test_has_component();
    test_remove_component();
    test_multiple_components();
    test_destroy_entity_removes_components();
    test_component_update();

    // Assets
    std::cout << "\n--- Asset System ---" << std::endl;
    test_asset_binary_roundtrip();
    test_asset_registry_scan();

    // Networking
    std::cout << "\n--- Networking ---" << std::endl;
    test_net_init();
    test_net_authority();
    test_net_shutdown();

    // Network Queue
    std::cout << "\n--- Network Queue ---" << std::endl;
    test_net_add_peer();
    test_net_remove_peer();
    test_net_send_receive();
    test_net_broadcast_receive();
    test_net_shutdown_clears_queues();

    // World
    std::cout << "\n--- World Layout ---" << std::endl;
    test_cube_sphere_projection();
    test_cube_sphere_chunk_roundtrip();
    test_cube_sphere_neighbors();
    test_cube_sphere_lod();
    test_voxel_chunk_roundtrip();
    test_voxel_neighbors();

    // Terrain Mesh
    std::cout << "\n--- Terrain Mesh ---" << std::endl;
    test_terrain_flat_mesh();
    test_terrain_heightmap_mesh();
    test_terrain_normals_computed();
    test_terrain_resolution_one();

    // Noise
    std::cout << "\n--- Noise Generator ---" << std::endl;
    test_perlin_range();
    test_perlin_deterministic();
    test_perlin_seed_variation();
    test_fbm_octaves();
    test_fbm_deterministic();
    test_perlin_spatial_variation();

    // Streaming
    std::cout << "\n--- World Streaming ---" << std::endl;
    test_streamer_request_load();
    test_streamer_set_chunk_data();
    test_streamer_unload_chunk();
    test_streamer_get_loaded_chunks();
    test_streamer_disk_cache();
    test_streamer_duplicate_request();

    // Galaxy
    std::cout << "\n--- Galaxy Generator ---" << std::endl;
    test_galaxy_system_count();
    test_galaxy_deterministic();
    test_galaxy_seed_variation();
    test_galaxy_region_filter();
    test_galaxy_unique_ids();
    test_galaxy_star_classes();

    // Compiler
    std::cout << "\n--- Graph Compiler ---" << std::endl;
    test_compile_constants_and_add();
    test_compile_and_execute_full();
    test_compile_multiply();

    // Engine
    std::cout << "\n--- Engine ---" << std::endl;
    test_engine_init_and_shutdown();
    test_engine_run_loop_ticks();
    test_engine_capabilities();
    test_engine_net_mode_from_config();

    // Console
    std::cout << "\n--- Console ---" << std::endl;
    test_console_spawn_entity();
    test_console_ecs_dump();
    test_console_set_tickrate();
    test_console_net_mode();
    test_console_help();
    test_console_unknown_command();

    std::cout << "\n=== All tests passed! ===" << std::endl;
    return 0;
}
