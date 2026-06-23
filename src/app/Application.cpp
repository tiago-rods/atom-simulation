#include "Application.h"
#include <GLFW/glfw3.h>
#include <stdexcept>

Application::Application(int width, int height, const char* title)
    : m_width(width), m_height(height)
{
    if (!glfwInit())
        throw std::runtime_error("Failed to initialize GLFW");

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    m_window = glfwCreateWindow(width, height, title, nullptr, nullptr);
    if (!m_window) {
        glfwTerminate();
        throw std::runtime_error("Failed to create GLFW window");
    }

    glfwSetWindowUserPointer(m_window, this);
    glfwSetFramebufferSizeCallback(m_window, [](GLFWwindow* w, int width, int height) {
        static_cast<Application*>(glfwGetWindowUserPointer(w))->onResize(width, height);
    });

    glfwMakeContextCurrent(m_window);
}

Application::~Application() {
    glfwDestroyWindow(m_window);
    glfwTerminate();
}

void Application::run() {
    while (!glfwWindowShouldClose(m_window)) {
        processInput();
        glfwSwapBuffers(m_window);
        glfwPollEvents();
    }
}

void Application::processInput() {
    if (glfwGetKey(m_window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(m_window, true);
}

void Application::onResize(int width, int height) {
    m_width = width;
    m_height = height;
    // glViewport set after glad is wired in
}
