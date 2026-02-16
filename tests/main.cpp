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

// Marketplace importer tests
void test_marketplace_registry();
void test_itch_io_importer();
void test_marketplace_metadata();
void test_marketplace_import_options();
void test_unreal_marketplace_importer();
void test_unity_assetstore_importer();

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
void test_engine_frame_pacing_default_enabled();
void test_engine_frame_pacing_prevents_spin();
void test_engine_ui_update_in_loop();

// Logger tests
void test_logger_creates_log_directory();
void test_logger_writes_to_file();

// Console tests
void test_console_spawn_entity();
void test_console_ecs_dump();
void test_console_set_tickrate();
void test_console_net_mode();
void test_console_help();
void test_console_unknown_command();

// Interaction tests
void test_utterance_creation();
void test_intent_creation();
void test_rule_intent_resolver();
void test_interaction_system_uninitialized();
void test_interaction_system_no_match();
void test_interaction_system_full_pipeline();

// Interaction (graph-based) tests
void test_intent_registry_dispatch();
void test_intent_registry_missing();
void test_intent_registry_has_handler();
void test_pattern_resolver();
void test_pattern_resolver_no_match();
void test_pattern_resolver_case_insensitive();

// Diplomacy tests
void test_faction_request_neutral();
void test_faction_request_trusted();
void test_faction_request_hostile();
void test_faction_threaten_increases_hostility();
void test_faction_unknown_intent();
void test_faction_personality_affects_hostility();

// Project tests
void test_project_load_valid();
void test_project_load_minimal();
void test_project_load_missing_file();
void test_project_invalid_schema();
void test_project_missing_name();
void test_project_missing_name_field();
void test_project_invalid_version();
void test_project_schema_validation();

// Command tests
void test_command_execute();
void test_command_undo();
void test_command_redo();
void test_command_redo_cleared_on_execute();
void test_command_clear();
void test_command_last_executed();
void test_command_multiple_undo_redo();

// WorldGraph tests
void test_worldgraph_add_nodes();
void test_worldgraph_remove_node();
void test_worldgraph_compile_empty();
void test_worldgraph_compile_single_node();
void test_worldgraph_compile_chain();
void test_worldgraph_deterministic();
void test_worldgraph_clamp_node();

// Voice Command tests
void test_voice_register_command();
void test_voice_match_command();
void test_voice_no_match();
void test_voice_get_by_context();

// Plugin System tests
void test_plugin_validate_valid();
void test_plugin_validate_no_name();
void test_plugin_validate_nondeterministic();
void test_plugin_validate_incompatible_version();
void test_plugin_registry_register();
void test_plugin_registry_find();
void test_plugin_registry_unregister();
void test_plugin_registry_get_by_type();

// Heightfield tests
void test_heightfield_at();
void test_heightfield_mesh_generation();
void test_heightfield_mesh_lod();

// StrategyGraph tests
void test_strategygraph_add_nodes();
void test_strategygraph_remove_node();
void test_strategygraph_compile_empty();
void test_strategygraph_compile_single_node();
void test_strategygraph_compile_chain();
void test_strategygraph_execute();
void test_strategygraph_deterministic();
void test_strategygraph_objective_score();
void test_strategygraph_read_ai_signal();
void test_strategygraph_emit_action();

// Server Rules tests
void test_server_rules_register();
void test_server_rules_get_value();
void test_server_rules_set_multiplier();
void test_server_rules_clamp_multiplier();
void test_server_rules_missing_rule();
void test_server_rules_hot_reload_flag();
void test_server_rules_clear();

// ConversationGraph tests
void test_conversation_add_nodes();
void test_conversation_remove_node();
void test_conversation_compile_empty();
void test_conversation_dialogue_node();
void test_conversation_chain();
void test_conversation_relationship_delta();
void test_conversation_player_choice();

// AI Signal tests
void test_ai_signal_register();
void test_ai_signal_read_write();
void test_ai_signal_missing();
void test_ai_signal_namespace();
void test_ai_signal_clear();

// AI Memory tests
void test_ai_memory_store_recall();
void test_ai_memory_has();
void test_ai_memory_forget();
void test_ai_memory_decay();
void test_ai_memory_permanent();
void test_ai_memory_clear();

// Relationship Model tests
void test_relationship_set_get();
void test_relationship_default_zero();
void test_relationship_modify();
void test_relationship_has();
void test_relationship_remove();
void test_relationship_clear();

// Asset graph tests
void test_asset_context();
void test_asset_graph_executor_empty();
void test_asset_graph_executor_single_node();
void test_asset_graph_executor_multiple_nodes();
void test_damage_state();

// Mod tests
void test_mod_register_graph();
void test_mod_register_archetype();
void test_mod_registry_mixed();

// Editor assistant tests
void test_editor_assistant_explain_performance();
void test_editor_assistant_explain_node();
void test_editor_assistant_unknown();
void test_interaction_debugger_record();
void test_interaction_debugger_clear();

// Input tests
void test_input_bind_action();
void test_input_unbind_action();
void test_input_press_release();
void test_input_axis();
void test_input_callback();
void test_input_unbound_action();

// Camera tests
void test_camera_default_state();
void test_camera_set_mode();
void test_camera_position();
void test_camera_fov_clamp();
void test_camera_orbit();
void test_camera_movement();
void test_camera_pitch_clamp();

// Physics tests
void test_physics_create_body();
void test_physics_destroy_body();
void test_physics_gravity();
void test_physics_static_body();
void test_physics_apply_force();
void test_physics_collision_detection();

// Audio tests
void test_audio_load_sound();
void test_audio_unload_sound();
void test_audio_play_pause_stop();
void test_audio_volume();
void test_audio_master_volume();
void test_audio_looping();

// Gameplay mechanic tests
void test_mechanic_register();
void test_mechanic_unregister();
void test_mechanic_find_by_name();
void test_mechanic_params();
void test_mechanic_get_by_type();

// Skill tree tests
void test_skill_add_node();
void test_skill_unlock();
void test_skill_prerequisites();
void test_skill_effects();
void test_skill_tree_chain();

// Weapon Graph tests
void test_weapongraph_add_nodes();
void test_weapongraph_remove_node();
void test_weapongraph_compile_empty();
void test_weapongraph_compile_single_node();
void test_weapongraph_compile_chain();
void test_weapongraph_execute();
void test_weapongraph_wear_degrades_stats();
void test_weapongraph_deterministic();

// Character Graph tests
void test_charactergraph_add_nodes();
void test_charactergraph_remove_node();
void test_charactergraph_compile_empty();
void test_charactergraph_compile_single_node();
void test_charactergraph_compile_chain();
void test_charactergraph_execute();
void test_charactergraph_deterministic();

// Animation Graph tests
void test_animationgraph_add_nodes();
void test_animationgraph_remove_node();
void test_animationgraph_compile_empty();
void test_animationgraph_compile_single_node();
void test_animationgraph_compile_chain();
void test_animationgraph_execute();
void test_animationgraph_modifier();
void test_animationgraph_deterministic();

// Tile Graph tests
void test_tilegraph_add_nodes();
void test_tilegraph_remove_node();
void test_tilegraph_compile_empty();
void test_tilegraph_compile_single_node();
void test_tilegraph_compile_chain();
void test_tilegraph_execute();
void test_tilegraph_deterministic();

// Sound Graph tests
void test_soundgraph_add_nodes();
void test_soundgraph_remove_node();
void test_soundgraph_compile_empty();
void test_soundgraph_compile_single_node();
void test_soundgraph_compile_chain();
void test_soundgraph_execute();
void test_soundgraph_deterministic();

// BehaviorGraph tests
void test_behaviorgraph_add_nodes();
void test_behaviorgraph_remove_node();
void test_behaviorgraph_compile_empty();
void test_behaviorgraph_compile_single_node();
void test_behaviorgraph_compile_chain();
void test_behaviorgraph_execute();
void test_behaviorgraph_deterministic();

// UI Screen tests
void test_ui_add_widget();
void test_ui_visibility();
void test_ui_parent_child();
void test_ui_remove_widget();

// Game Flow Graph tests
void test_gameflowgraph_add_nodes();
void test_gameflowgraph_remove_node();
void test_gameflowgraph_compile_empty();
void test_gameflowgraph_compile_single_node();
void test_gameflowgraph_compile_chain();
void test_gameflowgraph_execute();
void test_gameflowgraph_deterministic();

