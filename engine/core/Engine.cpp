#include "Engine.h"
#include "Logger.h"
#include "../render/GLRenderer.h"
#include "../render/VulkanRenderer.h"
#include "../sim/StateHasher.h"
#include "../sim/ReplayRecorder.h"

#ifdef __linux__
#include "../platform/X11Window.h"
#endif

namespace atlas {

Engine::Engine(const EngineConfig& cfg) : m_config(cfg) {
}

Engine::~Engine() {
    Shutdown();
}

void Engine::InitCore() {
    Logger::Init();
    Logger::Info("Engine core initialized");
    m_running = true;
}

void Engine::InitRender() {
    if (m_config.mode == EngineMode::Server) {
        Logger::Info("Server mode: rendering disabled");
        return;
    }

    if (m_config.headless) {
        Logger::Info("Headless mode: rendering disabled");
        return;
    }

#ifdef __linux__
    auto window = std::make_unique<platform::X11Window>();
#else
    Logger::Error("No platform window implementation for this OS");
    return;
#endif

    platform::PlatformWindowConfig winCfg;
    switch (m_config.mode) {
        case EngineMode::Editor: winCfg.title = "Atlas Editor"; break;
        case EngineMode::Client: winCfg.title = "Atlas Client"; break;
        default: winCfg.title = "Atlas Engine"; break;
    }
    winCfg.width = m_config.windowWidth;
    winCfg.height = m_config.windowHeight;
    winCfg.resizable = true;

    if (!window->Init(winCfg)) {
        Logger::Error("Failed to create platform window");
        return;
    }

    m_window = std::move(window);

    if (m_config.renderAPI == render::RenderAPI::OpenGL) {
        auto gl = std::make_unique<render::GLRenderer>();
        gl->SetViewport(m_config.windowWidth, m_config.windowHeight);
        m_renderer = std::move(gl);
        Logger::Info("OpenGL renderer initialized");
    } else if (m_config.renderAPI == render::RenderAPI::Vulkan) {
        auto vk = std::make_unique<render::VulkanRenderer>();
        vk->SetViewport(m_config.windowWidth, m_config.windowHeight);
        m_renderer = std::move(vk);
        Logger::Info("Vulkan renderer initialized (stub)");
    }

    Logger::Info("Render system initialized");
}

void Engine::InitUI() {
    ui::GUIContext guiCtx;
    switch (m_config.mode) {
        case EngineMode::Editor: guiCtx = ui::GUIContext::Editor; break;
        case EngineMode::Client: guiCtx = ui::GUIContext::Game;   break;
        case EngineMode::Server: guiCtx = ui::GUIContext::Server; break;
    }
    m_uiManager.Init(guiCtx);
    if (m_renderer) {
        m_uiManager.SetRenderer(m_renderer.get());
    }
    Logger::Info("UI system initialized");
}

void Engine::InitECS() {
    Logger::Info("ECS initialized (empty world)");
}

void Engine::InitNetworking() {
    net::NetMode netMode = net::NetMode::Standalone;
    if (m_config.mode == EngineMode::Server) {
        netMode = net::NetMode::Server;
    } else if (m_config.mode == EngineMode::Client) {
        netMode = net::NetMode::Client;
    }
    m_net.Init(netMode);
    Logger::Info("Networking initialized");
}

void Engine::InitEditor() {
    if (m_config.mode != EngineMode::Editor) {
        return;
    }
    Logger::Info("Editor tools initialized");
}

void Engine::Run() {
    m_scheduler.SetTickRate(m_config.tickRate);
    m_timeModel.SetTickRate(m_config.tickRate);

    switch (m_config.mode) {
        case EngineMode::Editor: RunEditor(); break;
        case EngineMode::Client: RunClient(); break;
        case EngineMode::Server: RunServer(); break;
    }
}

void Engine::PerformAutosaveIfNeeded(uint64_t tickCount) {
    if (m_config.autosaveInterval > 0 && tickCount % m_config.autosaveInterval == 0) {
        auto ecsData = m_world.Serialize();
        m_saveSystem.Save(m_config.autosavePath,
                          m_timeModel.Context().sim.tick,
                          m_config.tickRate, 0, ecsData);
    }
}

void Engine::ProcessWindowEvents() {
    if (!m_window) return;

    platform::WindowEvent event;
    while (m_window->PollEvent(event)) {
        switch (event.type) {
            case platform::WindowEvent::Type::Close:
                m_running = false;
                break;
            case platform::WindowEvent::Type::Resize:
                if (m_renderer) {
                    if (m_config.renderAPI == render::RenderAPI::OpenGL) {
                        static_cast<render::GLRenderer*>(m_renderer.get())
                            ->SetViewport(event.width, event.height);
                    } else {
                        static_cast<render::VulkanRenderer*>(m_renderer.get())
                            ->SetViewport(event.width, event.height);
                    }
                }
                break;
            case platform::WindowEvent::Type::MouseButtonDown: {
                ui::UICommand cmd;
                cmd.type = ui::UICommandType::ButtonPress;
                cmd.valueFloat = static_cast<float>(event.mouseX);
                cmd.valueString = std::to_string(event.mouseY);
                m_uiManager.GetCommandBus().Enqueue(std::move(cmd));
                break;
            }
            default:
                break;
        }
    }
}

void Engine::RunEditor() {
    Logger::Info("Running Atlas Editor");
    uint64_t tickCount = 0;
    while (m_running) {
        ProcessWindowEvents();
        m_net.Poll();
        m_scheduler.Tick([this](float dt) {
            m_timeModel.AdvanceTick();
            const auto& timeCtx = m_timeModel.Context();
            m_world.Update(timeCtx.sim.fixedDeltaTime);
            ui::UIContext uiCtx{};
            if (m_window) {
                uiCtx.screenWidth = static_cast<float>(m_window->Width());
                uiCtx.screenHeight = static_cast<float>(m_window->Height());
            }
            uiCtx.deltaTime = timeCtx.sim.fixedDeltaTime;
            uiCtx.tick = static_cast<uint32_t>(timeCtx.sim.tick);
            m_uiManager.Update(uiCtx);
        });

        if (m_renderer && m_window && m_window->IsOpen()) {
            m_renderer->BeginFrame();
            m_uiManager.Render(m_renderer.get());
            m_renderer->EndFrame();
            m_window->SwapBuffers();
        }

        tickCount++;
        if (m_config.maxTicks > 0 && tickCount >= m_config.maxTicks) {
            m_running = false;
        }
    }
}

void Engine::RunClient() {
    Logger::Info("Running Atlas Client");
    uint64_t tickCount = 0;
    while (m_running) {
        ProcessWindowEvents();
        m_net.Poll();
        m_scheduler.Tick([this](float dt) {
            m_timeModel.AdvanceTick();
            const auto& timeCtx = m_timeModel.Context();
            m_world.Update(timeCtx.sim.fixedDeltaTime);
            ui::UIContext uiCtx{};
            if (m_window) {
                uiCtx.screenWidth = static_cast<float>(m_window->Width());
                uiCtx.screenHeight = static_cast<float>(m_window->Height());
            }
            uiCtx.deltaTime = timeCtx.sim.fixedDeltaTime;
            uiCtx.tick = static_cast<uint32_t>(timeCtx.sim.tick);
            m_uiManager.Update(uiCtx);
        });

        if (m_renderer && m_window && m_window->IsOpen()) {
            m_renderer->BeginFrame();
            m_uiManager.Render(m_renderer.get());
            m_renderer->EndFrame();
            m_window->SwapBuffers();
        }

        tickCount++;

        PerformAutosaveIfNeeded(tickCount);

        if (m_config.maxTicks > 0 && tickCount >= m_config.maxTicks) {
            m_running = false;
        }
    }
}

void Engine::RunServer() {
    Logger::Info("Running Atlas Server");
    uint64_t tickCount = 0;
    while (m_running) {
        m_net.Poll();
        m_scheduler.Tick([this](float dt) {
            m_timeModel.AdvanceTick();
            const auto& timeCtx = m_timeModel.Context();
            m_world.Update(timeCtx.sim.fixedDeltaTime);

            // Periodically snapshot world state for rollback support.
            // Snapshot every tick so the server can roll back as needed.
            auto ecsData = m_world.Serialize();
            auto snapshot = m_worldState.TakeSnapshot(timeCtx.sim.tick, ecsData);
            m_worldState.PushSnapshot(std::move(snapshot));
        });
        m_net.Flush();

        tickCount++;

        PerformAutosaveIfNeeded(tickCount);

        if (m_config.maxTicks > 0 && tickCount >= m_config.maxTicks) {
            m_running = false;
        }
    }
}

bool Engine::RollbackToTick(uint64_t tick) {
    const auto* snapshot = m_worldState.SnapshotAtTick(tick);
    if (!snapshot) return false;

    if (!m_world.Deserialize(snapshot->ecsData)) return false;

    m_timeModel.SetTick(tick);
    return true;
}

bool Engine::LoadAndReplay(const std::string& savePath) {
    auto result = m_saveSystem.Load(savePath);
    if (result != sim::SaveResult::Success) {
        return false;
    }

    if (!m_world.Deserialize(m_saveSystem.ECSData())) {
        return false;
    }

    m_timeModel.SetTick(m_saveSystem.Header().saveTick);
    return true;
}

bool Engine::ReplayFromSave(const std::string& savePath, const std::string& replayPath) {
    // Step 1: Load save file to restore world state
    auto result = m_saveSystem.Load(savePath);
    if (result != sim::SaveResult::Success) {
        return false;
    }

    if (!m_world.Deserialize(m_saveSystem.ECSData())) {
        return false;
    }

    uint64_t saveTick = m_saveSystem.Header().saveTick;
    m_timeModel.SetTick(saveTick);

    // Step 2: Load replay file
    sim::ReplayRecorder replay;
    if (!replay.LoadReplay(replayPath)) {
        return false;
    }

    // Step 3: Apply replay inputs from save tick forward
    m_scheduler.SetTickRate(m_config.tickRate);
    m_scheduler.SetFramePacing(false);

    for (const auto& frame : replay.Frames()) {
        if (frame.tick <= saveTick) continue;  // Skip frames before/at save point

        m_timeModel.AdvanceTick();
        m_world.Update(m_timeModel.Context().sim.fixedDeltaTime);
    }

    return true;
}

bool Engine::Running() const {
    return m_running;
}

void Engine::Shutdown() {
    if (m_running) {
        Logger::Info("Engine shutting down");
        m_uiManager.Shutdown();
        m_net.Shutdown();
        m_renderer.reset();
        if (m_window) {
            m_window->Shutdown();
            m_window.reset();
        }
        m_running = false;
        Logger::Shutdown();
    }
}

bool Engine::Can(Capability cap) const {
    switch (cap) {
        case Capability::AssetWrite:
            return m_config.mode == EngineMode::Editor;
        case Capability::Rendering:
            return m_config.mode != EngineMode::Server;
        case Capability::Physics:
            return true;
        case Capability::GraphEdit:
            return m_config.mode == EngineMode::Editor;
        case Capability::GraphExecute:
            return true;
        case Capability::NetAuthority:
            return m_config.mode == EngineMode::Server;
        case Capability::HotReload:
            return m_config.mode == EngineMode::Editor;
    }
    return false;
}

}
