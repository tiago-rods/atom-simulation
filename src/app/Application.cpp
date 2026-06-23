#include "Application.h"
#include <GLFW/glfw3.h>
#include <stdexcept>

Application::Application(int width, int height, const char* title)
    : m_width(width), m_height(height) // lista de inicialização: executada antes do corpo {}
{
    if (!glfwInit())
        throw std::runtime_error("Failed to initialize GLFW");

    // Exige OpenGL 3.3 Core Profile (remove funções legadas, força uso de shaders).
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // nullptr #1 = sem fullscreen, nullptr #2 = sem contexto compartilhado.
    m_window = glfwCreateWindow(width, height, title, nullptr, nullptr);
    if (!m_window) {
        glfwTerminate(); // libera GLFW antes de lançar, senão vaza recursos
        throw std::runtime_error("Failed to create GLFW window");
    }

    // Armazena 'this' na janela para recuperar o objeto dentro de callbacks C.
    // Callbacks do GLFW são funções livres (API C), não métodos — sem acesso ao objeto.
    glfwSetWindowUserPointer(m_window, this);
    glfwSetFramebufferSizeCallback(m_window, [](GLFWwindow* w, int width, int height) {
        // Recupera o ponteiro salvo e delega para o método do objeto.
        static_cast<Application*>(glfwGetWindowUserPointer(w))->onResize(width, height);
    });

    // Torna este contexto OpenGL ativo na thread atual.
    // Todo comando GL depois daqui opera nesta janela.
    glfwMakeContextCurrent(m_window);
}

Application::~Application() {
    glfwDestroyWindow(m_window);
    glfwTerminate();
}

void Application::run() {
    while (!glfwWindowShouldClose(m_window)) {
        processInput();

        // Double buffering: desenha no back buffer, swap mostra na tela sem flickering.
        glfwSwapBuffers(m_window);

        // Processa eventos do SO (teclado, mouse, resize).
        // Sem isso a janela trava e não responde.
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
    // glViewport será chamado aqui após o GLAD ser inicializado (Sprint 1).
}