// Flow IR tests
void test_flow_ir_default_values();
void test_flow_ir_add_nodes();
void test_flow_ir_add_edges();
void test_flow_ir_to_json();
void test_flow_ir_roundtrip();
void test_flow_ir_validate_valid();
void test_flow_ir_validate_invalid();
void test_flow_ir_migrate_v1();

// Flow Codegen tests
void test_flow_codegen_nonempty();
void test_flow_codegen_function_signature();
void test_flow_codegen_empty_graph();
void test_flow_codegen_node_properties();
void test_flow_codegen_edge_wiring();
void test_flow_codegen_all_node_types();
void test_flow_codegen_header_comment();

// Flow Debugger tests
void test_debugger_initial_state();
void test_debugger_add_breakpoint();
void test_debugger_remove_breakpoint();
void test_debugger_clear_breakpoints();
void test_debugger_execute_no_breakpoints();
void test_debugger_breakpoint_pause();
void test_debugger_step_node();

// Shader IR tests
void test_shader_module_defaults();
void test_shader_passthrough_vertex();
void test_shader_solid_color_fragment();
void test_shader_validate_valid();
void test_shader_validate_empty();
void test_shader_hash_deterministic();
void test_shader_hash_different();
void test_shader_serialize_roundtrip();

// Build Manifest tests
void test_manifest_defaults();
void test_manifest_add_artifact();
void test_manifest_find_artifact();
void test_manifest_find_missing();
void test_manifest_validate_valid();
void test_manifest_validate_missing_fields();
void test_manifest_to_json();
void test_manifest_roundtrip();
void test_manifest_verify_hash();

// UIGraph tests
void test_uigraph_add_nodes();
void test_uigraph_remove_node();
void test_uigraph_compile_empty();
void test_uigraph_compile_single_node();
void test_uigraph_compile_chain();
void test_uigraph_execute();
void test_uigraph_deterministic();

// UICommandBus tests
void test_command_bus_enqueue();
void test_command_bus_drain();
void test_command_bus_clear();
void test_command_bus_dispatch();
void test_command_bus_dispatch_unhandled();
void test_command_bus_move_enqueue();

// UIManager tests
void test_ui_manager_init();
void test_ui_manager_context_switch();
void test_ui_manager_screen_access();
void test_ui_manager_graph_access();
void test_ui_manager_update();
void test_ui_manager_command_bus();
void test_ui_manager_shutdown();
void test_ui_manager_server_context();

// UILayoutSolver tests
void test_layout_solver_single_entry();
void test_layout_solver_horizontal_split();
void test_layout_solver_vertical_split();
void test_layout_solver_weighted();
void test_layout_solver_min_size_respect();
void test_layout_solver_clear();
void test_layout_solver_deterministic();
void test_layout_solver_offset();

// UI Nodes Extended tests
void test_slotgrid_node_defaults();
void test_inputfield_node_defaults();
void test_inputfield_node_with_text();
void test_slotgrid_node_metadata();
void test_inputfield_node_metadata();

// Schema Validator tests
void test_schema_valid();
void test_schema_empty_id();
void test_schema_zero_version();
void test_schema_duplicate_node_id();
void test_schema_empty_node_id();
void test_schema_no_nodes();

// Graph Cache tests
void test_cache_store_and_get();
void test_cache_has();
void test_cache_invalidate();
void test_cache_invalidate_all();
void test_cache_evict_before();
void test_cache_hash_key();

// Graph Serialization tests
void test_json_builder_object();
void test_json_builder_array();
void test_json_reader_parse();
void test_json_reader_array();
void test_json_roundtrip();

// ECS Serialization tests
void test_ecs_serialize_empty_world();
void test_ecs_serialize_entities_no_components();
void test_ecs_serialize_roundtrip();
void test_ecs_serialize_preserves_next_id();
void test_ecs_deserialize_clears_existing();
void test_ecs_deserialize_invalid_data();
void test_ecs_has_serializer();

// Snapshot / Rollback tests
void test_snapshot_saves_ecs_state();
void test_rollback_restores_ecs_state();
void test_rollback_removes_future_snapshots();
void test_snapshot_without_world();
void test_rollback_with_multiple_entities();
void test_record_and_replay_input();
void test_replay_applies_input_frames();

// ECS Inspector tests
void test_inspector_empty_world();
void test_inspector_entities_with_components();
void test_inspector_select_entity();
void test_inspector_refreshes_on_draw();
void test_inspector_state_blocks_empty();
void test_inspector_state_blocks_with_world_state();

// Replication tests
void test_replication_add_rule();
void test_replication_remove_rule();
void test_replication_get_rule();
void test_replication_replace_rule();
void test_replication_dirty_tracking();
void test_replication_dirty_no_duplicates();
void test_replication_collect_delta();
void test_replication_apply_delta();
void test_replication_multiple_rules();
void test_replication_delta_roundtrip();
void test_replication_delta_every_tick();

// Asset Browser tests
void test_asset_browser_empty();
void test_asset_browser_with_assets();
void test_asset_browser_filter();
void test_asset_browser_sort_modes();
void test_asset_browser_select_asset();

// Asset Cooker tests
void test_cooker_default_settings();
void test_cooker_set_output_dir();
void test_cooker_cook_single_asset();
void test_cooker_source_not_found();
void test_cooker_cook_all();
void test_cooker_strip_editor_data();
void test_cooker_clear_log();
void test_cooker_progress_callback();

// Graph Editor Panel tests
void test_graph_panel_no_graph();
void test_graph_panel_with_nodes();
void test_graph_panel_select_node();
void test_graph_panel_compile_and_preview();
void test_graph_panel_summary();
void test_graph_panel_node_info();

// Asset Graph Assistant tests
void test_assistant_suggest_empty();
void test_assistant_suggest_with_executor();
void test_assistant_explain_no_executor();
void test_assistant_explain_with_nodes();
void test_assistant_mutate_no_executor();
void test_assistant_mutate_intensity();

// Story tests
void test_story_add_node();
void test_story_connections();
void test_story_tags();
void test_story_get_by_type();

// Build Profile tests
void test_profile_default_debug();
void test_profile_default_development();
void test_profile_default_release();
void test_profile_name();
void test_profile_set_active();
void test_profile_set_custom_config();
void test_profile_overrides();
void test_profile_override_missing_key();

// Mod Loader tests
void test_mod_loader_register();
void test_mod_loader_register_invalid();
void test_mod_loader_register_duplicate();
void test_mod_loader_unregister();
void test_mod_loader_enable_disable();
void test_mod_loader_get_mod();
void test_mod_loader_dependencies();
void test_mod_loader_scan_directory();
void test_mod_loader_clear();

// Platform Target tests
void test_platform_default_windows();
void test_platform_default_linux();
void test_platform_default_macos();
void test_platform_default_web();
void test_platform_name();
void test_platform_set_target();
void test_platform_custom_config();
void test_platform_module_exclusion();
void test_platform_host();
void test_platform_supported_list();

// Profiler Panel tests
void test_profiler_empty();
void test_profiler_record_frame();
void test_profiler_average_and_peak();
void test_profiler_max_history();
void test_profiler_system_metrics();
void test_profiler_pause();
void test_profiler_clear();

// Replay Recorder tests
void test_replay_initial_state();
void test_replay_start_recording();
void test_replay_record_frames();
void test_replay_stop_recording();
void test_replay_save_and_load();
void test_replay_load_invalid();
void test_replay_clear();

// State Hasher tests
void test_hasher_initial_state();
void test_hasher_advance_tick();
void test_hasher_deterministic();
void test_hasher_chaining();
void test_hasher_divergence_detection();
void test_hasher_different_seeds();
void test_hasher_empty_data();
void test_hasher_raw_pointer_api();
void test_replay_record_with_hash();
void test_replay_save_load_with_hash();
void test_replay_default_hash_zero();
void test_hash_combine_deterministic();

// Visual Diff tests
void test_diff_identical();
void test_diff_added();
void test_diff_removed();
void test_diff_modified();
void test_diff_empty();
void test_diff_compare_graphs();
void test_diff_summarize();

// Crash Handler tests
void test_crash_breadcrumb();
void test_crash_breadcrumb_limit();
void test_crash_clear_breadcrumbs();
void test_crash_generate_report();
void test_crash_save_report();
void test_crash_callback();
void test_crash_report_to_file();

// Net Inspector Panel tests
void test_net_inspector_standalone();
void test_net_inspector_server_with_peers();
void test_net_inspector_mode_names();

