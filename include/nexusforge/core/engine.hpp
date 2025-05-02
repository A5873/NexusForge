#pragma once

#include "nexusforge/core/types.hpp"
#include "nexusforge/core/window.hpp"
#include <memory>
#include <string>

namespace nf {

// Forward declarations
class EngineImpl;

/**
 * Engine configuration parameters
 */
struct EngineConfig {
    std::string appName = "NexusForge Application";
    Version appVersion = {0, 1, 0};
    bool enableLogging = true;
    bool enableProfiling = false;
    WindowConfig windowConfig;
};

/**
 * Main engine class that manages the game loop and systems
 */
class NF_API Engine {
public:
    /**
     * Creates the engine with the specified configuration
     * 
     * @param config Engine configuration parameters
     */
    explicit Engine(const EngineConfig& config = EngineConfig());

    /**
     * Destructor
     */
    ~Engine();

    /**
     * Prevent copying
     */
    Engine(const Engine&) = delete;
    Engine& operator=(const Engine&) = delete;

    /**
     * Initializes the engine
     * 
     * @return Result indicating success or failure
     */
    Result initialize();

    /**
     * Runs the main game loop until the window is closed or stop() is called
     * 
     * @return Result indicating success or failure
     */
    Result run();

    /**
     * Updates a single frame of the engine
     * 
     * @param deltaTime Time elapsed since the last update in seconds
     * @return True if the engine should continue running, false otherwise
     */
    bool update(float deltaTime);

    /**
     * Renders a single frame
     * 
     * @return Result indicating success or failure
     */
    Result render();

    /**
     * Stops the engine's main loop
     */
    void stop();

    /**
     * Checks if the engine is running
     * 
     * @return True if the engine is running, false otherwise
     */
    bool isRunning() const;

    /**
     * Gets the main window
     * 
     * @return Reference to the main window
     */
    Window& getWindow();

    /**
     * Gets the engine's version
     * 
     * @return Engine version information
     */
    static Version getVersion();

private:
    // Internal implementation
    std::unique_ptr<EngineImpl> m_impl;
    std::unique_ptr<Window> m_window;
    bool m_running = false;
};

} // namespace nf

