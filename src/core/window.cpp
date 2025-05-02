#include "nexusforge/core/window.hpp"
#include <iostream>

// Platform-specific implementation
// This is a minimal implementation for demo
namespace nf {

// Simple implementation for demo purposes
// In a real implementation, this would use a platform API like GLFW, SDL, etc.
class WindowImpl {
public:
    WindowImpl(const WindowConfig& config)
        : m_config(config)
        , m_open(true) {
        std::cout << "Creating window: " << config.title << " (" 
                  << config.width << "x" << config.height << ")" << std::endl;
    }

    ~WindowImpl() {
        std::cout << "Destroying window" << std::endl;
    }

    bool isOpen() const {
        return m_open;
    }

    bool update() {
        // Process events, etc.
        // This is a placeholder - no real window events to process
        return m_open;
    }

    void close() {
        m_open = false;
    }

    int getWidth() const {
        return m_config.width;
    }

    int getHeight() const {
        return m_config.height;
    }

    void setTitle(const std::string& title) {
        m_config.title = title;
        std::cout << "Window title changed to: " << title << std::endl;
    }

    void* getNativeHandle() const {
        return nullptr; // No real window in this demo
    }

private:
    WindowConfig m_config;
    bool m_open;
};

// Window implementation

Window::Window(const WindowConfig& config)
    : m_impl(std::make_unique<WindowImpl>(config)) {
}

Window::~Window() = default;

bool Window::isOpen() const {
    return m_impl->isOpen();
}

bool Window::update() {
    return m_impl->update();
}

void Window::close() {
    m_impl->close();
}

int Window::getWidth() const {
    return m_impl->getWidth();
}

int Window::getHeight() const {
    return m_impl->getHeight();
}

void Window::setTitle(const std::string& title) {
    m_impl->setTitle(title);
}

void* Window::getNativeHandle() const {
    return m_impl->getNativeHandle();
}

} // namespace nf

