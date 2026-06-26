#include "Application.h"
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/gl.h>
#include <stdexcept>
#include "simulation/OrbitalSimulation.h"
#include "rendering/OrbitalRenderer.h"
#include "ui/OrbitalCommands.h"

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
    glfwSetMouseButtonCallback(m_window, [](GLFWwindow* w, int btn, int action, int) {
        static_cast<Application*>(glfwGetWindowUserPointer(w))->onMouseButton(btn, action);
    });
    glfwSetCursorPosCallback(m_window, [](GLFWwindow* w, double x, double y) {
        static_cast<Application*>(glfwGetWindowUserPointer(w))->onCursorPos(x, y);
    });
    glfwSetScrollCallback(m_window, [](GLFWwindow* w, double, double dy) {
        static_cast<Application*>(glfwGetWindowUserPointer(w))->onScroll(dy);
    });

    glfwMakeContextCurrent(m_window);

    if (!gladLoadGL(glfwGetProcAddress))
        throw std::runtime_error("Failed to initialize GLAD");

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_PROGRAM_POINT_SIZE);

    // Criados aqui — depois do GLAD carregar — porque fazem chamadas GL no construtor.
    m_sim      = std::make_unique<OrbitalSimulation>(1, 0, 0);
    m_renderer = std::make_unique<OrbitalRenderer>(width, height);

    // Liga o renderer como observer: quando m_sim gera partículas, avisa o renderer.
    m_sim->addObserver(m_renderer.get());

    // Mapeia teclas GLFW para comandos.
    m_commands[GLFW_KEY_W] = std::make_unique<IncreaseN>(*m_sim);
    m_commands[GLFW_KEY_S] = std::make_unique<DecreaseN>(*m_sim);
    m_commands[GLFW_KEY_E] = std::make_unique<IncreaseL>(*m_sim);
    m_commands[GLFW_KEY_D] = std::make_unique<DecreaseL>(*m_sim);
    m_commands[GLFW_KEY_R] = std::make_unique<IncreaseM>(*m_sim);
    m_commands[GLFW_KEY_F] = std::make_unique<DecreaseM>(*m_sim);
    m_commands[GLFW_KEY_T] = std::make_unique<IncreaseParticles>(*m_sim);
    m_commands[GLFW_KEY_G] = std::make_unique<DecreaseParticles>(*m_sim);

    // Primeira amostragem: dispara onParticlesUpdated → renderer recebe os pontos.
    m_sim->resample();
}

Application::~Application() {
    // Ordem importa: commands referenciam m_sim, renderer usa GL.
    // Destruir explicitamente antes de encerrar o contexto OpenGL.
    m_commands.clear();
    m_renderer.reset();
    m_sim.reset();
    glfwDestroyWindow(m_window);
    glfwTerminate();
}

void Application::run() {
    while (!glfwWindowShouldClose(m_window)) {
        processInput();
        m_renderer->draw(m_width, m_height);
        glfwSwapBuffers(m_window);
        glfwPollEvents();
    }
}

void Application::processInput() {
    if (glfwGetKey(m_window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(m_window, true);

    // Executa o comando só na borda de subida (transição não-pressionado → pressionado).
    for (auto& [key, cmd] : m_commands) {
        bool pressed = glfwGetKey(m_window, key) == GLFW_PRESS;
        if (pressed && !m_prevKeys[key])
            cmd->execute();
        m_prevKeys[key] = pressed;
    }
}

void Application::onResize(int width, int height) {
    m_width  = width;
    m_height = height;
    glViewport(0, 0, width, height);
}

void Application::onMouseButton(int button, int action) {
    if (button != GLFW_MOUSE_BUTTON_LEFT) return;
    m_mousePressed = (action == GLFW_PRESS);
    if (m_mousePressed) {
        double x, y;
        glfwGetCursorPos(m_window, &x, &y);
        m_lastMouseX = static_cast<float>(x);
        m_lastMouseY = static_cast<float>(y);
    }
}

void Application::onCursorPos(double x, double y) {
    float fx = static_cast<float>(x);
    float fy = static_cast<float>(y);
    if (m_mousePressed)
        m_renderer->camera().onMouseDrag(fx - m_lastMouseX, fy - m_lastMouseY);
    m_lastMouseX = fx;
    m_lastMouseY = fy;
}

void Application::onScroll(double dy) {
    m_renderer->camera().onScroll(static_cast<float>(dy));
}
