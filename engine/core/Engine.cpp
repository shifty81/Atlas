#include "Engine.h"
#include "Logger.h"

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

    switch (m_config.mode) {
        case EngineMode::Editor: RunEditor(); break;
        case EngineMode::Client: RunClient(); break;
        case EngineMode::Server: RunServer(); break;
    }
}

void Engine::RunEditor() {
    Logger::Info("Running Atlas Editor");
    uint64_t tickCount = 0;
    while (m_running) {
        m_net.Poll();
        m_scheduler.Tick([this](float dt) {
            m_world.Update(dt);
            ui::UIContext uiCtx{};
            uiCtx.deltaTime = dt;
            uiCtx.tick = static_cast<uint32_t>(m_scheduler.CurrentTick());
            m_uiManager.Update(uiCtx);
        });
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
        m_net.Poll();
        m_scheduler.Tick([this](float dt) {
            m_world.Update(dt);
            ui::UIContext uiCtx{};
            uiCtx.deltaTime = dt;
            uiCtx.tick = static_cast<uint32_t>(m_scheduler.CurrentTick());
            m_uiManager.Update(uiCtx);
        });
        tickCount++;
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
            m_world.Update(dt);
        });
        m_net.Flush();
        tickCount++;
        if (m_config.maxTicks > 0 && tickCount >= m_config.maxTicks) {
            m_running = false;
        }
    }
}

bool Engine::Running() const {
    return m_running;
}

void Engine::Shutdown() {
    if (m_running) {
        Logger::Info("Engine shutting down");
        m_uiManager.Shutdown();
        m_net.Shutdown();
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
