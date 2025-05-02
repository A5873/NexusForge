#include "nexusforge/core/engine.hpp"
#include <iostream>
#include <chrono>
#include <thread>

namespace nf {

// Private implementation class (PIMPL pattern)
class EngineImpl {
public:
    EngineImpl(const EngineConfig& config) 
        : m_config(config) {
    }

    Result initialize() {
        std::cout << "NexusForge Engine v" << Engine::getVersion().toString() << " initializing..." << std::endl;
        std::cout << "Application: " << m_config.appName << " v" << m_config.appVersion.toString() << std::endl;
        
        // Future: Initialize subsystems here (rendering, physics, audio, etc.)
        
        return Result::Success;
    }

    bool update(float deltaTime) {
        // Process input, update game logic, physics, etc.
        std::cout << "Update (dt=" << deltaTime << "s)" << std::endl;
        return true;
    }

    Result render() {
        // Future: Render the current frame
        return Result::Success;
    }

private:
    EngineConfig m_config;
};

// Engine implementation

Engine::Engine(const EngineConfig& config)
    : m_impl(std::make_unique<EngineImpl>(config))
    , m_window(std::make_unique<Window>(config.windowConfig))
    , m_running(false) {
}

Engine::~Engine() {
    if (m_running) {
        stop();
    }
}

Result Engine::initialize() {
    if (!m_window->isOpen()) {
        std::cerr << "Failed to create window" << std::endl;
        return Result::Failure;
    }
    
    return m_impl->initialize();
}

Result Engine::run() {
    if (!m_window->isOpen()) {
        return Result::Failure;
    }
    
    m_running = true;
    
    auto lastTime = std::chrono::high_resolution_clock::now();
    
    // Main game loop
    while (m_running && m_window->update()) {
        // Calculate delta time
        auto currentTime = std::chrono::high_resolution_clock::now();
        float deltaTime = std::chrono::duration<float>(currentTime - lastTime).count();
        lastTime = currentTime;
        
        // Update engine systems
        if (!update(deltaTime)) {
            break;
        }
        
        // Render frame
        render();
        
        // Add a small sleep to avoid consuming 100% CPU in this demo
        std::this_thread::sleep_for(std::chrono::milliseconds(16)); // Roughly 60 FPS
    }
    
    m_running = false;
    return Result::Success;
}

bool Engine::update(float deltaTime) {
    // Future: Update ECS, physics, audio, etc.
    return m_impl->update(deltaTime);
}

Result Engine::render() {
    // Clear, draw, present
    return m_impl->render();
}

void Engine::stop() {
    m_running = false;
}

bool Engine::isRunning() const {
    return m_running;
}

Window& Engine::getWindow() {
    return *m_window;
}

Version Engine::getVersion() {
    // Current engine version
    return {0, 1, 0};
}

} // namespace nf

