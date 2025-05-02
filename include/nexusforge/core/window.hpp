#pragma once

#include "nexusforge/core/types.hpp"
#include <string>
#include <memory>

namespace nf {

// Forward declarations
class WindowImpl;

/**
 * Window configuration parameters
 */
struct WindowConfig {
    std::string title = "NexusForge Application";
    int width = 800;
    int height = 600;
    bool fullscreen = false;
    bool resizable = true;
    bool vsync = true;
};

/**
 * Window class that handles the application's main window
 */
class NF_API Window {
public:
    /**
     * Creates a window with the specified configuration
     * 
     * @param config Window configuration parameters
     */
    explicit Window(const WindowConfig& config = WindowConfig());

    /**
     * Destructor
     */
    ~Window();

    /**
     * Prevent copying
     */
    Window(const Window&) = delete;
    Window& operator=(const Window&) = delete;

    /**
     * Checks if the window is still open
     * 
     * @return True if the window is open, false otherwise
     */
    bool isOpen() const;

    /**
     * Updates the window, processing events
     * 
     * @return True if the window is still open, false if it should close
     */
    bool update();

    /**
     * Closes the window
     */
    void close();

    /**
     * Gets the window width
     * 
     * @return Current window width in pixels
     */
    int getWidth() const;

    /**
     * Gets the window height
     * 
     * @return Current window height in pixels
     */
    int getHeight() const;

    /**
     * Sets the window title
     * 
     * @param title New window title
     */
    void setTitle(const std::string& title);

    /**
     * Gets the window's native handle
     * 
     * @return Native window handle as void pointer
     */
    void* getNativeHandle() const;

private:
    // Platform-specific implementation
    std::unique_ptr<WindowImpl> m_impl;
};

} // namespace nf