// Game Packager Panel tests
void test_game_packager_defaults();
void test_game_packager_summary();
void test_game_packager_modify_settings();

// Game Module tests
void test_game_module_describe();
void test_game_module_lifecycle();
void test_module_loader_static();
void test_module_loader_already_loaded();
void test_module_loader_not_found();

// Atlas Gameplay tests
void test_faction_register();
void test_faction_relations();
void test_faction_clear();
void test_combat_register_unit();
void test_combat_resolve();
void test_combat_clear();
void test_economy_register();
void test_economy_transactions();
void test_economy_capacity();
void test_economy_clear();

// EveOffline Module tests
void test_eveoffline_describe();
void test_eveoffline_register_types();
void test_eveoffline_replication_rules();
void test_eveoffline_server_rules();
void test_eveoffline_lifecycle();
void test_eveoffline_factory();

// Arena2D Module tests
void test_arena2d_describe();
void test_arena2d_register_types();
void test_arena2d_replication_rules();
void test_arena2d_server_rules();
void test_arena2d_lifecycle();
void test_arena2d_factory();

// Replay Timeline Panel tests
void test_replay_timeline_empty();
void test_replay_timeline_load();
void test_replay_timeline_scrub();
void test_replay_timeline_markers();
void test_replay_timeline_compare_identical();
void test_replay_timeline_compare_divergent();
void test_replay_timeline_inject_input();
void test_replay_timeline_branch();

// Replay Divergence Inspector tests
void test_divergence_no_mismatch();
void test_divergence_hash_mismatch();
void test_divergence_frame_comparison();
void test_divergence_single_frame_check();
void test_divergence_report_format();
void test_divergence_history();
void test_divergence_severity();

// Deterministic Allocator tests
void test_allocator_create();
void test_allocator_single_alloc();
void test_allocator_multiple_allocs();
void test_allocator_dealloc();
void test_allocator_reuse_space();
void test_allocator_stable_ids();
void test_allocator_reset();
void test_allocator_deterministic();

// Permission Manager tests
void test_permission_defaults();
void test_permission_register_user();
void test_permission_tier_viewonly();
void test_permission_tier_developer();
void test_permission_tier_admin();
void test_permission_grant_custom();
void test_permission_revoke();
void test_permission_user_management();

// HUD Overlay tests
void test_hud_defaults();
void test_hud_tick_display();
void test_hud_warnings();
void test_hud_critical_warning();
void test_hud_time_controls();
void test_hud_step_forward();
void test_hud_determinism_status();
void test_hud_visibility();
void test_hud_world_dilation();
void test_hud_world_paused();

// Time Model tests
void test_time_model_defaults();
void test_time_model_set_tick_rate();
void test_time_model_advance_tick();
void test_time_model_world_dilation();
void test_time_model_world_pause();
void test_time_model_presentation();
void test_time_model_reset();
void test_simulation_time_elapsed();

// World State tests
void test_world_state_register_block();
void test_world_state_find_block();
void test_world_state_snapshot();
void test_world_state_snapshot_deterministic();
void test_world_state_push_and_retrieve();
void test_world_state_max_snapshots();
void test_world_state_prune();
void test_world_state_clear();
void test_world_state_derived_rebuild();

// Save System tests
void test_save_system_save_and_load();
void test_save_system_hash_integrity();
void test_save_system_file_not_found();
void test_save_system_validate();
void test_save_system_invalid_format();
void test_save_system_empty_data();
void test_save_system_clear();

// Engine Integration tests
void test_engine_time_model_initialized();
void test_engine_time_model_tick_rate_matches();
void test_engine_time_model_world_time_advances();
void test_engine_world_state_snapshots_in_server();
void test_engine_world_state_snapshot_at_tick();
void test_engine_save_system_accessible();
void test_console_save_command();
void test_console_load_command();
void test_console_load_file_not_found();
void test_console_save_no_path();
void test_console_time_info();
void test_ai_behavior_deterministic_across_runs();
void test_ai_relationship_determinism_across_save_load();

// Next Tasks tests
void test_engine_autosave_creates_file();
void test_time_model_set_tick();
void test_replay_save_point_mark();
void test_replay_save_point_multiple();
void test_replay_save_point_serialization();
void test_replay_start_from_save();
void test_engine_load_and_replay();
void test_divergence_export_report();
void test_engine_rollback_to_tick();
void test_net_broadcast_save_tick();
void test_state_hash_diff_panel_no_divergence();
void test_state_hash_diff_panel_with_divergence();
void test_hash_ladder_save_load_continuity();
void test_engine_replay_from_save();
void test_engine_replay_from_save_bad_save();
void test_engine_replay_from_save_bad_replay();
void test_state_hash_diff_panel_component_breakdown();
void test_state_hash_diff_panel_no_component_divergence();

// Next Tasks Phase 2 tests
void test_behaviorgraph_serialize_state();
void test_behaviorgraph_serialize_empty();
void test_partial_save_and_load();
void test_partial_save_hash_integrity();
void test_partial_save_empty_chunks();
void test_save_file_browser_scan();
void test_save_file_browser_empty_dir();
void test_save_file_browser_select();
void test_hash_ladder_comparison();
void test_hash_ladder_comparison_no_divergence();
void test_divergence_detail();
void test_divergence_detail_no_divergence();
void test_replay_auto_divergence_callback();
void test_replay_no_divergence_callback();
void test_detailed_divergence_report();
void test_detailed_divergence_report_no_divergence();

// GUI DSL Parser tests
void test_dsl_parse_simple_layout();
void test_dsl_parse_split();
void test_dsl_parse_nested_splits();
void test_dsl_parse_dock();
void test_dsl_parse_tabs();
void test_dsl_parse_full_example();
void test_dsl_parse_comments();
void test_dsl_parse_error_missing_layout();
void test_dsl_parse_error_unterminated_string();
void test_dsl_parse_error_unknown_statement();
void test_dsl_version();
void test_dsl_parse_dock_targets();

// GUI Input Recorder tests
void test_recorder_initial_state();
void test_recorder_start_stop_recording();
void test_recorder_record_events();
void test_recorder_record_only_when_recording();
void test_recorder_playback();
void test_recorder_save_load();
void test_recorder_load_invalid_file();
void test_recorder_load_nonexistent_file();

// Headless GUI tests
void test_headless_gui_init();
void test_headless_gui_available_commands();
void test_headless_gui_widget_count();
void test_headless_gui_widget_info();
void test_headless_gui_widget_info_not_found();
void test_headless_gui_unknown_command();
void test_headless_gui_empty_command();
void test_headless_gui_help();
void test_headless_gui_status();
void test_headless_gui_enqueue();
void test_headless_gui_custom_command();
void test_headless_gui_quoted_args();

// Job Execution Tracer
void test_job_tracer_empty();
void test_job_tracer_single_tick();
void test_job_tracer_multiple_ticks();
void test_job_tracer_deterministic_hash();
void test_job_tracer_different_order_different_hash();
void test_job_tracer_compare_order();
void test_job_tracer_max_history();
void test_job_tracer_clear();
void test_job_trace_panel_no_tracer();
void test_job_trace_panel_consistent();
void test_job_trace_panel_mismatch();
void test_job_trace_panel_entries_at_tick();

// Render and Platform tests
void test_render_api_enum();
void test_null_renderer();
void test_gl_renderer_viewport();
void test_vulkan_renderer_viewport();
void test_ui_manager_set_renderer();
void test_ui_manager_render_empty();
void test_ui_manager_render_widgets();
void test_ui_manager_render_null_renderer();
void test_engine_config_render_fields();
void test_engine_headless_no_window();
void test_engine_server_no_window();
void test_platform_window_event_type();
void test_platform_window_config();

// Component Category
void test_component_category_defaults();
void test_component_category_simulated();
void test_component_category_presentation();
void test_component_category_debug();
void test_component_category_derived();
void test_component_category_assert_simulated();
void test_component_category_assert_not_presentation();
void test_component_category_runtime_query();

// Include Firewall tests
void test_include_firewall_sim_no_render();
void test_include_firewall_contract_no_render();

// FP Drift Detector tests
void test_fp_drift_no_drift();
void test_fp_drift_detected();
void test_fp_drift_system_comparison();
void test_fp_drift_format_report();
void test_fp_drift_export_report();
void test_fp_drift_add_system_hashes();

