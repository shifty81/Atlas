#include "../engine/core/Engine.h"
#include "../engine/sim/TimeModel.h"
#include "../engine/sim/ReplayRecorder.h"
#include "../engine/sim/ReplayDivergenceInspector.h"
#include "../engine/sim/SaveSystem.h"
#include <iostream>
#include <cassert>
#include <cstdio>
#include <fstream>
#include <string>

using namespace atlas;
using namespace atlas::sim;

// --- Autosave ---

void test_engine_autosave_creates_file() {
    const std::string path = "/tmp/atlas_autosave_test.asav";
    std::remove(path.c_str());

    EngineConfig cfg;
    cfg.mode = EngineMode::Server;
    cfg.tickRate = 60;
    cfg.maxTicks = 10;
    cfg.autosaveInterval = 5;
    cfg.autosavePath = path;

    Engine engine(cfg);
    engine.InitCore();
    engine.InitECS();
    engine.InitNetworking();
    engine.GetScheduler().SetFramePacing(false);

    engine.GetWorld().CreateEntity();
    engine.Run();

    SaveSystem validator;
    auto result = validator.Validate(path);
    assert(result == SaveResult::Success);

    std::remove(path.c_str());
    std::cout << "[PASS] test_engine_autosave_creates_file" << std::endl;
}

// --- TimeModel SetTick ---

void test_time_model_set_tick() {
    TimeModel tm;
    tm.SetTickRate(60);

    for (int i = 0; i < 5; i++) tm.AdvanceTick();
    assert(tm.Context().sim.tick == 5);

    tm.SetTick(100);
    assert(tm.Context().sim.tick == 100);

    std::cout << "[PASS] test_time_model_set_tick" << std::endl;
}

// --- Replay save-point markers ---

void test_replay_save_point_mark() {
    ReplayRecorder recorder;
    recorder.StartRecording(60);

    for (uint32_t t = 1; t <= 5; ++t)
        recorder.RecordFrame(t, {});

    recorder.MarkSavePoint(3);
    auto pts = recorder.SavePoints();
    assert(pts.size() == 1);
    assert(pts[0] == 3);

    std::cout << "[PASS] test_replay_save_point_mark" << std::endl;
}

void test_replay_save_point_multiple() {
    ReplayRecorder recorder;
    recorder.StartRecording(60);

    for (uint32_t t = 1; t <= 5; ++t)
        recorder.RecordFrame(t, {});

    recorder.MarkSavePoint(2);
    recorder.MarkSavePoint(4);
    auto pts = recorder.SavePoints();
    assert(pts.size() == 2);
    assert(pts[0] == 2);
    assert(pts[1] == 4);

    std::cout << "[PASS] test_replay_save_point_multiple" << std::endl;
}

void test_replay_save_point_serialization() {
    const std::string path = "/tmp/atlas_replay_sp_test.rply";
    std::remove(path.c_str());

    {
        ReplayRecorder recorder;
        recorder.StartRecording(60, 99);
        for (uint32_t t = 1; t <= 5; ++t)
            recorder.RecordFrame(t, {static_cast<uint8_t>(t)});
        recorder.MarkSavePoint(3);
        recorder.StopRecording();
        assert(recorder.SaveReplay(path));
    }

    {
        ReplayRecorder loader;
        assert(loader.LoadReplay(path));
        assert(loader.Header().version == 3);
        auto pts = loader.SavePoints();
        assert(pts.size() == 1);
        assert(pts[0] == 3);
    }

    std::remove(path.c_str());
    std::cout << "[PASS] test_replay_save_point_serialization" << std::endl;
}

void test_replay_start_from_save() {
    ReplayRecorder recorder;
    recorder.StartFromSave(100, 60, 42);

    assert(recorder.State() == ReplayState::Recording);
    assert(recorder.Header().tickRate == 60);
    assert(recorder.Header().seed == 42);

    recorder.RecordFrame(100, {1, 2});
    const ReplayFrame* f = recorder.FrameAtTick(100);
    assert(f != nullptr);
    assert(f->tick == 100);

    std::cout << "[PASS] test_replay_start_from_save" << std::endl;
}

// --- Engine LoadAndReplay ---

void test_engine_load_and_replay() {
    const std::string path = "/tmp/atlas_load_replay_test.asav";
    std::remove(path.c_str());

    // Save
    {
        EngineConfig cfg;
        cfg.mode = EngineMode::Server;
        cfg.tickRate = 60;
        cfg.maxTicks = 10;

        Engine engine(cfg);
        engine.InitCore();
        engine.InitECS();
        engine.InitNetworking();
        engine.GetScheduler().SetFramePacing(false);

        engine.GetWorld().CreateEntity();
        engine.Run();

        auto ecsData = engine.GetWorld().Serialize();
        auto& save = engine.GetSaveSystem();
        auto result = save.Save(path,
                                engine.GetTimeModel().Context().sim.tick,
                                cfg.tickRate, 0, ecsData);
        assert(result == SaveResult::Success);
    }

    // Load and replay
    {
        EngineConfig cfg;
        cfg.mode = EngineMode::Server;
        cfg.tickRate = 60;
        cfg.maxTicks = 0;

        Engine engine(cfg);
        engine.InitCore();
        engine.InitECS();
        engine.InitNetworking();
        engine.GetScheduler().SetFramePacing(false);

        bool ok = engine.LoadAndReplay(path);
        assert(ok);
        assert(engine.GetTimeModel().Context().sim.tick == 10);
        assert(engine.GetWorld().EntityCount() >= 1);
    }

    std::remove(path.c_str());
    std::cout << "[PASS] test_engine_load_and_replay" << std::endl;
}

// --- Divergence report export ---

void test_divergence_export_report() {
    const std::string path = "/tmp/atlas_div_report_test.txt";
    std::remove(path.c_str());

    DivergenceReport report;
    report.divergeTick = 42;
    report.localHash = 0x1111;
    report.remoteHash = 0x2222;
    report.totalTicksCompared = 100;
    report.firstMatchingTicks = 42;
    report.severity = DivergenceSeverity::Warning;

    bool ok = ReplayDivergenceInspector::ExportReport(report, path);
    assert(ok);

    std::ifstream in(path);
    assert(in.is_open());
    std::string content((std::istreambuf_iterator<char>(in)),
                         std::istreambuf_iterator<char>());
    assert(content.find("DIVERGED") != std::string::npos);
    assert(content.find("42") != std::string::npos);

    std::remove(path.c_str());
    std::cout << "[PASS] test_divergence_export_report" << std::endl;
}
