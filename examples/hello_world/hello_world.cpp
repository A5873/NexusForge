#include "nexusforge/core/engine.hpp"
#include <iostream>
#include <string>

int main(int argc, char* argv[]) {
    // Print welcome message
    std::cout << "NexusForge Hello World Example" << std::endl;
    std::cout << "===============================" << std::endl;
    
    try {
        // Configure the engine
        nf::EngineConfig config;
        config.appName = "NexusForge Hello World";
        config.appVersion = {0, 1, 0};
        config.windowConfig.title = "Hello NexusForge!";
        config.windowConfig.width = 1024;
        config.windowConfig.height = 768;
        
        // Create and initialize the engine
        nf::Engine engine(config);
        nf::Result result = engine.initialize();
        
        if (result != nf::Result::Success) {
            std::cerr << "Failed to initialize engine" << std::endl;
            return 1;
        }
        
        // Get a reference to the window and change the title
        nf::Window& window = engine.getWindow();
        window.setTitle("Hello NexusForge! - Running");
        
        // Run the engine's main loop
        std::cout << "Running main loop..." << std::endl;
        result = engine.run();
        
        // Check the result of running the engine
        if (result != nf::Result::Success) {
            std::cerr << "Engine encountered an error during execution" << std::endl;
            return 1;
        }
        
        std::cout << "Engine stopped cleanly" << std::endl;
        return 0;
    }
    catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
        return 1;
    }
    catch (...) {
        std::cerr << "Unknown exception" << std::endl;
        return 1;
    }
}