// Asset Import Pipeline tests
void test_asset_import_registry_empty();
void test_asset_import_registry_register();
void test_asset_import_mesh_can_import();
void test_asset_import_texture_can_import();
void test_asset_import_audio_can_import();
void test_asset_import_font_can_import();
void test_asset_import_sprite_can_import();
void test_asset_import_find_importer();
void test_asset_import_file_roundtrip();
void test_asset_import_file_not_found();
void test_asset_import_registry_import();

// Asset Validator tests
void test_asset_validator_validate_valid_file();
void test_asset_validator_validate_corrupt_file();
void test_asset_validator_validate_header_bad_magic();
void test_asset_validator_migration_path();
void test_asset_validator_dependencies();
void test_asset_validator_circular_dependency();
void test_asset_validator_no_circular_dependency();
void test_asset_validator_compute_file_hash();

// Golden Replay tests
void run_golden_replay_tests();

// Enforcement tests
void test_tick_rate_lock();
void test_tick_rate_set_before_lock();
void test_engine_locks_tick_rate_on_run();
void test_engine_system_order();
void test_engine_system_order_deterministic();
void test_net_packet_schema_version();
void test_asset_lock_and_check();
void test_asset_lock_multiple();
void test_determinism_config_exists();

// Lockdown Phase 2 tests
void test_rollback_and_verify_success();
void test_rollback_and_verify_bad_ticks();
void test_asset_schema_lock();
void test_asset_schema_lock_default();
void test_layout_hash_deterministic();
void test_layout_hash_differs_on_change();
void test_layout_hash_empty();
void test_mutation_ownership_register();
void test_mutation_ownership_exclusive();
void test_mutation_can_mutate();
void test_mutation_owned_components();
void test_mutation_owner_of_unregistered();

// Lockdown Phase 3 tests
void test_auditor_empty_passes();
void test_auditor_system_no_state_fails();
void test_auditor_system_with_state_passes();
void test_auditor_multiple_systems();
void test_auditor_duplicate_register();
void test_auditor_declared_blocks();
void test_auditor_unregistered_declaration_warning();
void test_auditor_clear();
void test_server_validator_empty_manifest();
void test_server_validator_register_and_check();
void test_server_validator_hash_match();
void test_server_validator_hash_mismatch();
void test_server_validator_unknown_asset();
void test_server_validator_file_validation();
void test_server_validator_file_not_found();
void test_server_validator_manifest();
void test_verify_save_load_determinism();
void test_desync_reproducer_output_dir();
void test_desync_reproducer_capture();
void test_desync_reproducer_generate_command();
void test_desync_reproducer_multiple_captures();

// GUI Completion
void test_editor_theme_dark_defaults();
void test_editor_theme_custom_values();
void test_panel_closable_by_default();
void test_panel_set_non_closable();
void test_layout_close_closable_panel();
void test_layout_close_non_closable_panel();
void test_layout_close_unknown_panel();
void test_layout_find_panel();
void test_main_panels_non_closable();
void test_attach_init();
void test_attach_standalone();
void test_attach_live_client();
void test_attach_live_client_missing_host();
void test_attach_headless_server();
void test_attach_replay();
void test_attach_replay_missing_path();
void test_attach_disconnect();
void test_default_editor_dsl_parses();
void test_default_editor_dsl_has_panels();

// Proof Viewer Panel tests
void test_proof_viewer_init();
void test_proof_viewer_load_specs();
void test_proof_viewer_select_spec();
void test_proof_viewer_tokenize();
void test_proof_viewer_invariants();
void test_proof_viewer_verification_status();
void test_proof_viewer_line_count();
void test_proof_viewer_empty_select();

// Replay Proof Exporter tests
void test_proof_exporter_empty();
void test_proof_exporter_add_entries();
void test_proof_exporter_validate_chain_valid();
void test_proof_exporter_validate_chain_invalid();
void test_proof_exporter_export_tla();
void test_proof_exporter_export_json();
void test_proof_exporter_export_csv();
void test_proof_exporter_save_points();
void test_proof_exporter_format_entry();
void test_proof_exporter_clear();

// CI Dashboard Panel tests
void test_ci_dashboard_init();
void test_ci_dashboard_start_run();
void test_ci_dashboard_add_checks();
void test_ci_dashboard_complete_run_passed();
void test_ci_dashboard_complete_run_failed();
void test_ci_dashboard_partial_failure();
void test_ci_dashboard_pass_rate();
void test_ci_dashboard_history();
void test_ci_dashboard_max_history();
void test_ci_dashboard_summary();
void test_ci_dashboard_clear();

// TLC Model Checker
void test_tlc_register_spec();
void test_tlc_register_multiple_specs();
void test_tlc_register_bad_path();
void test_tlc_unregister_spec();
void test_tlc_get_spec();
void test_tlc_extract_invariants_replay();
void test_tlc_extract_invariants_ecs();
void test_tlc_extract_invariants_layout();
void test_tlc_validate_spec();
void test_tlc_check_spec();
void test_tlc_check_spec_not_found();
void test_tlc_check_all();
void test_tlc_ci_check_pass();
void test_tlc_format_report();
void test_tlc_export_report();
void test_tlc_path_management();
void test_tlc_extract_invariants_static();

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

    // Replication
    std::cout << "\n--- Replication ---" << std::endl;
    test_replication_add_rule();
    test_replication_remove_rule();
    test_replication_get_rule();
    test_replication_replace_rule();
    test_replication_dirty_tracking();
    test_replication_dirty_no_duplicates();
    test_replication_collect_delta();
    test_replication_apply_delta();
    test_replication_multiple_rules();
    test_replication_delta_roundtrip();
    test_replication_delta_every_tick();

    // Asset Browser
    std::cout << "\n--- Asset Browser ---" << std::endl;
    test_asset_browser_empty();
    test_asset_browser_with_assets();
    test_asset_browser_filter();
    test_asset_browser_sort_modes();
    test_asset_browser_select_asset();

    // Asset Cooker
    std::cout << "\n--- Asset Cooker ---" << std::endl;
    test_cooker_default_settings();
    test_cooker_set_output_dir();
    test_cooker_cook_single_asset();
    test_cooker_source_not_found();
    test_cooker_cook_all();
    test_cooker_strip_editor_data();
    test_cooker_clear_log();
    test_cooker_progress_callback();

    // Graph Editor Panel
    std::cout << "\n--- Graph Editor Panel ---" << std::endl;
    test_graph_panel_no_graph();
    test_graph_panel_with_nodes();
    test_graph_panel_select_node();
    test_graph_panel_compile_and_preview();
    test_graph_panel_summary();
    test_graph_panel_node_info();

    // Asset Graph Assistant
    std::cout << "\n--- Asset Graph Assistant ---" << std::endl;
    test_assistant_suggest_empty();
    test_assistant_suggest_with_executor();
    test_assistant_explain_no_executor();
    test_assistant_explain_with_nodes();
    test_assistant_mutate_no_executor();
    test_assistant_mutate_intensity();

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

    // Render and Platform
    std::cout << "\n--- Render and Platform ---" << std::endl;
    test_render_api_enum();
    test_null_renderer();
    test_gl_renderer_viewport();
    test_vulkan_renderer_viewport();
    test_ui_manager_set_renderer();
    test_ui_manager_render_empty();
    test_ui_manager_render_widgets();
    test_ui_manager_render_null_renderer();
    test_engine_config_render_fields();
    test_engine_headless_no_window();
    test_engine_server_no_window();
    test_platform_window_event_type();
    test_platform_window_config();

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
    test_engine_frame_pacing_default_enabled();
    test_engine_frame_pacing_prevents_spin();
    test_engine_ui_update_in_loop();

    // Logger
    std::cout << "\n--- Logger ---" << std::endl;
    test_logger_creates_log_directory();
    test_logger_writes_to_file();

    // Console
    std::cout << "\n--- Console ---" << std::endl;
    test_console_spawn_entity();
    test_console_ecs_dump();
    test_console_set_tickrate();
    test_console_net_mode();
    test_console_help();
    test_console_unknown_command();

    // Interaction
    std::cout << "\n--- Interaction System ---" << std::endl;
    test_utterance_creation();
    test_intent_creation();
    test_rule_intent_resolver();
    test_interaction_system_uninitialized();
    test_interaction_system_no_match();
    test_interaction_system_full_pipeline();

    // Interaction (graph-based)
    std::cout << "\n--- Interaction (Graph-Based) ---" << std::endl;
    test_intent_registry_dispatch();
    test_intent_registry_missing();
    test_intent_registry_has_handler();
    test_pattern_resolver();
    test_pattern_resolver_no_match();
    test_pattern_resolver_case_insensitive();

    // Diplomacy
    std::cout << "\n--- AI Diplomacy ---" << std::endl;
    test_faction_request_neutral();
    test_faction_request_trusted();
    test_faction_request_hostile();
    test_faction_threaten_increases_hostility();
    test_faction_unknown_intent();
    test_faction_personality_affects_hostility();

    // Project
    std::cout << "\n--- Project System ---" << std::endl;
    test_project_load_valid();
    test_project_load_minimal();
    test_project_load_missing_file();
    test_project_invalid_schema();
    test_project_missing_name();
    test_project_missing_name_field();
    test_project_invalid_version();
    test_project_schema_validation();

    // Command
    std::cout << "\n--- Command System ---" << std::endl;
    test_command_execute();
    test_command_undo();
    test_command_redo();
    test_command_redo_cleared_on_execute();
    test_command_clear();
    test_command_last_executed();
    test_command_multiple_undo_redo();

    // WorldGraph
    std::cout << "\n--- World Graph ---" << std::endl;
    test_worldgraph_add_nodes();
    test_worldgraph_remove_node();
    test_worldgraph_compile_empty();
    test_worldgraph_compile_single_node();
    test_worldgraph_compile_chain();
    test_worldgraph_deterministic();
    test_worldgraph_clamp_node();

    // Voice Commands
    std::cout << "\n--- Voice Commands ---" << std::endl;
    test_voice_register_command();
    test_voice_match_command();
    test_voice_no_match();
    test_voice_get_by_context();

    // Plugin System
    std::cout << "\n--- Plugin System ---" << std::endl;
    test_plugin_validate_valid();
    test_plugin_validate_no_name();
    test_plugin_validate_nondeterministic();
    test_plugin_validate_incompatible_version();
    test_plugin_registry_register();
    test_plugin_registry_find();
    test_plugin_registry_unregister();
    test_plugin_registry_get_by_type();

    // Heightfield
    std::cout << "\n--- Heightfield Mesher ---" << std::endl;
    test_heightfield_at();
    test_heightfield_mesh_generation();
    test_heightfield_mesh_lod();

    // StrategyGraph
    std::cout << "\n--- Strategy Graph ---" << std::endl;
    test_strategygraph_add_nodes();
    test_strategygraph_remove_node();
    test_strategygraph_compile_empty();
    test_strategygraph_compile_single_node();
    test_strategygraph_compile_chain();
    test_strategygraph_execute();
    test_strategygraph_deterministic();
    test_strategygraph_objective_score();
    test_strategygraph_read_ai_signal();
    test_strategygraph_emit_action();

    // Server Rules
    std::cout << "\n--- Server Rules ---" << std::endl;
    test_server_rules_register();
    test_server_rules_get_value();
    test_server_rules_set_multiplier();
    test_server_rules_clamp_multiplier();
    test_server_rules_missing_rule();
    test_server_rules_hot_reload_flag();
    test_server_rules_clear();

    // ConversationGraph
    std::cout << "\n--- Conversation Graph ---" << std::endl;
    test_conversation_add_nodes();
    test_conversation_remove_node();
    test_conversation_compile_empty();
    test_conversation_dialogue_node();
    test_conversation_chain();
    test_conversation_relationship_delta();
    test_conversation_player_choice();

    // AI Signals
    std::cout << "\n--- AI Signals ---" << std::endl;
    test_ai_signal_register();
    test_ai_signal_read_write();
    test_ai_signal_missing();
    test_ai_signal_namespace();
    test_ai_signal_clear();

    // AI Memory
    std::cout << "\n--- AI Memory ---" << std::endl;
    test_ai_memory_store_recall();
    test_ai_memory_has();
    test_ai_memory_forget();
    test_ai_memory_decay();
    test_ai_memory_permanent();
    test_ai_memory_clear();

    // Relationship Model
    std::cout << "\n--- Relationship Model ---" << std::endl;
    test_relationship_set_get();
    test_relationship_default_zero();
    test_relationship_modify();
    test_relationship_has();
    test_relationship_remove();
    test_relationship_clear();

    // Asset Graph
    std::cout << "\n--- Asset Graph ---" << std::endl;
    test_asset_context();
    test_asset_graph_executor_empty();
    test_asset_graph_executor_single_node();
    test_asset_graph_executor_multiple_nodes();
    test_damage_state();

    // Mod System
    std::cout << "\n--- Mod System ---" << std::endl;
    test_mod_register_graph();
    test_mod_register_archetype();
    test_mod_registry_mixed();

    // Editor Assistant
    std::cout << "\n--- Editor Assistant ---" << std::endl;
    test_editor_assistant_explain_performance();
    test_editor_assistant_explain_node();
    test_editor_assistant_unknown();
    test_interaction_debugger_record();
    test_interaction_debugger_clear();

    // Input
    std::cout << "\n--- Input System ---" << std::endl;
    test_input_bind_action();
    test_input_unbind_action();
    test_input_press_release();
    test_input_axis();
    test_input_callback();
    test_input_unbound_action();

    // Camera
    std::cout << "\n--- Camera System ---" << std::endl;
    test_camera_default_state();
    test_camera_set_mode();
    test_camera_position();
    test_camera_fov_clamp();
    test_camera_orbit();
    test_camera_movement();
    test_camera_pitch_clamp();

    // Physics
    std::cout << "\n--- Physics ---" << std::endl;
    test_physics_create_body();
    test_physics_destroy_body();
    test_physics_gravity();
    test_physics_static_body();
    test_physics_apply_force();
    test_physics_collision_detection();

    // Audio
    std::cout << "\n--- Audio System ---" << std::endl;
    test_audio_load_sound();
    test_audio_unload_sound();
    test_audio_play_pause_stop();
    test_audio_volume();
    test_audio_master_volume();
    test_audio_looping();

    // Gameplay Mechanics
    std::cout << "\n--- Gameplay Mechanics ---" << std::endl;
    test_mechanic_register();
    test_mechanic_unregister();
    test_mechanic_find_by_name();
    test_mechanic_params();
    test_mechanic_get_by_type();

    // Skill Tree
    std::cout << "\n--- Skill Tree ---" << std::endl;
    test_skill_add_node();
    test_skill_unlock();
    test_skill_prerequisites();
    test_skill_effects();
    test_skill_tree_chain();

    // Weapon Graph
    std::cout << "\n--- Weapon Graph ---" << std::endl;
    test_weapongraph_add_nodes();
    test_weapongraph_remove_node();
    test_weapongraph_compile_empty();
    test_weapongraph_compile_single_node();
    test_weapongraph_compile_chain();
    test_weapongraph_execute();
    test_weapongraph_wear_degrades_stats();
    test_weapongraph_deterministic();

    // Character Graph
    std::cout << "\n--- Character Graph ---" << std::endl;
    test_charactergraph_add_nodes();
    test_charactergraph_remove_node();
    test_charactergraph_compile_empty();
    test_charactergraph_compile_single_node();
    test_charactergraph_compile_chain();
    test_charactergraph_execute();
    test_charactergraph_deterministic();

    // Animation Graph
    std::cout << "\n--- Animation Graph ---" << std::endl;
    test_animationgraph_add_nodes();
    test_animationgraph_remove_node();
    test_animationgraph_compile_empty();
    test_animationgraph_compile_single_node();
    test_animationgraph_compile_chain();
    test_animationgraph_execute();
    test_animationgraph_modifier();
    test_animationgraph_deterministic();

    // Tile Graph
    std::cout << "\n--- Tile Graph ---" << std::endl;
    test_tilegraph_add_nodes();
    test_tilegraph_remove_node();
    test_tilegraph_compile_empty();
    test_tilegraph_compile_single_node();
    test_tilegraph_compile_chain();
    test_tilegraph_execute();
    test_tilegraph_deterministic();

    // Sound Graph
    std::cout << "\n--- Sound Graph ---" << std::endl;
    test_soundgraph_add_nodes();
    test_soundgraph_remove_node();
    test_soundgraph_compile_empty();
    test_soundgraph_compile_single_node();
    test_soundgraph_compile_chain();
    test_soundgraph_execute();
    test_soundgraph_deterministic();

    // Behavior Graph
    std::cout << "\n--- Behavior Graph ---" << std::endl;
    test_behaviorgraph_add_nodes();
    test_behaviorgraph_remove_node();
    test_behaviorgraph_compile_empty();
    test_behaviorgraph_compile_single_node();
    test_behaviorgraph_compile_chain();
    test_behaviorgraph_execute();
    test_behaviorgraph_deterministic();

    // UI Screen
    std::cout << "\n--- UI Screen ---" << std::endl;
    test_ui_add_widget();
    test_ui_visibility();
    test_ui_parent_child();
    test_ui_remove_widget();

    // Game Flow Graph
    std::cout << "\n--- Game Flow Graph ---" << std::endl;
    test_gameflowgraph_add_nodes();
    test_gameflowgraph_remove_node();
    test_gameflowgraph_compile_empty();
    test_gameflowgraph_compile_single_node();
    test_gameflowgraph_compile_chain();
    test_gameflowgraph_execute();
    test_gameflowgraph_deterministic();

    // Flow IR
    std::cout << "\n--- Flow IR ---" << std::endl;
    test_flow_ir_default_values();
    test_flow_ir_add_nodes();
    test_flow_ir_add_edges();
    test_flow_ir_to_json();
    test_flow_ir_roundtrip();
    test_flow_ir_validate_valid();
    test_flow_ir_validate_invalid();
    test_flow_ir_migrate_v1();

    // Flow Codegen
    std::cout << "\n--- Flow Codegen ---" << std::endl;
    test_flow_codegen_nonempty();
    test_flow_codegen_function_signature();
    test_flow_codegen_empty_graph();
    test_flow_codegen_node_properties();
    test_flow_codegen_edge_wiring();
    test_flow_codegen_all_node_types();
    test_flow_codegen_header_comment();

    // Flow Debugger
    std::cout << "\n--- Flow Debugger ---" << std::endl;
    test_debugger_initial_state();
    test_debugger_add_breakpoint();
    test_debugger_remove_breakpoint();
    test_debugger_clear_breakpoints();
    test_debugger_execute_no_breakpoints();
    test_debugger_breakpoint_pause();
    test_debugger_step_node();

    // Shader IR
    std::cout << "\n--- Shader IR ---" << std::endl;
    test_shader_module_defaults();
    test_shader_passthrough_vertex();
    test_shader_solid_color_fragment();
    test_shader_validate_valid();
    test_shader_validate_empty();
    test_shader_hash_deterministic();
    test_shader_hash_different();
    test_shader_serialize_roundtrip();

    // Build Manifest
    std::cout << "\n--- Build Manifest ---" << std::endl;
    test_manifest_defaults();
    test_manifest_add_artifact();
    test_manifest_find_artifact();
    test_manifest_find_missing();
    test_manifest_validate_valid();
    test_manifest_validate_missing_fields();
    test_manifest_to_json();
    test_manifest_roundtrip();
    test_manifest_verify_hash();

    // UI Graph
    std::cout << "\n--- UI Graph ---" << std::endl;
    test_uigraph_add_nodes();
    test_uigraph_remove_node();
    test_uigraph_compile_empty();
    test_uigraph_compile_single_node();
    test_uigraph_compile_chain();
    test_uigraph_execute();
    test_uigraph_deterministic();

    // UI Command Bus
    std::cout << "\n--- UI Command Bus ---" << std::endl;
    test_command_bus_enqueue();
    test_command_bus_drain();
    test_command_bus_clear();
    test_command_bus_dispatch();
    test_command_bus_dispatch_unhandled();
    test_command_bus_move_enqueue();

    // UI Manager
    std::cout << "\n--- UI Manager ---" << std::endl;
    test_ui_manager_init();
    test_ui_manager_context_switch();
    test_ui_manager_screen_access();
    test_ui_manager_graph_access();
    test_ui_manager_update();
    test_ui_manager_command_bus();
    test_ui_manager_shutdown();
    test_ui_manager_server_context();

    // UI Layout Solver
    std::cout << "\n--- UI Layout Solver ---" << std::endl;
    test_layout_solver_single_entry();
    test_layout_solver_horizontal_split();
    test_layout_solver_vertical_split();
    test_layout_solver_weighted();
    test_layout_solver_min_size_respect();
    test_layout_solver_clear();
    test_layout_solver_deterministic();
    test_layout_solver_offset();

    // UI Nodes Extended
    std::cout << "\n--- UI Nodes Extended ---" << std::endl;
    test_slotgrid_node_defaults();
    test_inputfield_node_defaults();
    test_inputfield_node_with_text();
    test_slotgrid_node_metadata();
    test_inputfield_node_metadata();

    // Schema Validator
    std::cout << "\n--- Schema Validator ---" << std::endl;
    test_schema_valid();
    test_schema_empty_id();
    test_schema_zero_version();
    test_schema_duplicate_node_id();
    test_schema_empty_node_id();
    test_schema_no_nodes();

    // Graph Cache
    std::cout << "\n--- Graph Cache ---" << std::endl;
    test_cache_store_and_get();
    test_cache_has();
    test_cache_invalidate();
    test_cache_invalidate_all();
    test_cache_evict_before();
    test_cache_hash_key();

    // Graph Serialization
    std::cout << "\n--- Graph Serialization ---" << std::endl;
    test_json_builder_object();
    test_json_builder_array();
    test_json_reader_parse();
    test_json_reader_array();
    test_json_roundtrip();

    // ECS Serialization
    std::cout << "\n--- ECS Serialization ---" << std::endl;
    test_ecs_serialize_empty_world();
    test_ecs_serialize_entities_no_components();
    test_ecs_serialize_roundtrip();
    test_ecs_serialize_preserves_next_id();
    test_ecs_deserialize_clears_existing();
    test_ecs_deserialize_invalid_data();
    test_ecs_has_serializer();

    // Snapshot / Rollback
    std::cout << "\n--- Snapshot / Rollback ---" << std::endl;
    test_snapshot_saves_ecs_state();
    test_rollback_restores_ecs_state();
    test_rollback_removes_future_snapshots();
    test_snapshot_without_world();
    test_rollback_with_multiple_entities();
    test_record_and_replay_input();
    test_replay_applies_input_frames();

    // ECS Inspector
    std::cout << "\n--- ECS Inspector ---" << std::endl;
    test_inspector_empty_world();
    test_inspector_entities_with_components();
    test_inspector_select_entity();
    test_inspector_refreshes_on_draw();
    test_inspector_state_blocks_empty();
    test_inspector_state_blocks_with_world_state();

    // Story
    std::cout << "\n--- Story System ---" << std::endl;
    test_story_add_node();
    test_story_connections();
    test_story_tags();
    test_story_get_by_type();

    // Build Profiles
    std::cout << "\n--- Build Profiles ---" << std::endl;
    test_profile_default_debug();
    test_profile_default_development();
    test_profile_default_release();
    test_profile_name();
    test_profile_set_active();
    test_profile_set_custom_config();
    test_profile_overrides();
    test_profile_override_missing_key();

    // Mod Loader
    std::cout << "\n--- Mod Loader ---" << std::endl;
    test_mod_loader_register();
    test_mod_loader_register_invalid();
    test_mod_loader_register_duplicate();
    test_mod_loader_unregister();
    test_mod_loader_enable_disable();
    test_mod_loader_get_mod();
    test_mod_loader_dependencies();
    test_mod_loader_scan_directory();
    test_mod_loader_clear();

    // Platform Target
    std::cout << "\n--- Platform Target ---" << std::endl;
    test_platform_default_windows();
    test_platform_default_linux();
    test_platform_default_macos();
    test_platform_default_web();
    test_platform_name();
    test_platform_set_target();
    test_platform_custom_config();
    test_platform_module_exclusion();
    test_platform_host();
    test_platform_supported_list();

    // Profiler Panel
    std::cout << "\n--- Profiler Panel ---" << std::endl;
    test_profiler_empty();
    test_profiler_record_frame();
    test_profiler_average_and_peak();
    test_profiler_max_history();
    test_profiler_system_metrics();
    test_profiler_pause();
    test_profiler_clear();

    // Replay Recorder
    std::cout << "\n--- Replay Recorder ---" << std::endl;
    test_replay_initial_state();
    test_replay_start_recording();
    test_replay_record_frames();
    test_replay_stop_recording();
    test_replay_save_and_load();
    test_replay_load_invalid();
    test_replay_clear();

    // State Hasher
    std::cout << "\n--- State Hasher ---" << std::endl;
    test_hasher_initial_state();
    test_hasher_advance_tick();
    test_hasher_deterministic();
    test_hasher_chaining();
    test_hasher_divergence_detection();
    test_hasher_different_seeds();
    test_hasher_empty_data();
    test_hasher_raw_pointer_api();
    test_replay_record_with_hash();
    test_replay_save_load_with_hash();
    test_replay_default_hash_zero();
    test_hash_combine_deterministic();

    // Visual Diff
    std::cout << "\n--- Visual Diff ---" << std::endl;
    test_diff_identical();
    test_diff_added();
    test_diff_removed();
    test_diff_modified();
    test_diff_empty();
    test_diff_compare_graphs();
    test_diff_summarize();

    // Crash Handler
    std::cout << "\n--- Crash Handler ---" << std::endl;
    test_crash_breadcrumb();
    test_crash_breadcrumb_limit();
    test_crash_clear_breadcrumbs();
    test_crash_generate_report();
    test_crash_save_report();
    test_crash_callback();
    test_crash_report_to_file();

    // Net Inspector Panel
    std::cout << "\n--- Net Inspector Panel ---" << std::endl;
    test_net_inspector_standalone();
    test_net_inspector_server_with_peers();
    test_net_inspector_mode_names();

    // Game Packager Panel
    std::cout << "\n--- Game Packager Panel ---" << std::endl;
    test_game_packager_defaults();
    test_game_packager_summary();
    test_game_packager_modify_settings();

    // Game Module System
    std::cout << "\n--- Game Module System ---" << std::endl;
    test_game_module_describe();
    test_game_module_lifecycle();
    test_module_loader_static();
    test_module_loader_already_loaded();
    test_module_loader_not_found();

    // Atlas Gameplay
    std::cout << "\n--- Atlas Gameplay ---" << std::endl;
    test_faction_register();
    test_faction_relations();
    test_faction_clear();
    test_combat_register_unit();
    test_combat_resolve();
    test_combat_clear();
    test_economy_register();
    test_economy_transactions();
    test_economy_capacity();
    test_economy_clear();

    // EveOffline Module
    std::cout << "\n--- EveOffline Module ---" << std::endl;
    test_eveoffline_describe();
    test_eveoffline_register_types();
    test_eveoffline_replication_rules();
    test_eveoffline_server_rules();
    test_eveoffline_lifecycle();
    test_eveoffline_factory();

    // Arena2D Module
    std::cout << "\n--- Arena2D Module ---" << std::endl;
    test_arena2d_describe();
    test_arena2d_register_types();
    test_arena2d_replication_rules();
    test_arena2d_server_rules();
    test_arena2d_lifecycle();
    test_arena2d_factory();

    // Replay Timeline Panel
    std::cout << "\n--- Replay Timeline Panel ---" << std::endl;
    test_replay_timeline_empty();
    test_replay_timeline_load();
    test_replay_timeline_scrub();
    test_replay_timeline_markers();
    test_replay_timeline_compare_identical();
    test_replay_timeline_compare_divergent();
    test_replay_timeline_inject_input();
    test_replay_timeline_branch();

    // Replay Divergence Inspector
    std::cout << "\n--- Replay Divergence Inspector ---" << std::endl;
    test_divergence_no_mismatch();
    test_divergence_hash_mismatch();
    test_divergence_frame_comparison();
    test_divergence_single_frame_check();
    test_divergence_report_format();
    test_divergence_history();
    test_divergence_severity();

    // Deterministic Allocator
    std::cout << "\n--- Deterministic Allocator ---" << std::endl;
    test_allocator_create();
    test_allocator_single_alloc();
    test_allocator_multiple_allocs();
    test_allocator_dealloc();
    test_allocator_reuse_space();
    test_allocator_stable_ids();
    test_allocator_reset();
    test_allocator_deterministic();

    // Permission Manager
    std::cout << "\n--- Permission Manager ---" << std::endl;
    test_permission_defaults();
    test_permission_register_user();
    test_permission_tier_viewonly();
    test_permission_tier_developer();
    test_permission_tier_admin();
    test_permission_grant_custom();
    test_permission_revoke();
    test_permission_user_management();

    // HUD Overlay
    std::cout << "\n--- HUD Overlay ---" << std::endl;
    test_hud_defaults();
    test_hud_tick_display();
    test_hud_warnings();
    test_hud_critical_warning();
    test_hud_time_controls();
    test_hud_step_forward();
    test_hud_determinism_status();
    test_hud_visibility();
    test_hud_world_dilation();
    test_hud_world_paused();

    // Time Model
    std::cout << "\n--- Time Model ---" << std::endl;
    test_time_model_defaults();
    test_time_model_set_tick_rate();
    test_time_model_advance_tick();
    test_time_model_world_dilation();
    test_time_model_world_pause();
    test_time_model_presentation();
    test_time_model_reset();
    test_simulation_time_elapsed();

    // World State
    std::cout << "\n--- World State ---" << std::endl;
    test_world_state_register_block();
    test_world_state_find_block();
    test_world_state_snapshot();
    test_world_state_snapshot_deterministic();
    test_world_state_push_and_retrieve();
    test_world_state_max_snapshots();
    test_world_state_prune();
    test_world_state_clear();
    test_world_state_derived_rebuild();

    // Save System
    std::cout << "\n--- Save System ---" << std::endl;
    test_save_system_save_and_load();
    test_save_system_hash_integrity();
    test_save_system_file_not_found();
    test_save_system_validate();
    test_save_system_invalid_format();
    test_save_system_empty_data();
    test_save_system_clear();

    // Engine Integration
    std::cout << "\n--- Engine Integration ---" << std::endl;
    test_engine_time_model_initialized();
    test_engine_time_model_tick_rate_matches();
    test_engine_time_model_world_time_advances();
    test_engine_world_state_snapshots_in_server();
    test_engine_world_state_snapshot_at_tick();
    test_engine_save_system_accessible();
    test_console_save_command();
    test_console_load_command();
    test_console_load_file_not_found();
    test_console_save_no_path();
    test_console_time_info();
    test_ai_behavior_deterministic_across_runs();
    test_ai_relationship_determinism_across_save_load();

    // Next Tasks
    std::cout << "\n--- Next Tasks ---" << std::endl;
    test_engine_autosave_creates_file();
    test_time_model_set_tick();
    test_replay_save_point_mark();
    test_replay_save_point_multiple();
    test_replay_save_point_serialization();
    test_replay_start_from_save();
    test_engine_load_and_replay();
    test_divergence_export_report();
    test_engine_rollback_to_tick();
    test_net_broadcast_save_tick();
    test_state_hash_diff_panel_no_divergence();
    test_state_hash_diff_panel_with_divergence();
    test_hash_ladder_save_load_continuity();
    test_engine_replay_from_save();
    test_engine_replay_from_save_bad_save();
    test_engine_replay_from_save_bad_replay();
    test_state_hash_diff_panel_component_breakdown();
    test_state_hash_diff_panel_no_component_divergence();

    // Next Tasks Phase 2
    std::cout << "\n--- Next Tasks Phase 2 ---" << std::endl;
    test_behaviorgraph_serialize_state();
    test_behaviorgraph_serialize_empty();
    test_partial_save_and_load();
    test_partial_save_hash_integrity();
    test_partial_save_empty_chunks();
    test_save_file_browser_scan();
    test_save_file_browser_empty_dir();
    test_save_file_browser_select();
    test_hash_ladder_comparison();
    test_hash_ladder_comparison_no_divergence();
    test_divergence_detail();
    test_divergence_detail_no_divergence();
    test_replay_auto_divergence_callback();
    test_replay_no_divergence_callback();
    test_detailed_divergence_report();
    test_detailed_divergence_report_no_divergence();

    // GUI DSL Parser
    std::cout << "\n--- GUI DSL Parser ---" << std::endl;
    test_dsl_parse_simple_layout();
    test_dsl_parse_split();
    test_dsl_parse_nested_splits();
    test_dsl_parse_dock();
    test_dsl_parse_tabs();
    test_dsl_parse_full_example();
    test_dsl_parse_comments();
    test_dsl_parse_error_missing_layout();
    test_dsl_parse_error_unterminated_string();
    test_dsl_parse_error_unknown_statement();
    test_dsl_version();
    test_dsl_parse_dock_targets();

    // GUI Input Recorder
    std::cout << "\n--- GUI Input Recorder ---" << std::endl;
    test_recorder_initial_state();
    test_recorder_start_stop_recording();
    test_recorder_record_events();
    test_recorder_record_only_when_recording();
    test_recorder_playback();
    test_recorder_save_load();
    test_recorder_load_invalid_file();
    test_recorder_load_nonexistent_file();

    // Headless GUI
    std::cout << "\n--- Headless GUI ---" << std::endl;
    test_headless_gui_init();
    test_headless_gui_available_commands();
    test_headless_gui_widget_count();
    test_headless_gui_widget_info();
    test_headless_gui_widget_info_not_found();
    test_headless_gui_unknown_command();
    test_headless_gui_empty_command();
    test_headless_gui_help();
    test_headless_gui_status();
    test_headless_gui_enqueue();
    test_headless_gui_custom_command();
    test_headless_gui_quoted_args();

    // Job Execution Tracer
    std::cout << "\n--- Job Execution Tracer ---" << std::endl;
    test_job_tracer_empty();
    test_job_tracer_single_tick();
    test_job_tracer_multiple_ticks();
    test_job_tracer_deterministic_hash();
    test_job_tracer_different_order_different_hash();
    test_job_tracer_compare_order();
    test_job_tracer_max_history();
    test_job_tracer_clear();
    test_job_trace_panel_no_tracer();
    test_job_trace_panel_consistent();
    test_job_trace_panel_mismatch();
    test_job_trace_panel_entries_at_tick();

    // Component Category
    std::cout << "\n--- Component Category ---" << std::endl;
    test_component_category_defaults();
    test_component_category_simulated();
    test_component_category_presentation();
    test_component_category_debug();
    test_component_category_derived();
    test_component_category_assert_simulated();
    test_component_category_assert_not_presentation();
    test_component_category_runtime_query();

    // Include Firewall
    std::cout << "\n--- Include Firewall ---" << std::endl;
    test_include_firewall_sim_no_render();
    test_include_firewall_contract_no_render();

    // FP Drift Detector
    std::cout << "\n--- FP Drift Detector ---" << std::endl;
    test_fp_drift_no_drift();
    test_fp_drift_detected();
    test_fp_drift_system_comparison();
    test_fp_drift_format_report();
    test_fp_drift_export_report();
    test_fp_drift_add_system_hashes();

    // Asset Import Pipeline
    std::cout << "\n--- Asset Import Pipeline ---" << std::endl;
    test_asset_import_registry_empty();
    test_asset_import_registry_register();
    test_asset_import_mesh_can_import();
    test_asset_import_texture_can_import();
    test_asset_import_audio_can_import();
    test_asset_import_font_can_import();
    test_asset_import_sprite_can_import();
    test_asset_import_find_importer();
    test_asset_import_file_roundtrip();
    test_asset_import_file_not_found();
    test_asset_import_registry_import();

    // Marketplace Importer
    std::cout << "\n--- Marketplace Importer ---" << std::endl;
    test_marketplace_registry();
    test_itch_io_importer();
    test_marketplace_metadata();
    test_marketplace_import_options();
    test_unreal_marketplace_importer();
    test_unity_assetstore_importer();

    // Asset Validator
    std::cout << "\n--- Asset Validator ---" << std::endl;
    test_asset_validator_validate_valid_file();
    test_asset_validator_validate_corrupt_file();
    test_asset_validator_validate_header_bad_magic();
    test_asset_validator_migration_path();
    test_asset_validator_dependencies();
    test_asset_validator_circular_dependency();
    test_asset_validator_no_circular_dependency();
    test_asset_validator_compute_file_hash();

    // Golden Replays
    std::cout << "\n--- Golden Replays ---" << std::endl;
    run_golden_replay_tests();

    // Enforcement
    std::cout << "\n--- Enforcement ---" << std::endl;
    test_tick_rate_lock();
    test_tick_rate_set_before_lock();
    test_engine_locks_tick_rate_on_run();
    test_engine_system_order();
    test_engine_system_order_deterministic();
    test_net_packet_schema_version();
    test_asset_lock_and_check();
    test_asset_lock_multiple();
    test_determinism_config_exists();

    // Lockdown Phase 2
    std::cout << "\n--- Lockdown Phase 2 ---" << std::endl;
    test_rollback_and_verify_success();
    test_rollback_and_verify_bad_ticks();
    test_asset_schema_lock();
    test_asset_schema_lock_default();
    test_layout_hash_deterministic();
    test_layout_hash_differs_on_change();
    test_layout_hash_empty();
    test_mutation_ownership_register();
    test_mutation_ownership_exclusive();
    test_mutation_can_mutate();
    test_mutation_owned_components();
    test_mutation_owner_of_unregistered();

    // Lockdown Phase 3
    std::cout << "\n--- Lockdown Phase 3 ---" << std::endl;
    test_auditor_empty_passes();
    test_auditor_system_no_state_fails();
    test_auditor_system_with_state_passes();
    test_auditor_multiple_systems();
    test_auditor_duplicate_register();
    test_auditor_declared_blocks();
    test_auditor_unregistered_declaration_warning();
    test_auditor_clear();
    test_server_validator_empty_manifest();
    test_server_validator_register_and_check();
    test_server_validator_hash_match();
    test_server_validator_hash_mismatch();
    test_server_validator_unknown_asset();
    test_server_validator_file_validation();
    test_server_validator_file_not_found();
    test_server_validator_manifest();
    test_verify_save_load_determinism();
    test_desync_reproducer_output_dir();
    test_desync_reproducer_capture();
    test_desync_reproducer_generate_command();
    test_desync_reproducer_multiple_captures();

    // GUI Completion
    std::cout << "\n--- GUI Completion ---" << std::endl;
    test_editor_theme_dark_defaults();
    test_editor_theme_custom_values();
    test_panel_closable_by_default();
    test_panel_set_non_closable();
    test_layout_close_closable_panel();
    test_layout_close_non_closable_panel();
    test_layout_close_unknown_panel();
    test_layout_find_panel();
    test_main_panels_non_closable();
    test_attach_init();
    test_attach_standalone();
    test_attach_live_client();
    test_attach_live_client_missing_host();
    test_attach_headless_server();
    test_attach_replay();
    test_attach_replay_missing_path();
    test_attach_disconnect();
    test_default_editor_dsl_parses();
    test_default_editor_dsl_has_panels();

    // Proof Viewer Panel
    std::cout << "\n--- Proof Viewer Panel ---" << std::endl;
    test_proof_viewer_init();
    test_proof_viewer_load_specs();
    test_proof_viewer_select_spec();
    test_proof_viewer_tokenize();
    test_proof_viewer_invariants();
    test_proof_viewer_verification_status();
    test_proof_viewer_line_count();
    test_proof_viewer_empty_select();

    // Replay Proof Exporter
    std::cout << "\n--- Replay Proof Exporter ---" << std::endl;
    test_proof_exporter_empty();
    test_proof_exporter_add_entries();
    test_proof_exporter_validate_chain_valid();
    test_proof_exporter_validate_chain_invalid();
    test_proof_exporter_export_tla();
    test_proof_exporter_export_json();
    test_proof_exporter_export_csv();
    test_proof_exporter_save_points();
    test_proof_exporter_format_entry();
    test_proof_exporter_clear();

    // CI Dashboard Panel
    std::cout << "\n--- CI Dashboard Panel ---" << std::endl;
    test_ci_dashboard_init();
    test_ci_dashboard_start_run();
    test_ci_dashboard_add_checks();
    test_ci_dashboard_complete_run_passed();
    test_ci_dashboard_complete_run_failed();
    test_ci_dashboard_partial_failure();
    test_ci_dashboard_pass_rate();
    test_ci_dashboard_history();
    test_ci_dashboard_max_history();
    test_ci_dashboard_summary();
    test_ci_dashboard_clear();

    // TLC Model Checker
    std::cout << "\n--- TLC Model Checker ---" << std::endl;
    test_tlc_register_spec();
    test_tlc_register_multiple_specs();
    test_tlc_register_bad_path();
    test_tlc_unregister_spec();
    test_tlc_get_spec();
    test_tlc_extract_invariants_replay();
    test_tlc_extract_invariants_ecs();
    test_tlc_extract_invariants_layout();
    test_tlc_validate_spec();
    test_tlc_check_spec();
    test_tlc_check_spec_not_found();
    test_tlc_check_all();
    test_tlc_ci_check_pass();
    test_tlc_format_report();
    test_tlc_export_report();
    test_tlc_path_management();
    test_tlc_extract_invariants_static();

    std::cout << "\n=== All tests passed! ===" << std::endl;
    return 0;
}
