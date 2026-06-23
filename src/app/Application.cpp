#include "Application.h"
// GLFW_INCLUDE_NONE impede o GLFW de incluir seus próprios headers GL.
// O GLAD deve ser o único responsável por expor as funções OpenGL.
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/gl.h>
#include <stdexcept>
#include <random>
#include "rendering/Shader.h"
#include "rendering/ParticleBuffer.h"

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

    // Armazena 'this' para recuperar o objeto dentro dos callbacks C abaixo.
    glfwSetWindowUserPointer(m_window, this);

    glfwSetFramebufferSizeCallback(m_window, [](GLFWwindow* w, int width, int height) {
        static_cast<Application*>(glfwGetWindowUserPointer(w))->onResize(width, height);
    });

    // Registra botão do mouse: armazena posição inicial no clique para calcular delta.
    glfwSetMouseButtonCallback(m_window, [](GLFWwindow* w, int btn, int action, int) {
        static_cast<Application*>(glfwGetWindowUserPointer(w))->onMouseButton(btn, action);
    });

    // Registra movimento do mouse: calcula delta desde a última posição e orbita a câmera.
    glfwSetCursorPosCallback(m_window, [](GLFWwindow* w, double x, double y) {
        static_cast<Application*>(glfwGetWindowUserPointer(w))->onCursorPos(x, y);
    });

    glfwSetScrollCallback(m_window, [](GLFWwindow* w, double, double dy) {
        static_cast<Application*>(glfwGetWindowUserPointer(w))->onScroll(dy);
    });

    // Contexto OpenGL ativo nesta thread — GLAD precisa disso para consultar os ponteiros.
    glfwMakeContextCurrent(m_window);

    // GLAD resolve os ponteiros de função OpenGL do driver em tempo de execução.
    // Deve vir obrigatoriamente após MakeContextCurrent e antes de qualquer chamada GL.
    if (!gladLoadGL(glfwGetProcAddress))
        throw std::runtime_error("Failed to initialize GLAD");

    glEnable(GL_DEPTH_TEST);        // descarta fragmentos ocluídos usando o z-buffer
    glEnable(GL_PROGRAM_POINT_SIZE); // permite o shader controlar o tamanho dos pontos via gl_PointSize

    // Shader e ParticleBuffer fazem chamadas GL no construtor,
    // por isso só são criados aqui — depois do GLAD estar carregado.
    m_shader = std::make_unique<Shader>("shaders/particle.vert", "shaders/particle.frag");

    // Smoke test: 10k pontos gerados por rejection sampling em uma esfera de raio 10.
    // Rejection sampling: gera ponto no cubo [-1,1]³, descarta se fora da esfera unitária.
    // Isso garante distribuição uniforme no volume (ao contrário de gerar em coords esféricas direto).
    std::mt19937 rng(42);
    std::uniform_real_distribution<float> dist(-1.0f, 1.0f);
    std::vector<glm::vec3> pts;
    pts.reserve(10'000);
    while (static_cast<int>(pts.size()) < 10'000) {
        glm::vec3 p{dist(rng), dist(rng), dist(rng)};
        if (glm::length(p) <= 1.0f)
            pts.push_back(p * 10.0f);
    }
    m_particles = std::make_unique<ParticleBuffer>(pts);
}

Application::~Application() {
    // Em C++, o corpo do destrutor roda ANTES dos destrutores dos membros.
    // Se deixássemos os unique_ptrs serem destruídos normalmente, as chamadas
    // glDelete* aconteceriam depois de glfwTerminate() — contexto já inválido.
    // Resetar explicitamente aqui garante a ordem correta de limpeza.
    m_shader.reset();
    m_particles.reset();
    glfwDestroyWindow(m_window);
    glfwTerminate();
}

void Application::run() {
    while (!glfwWindowShouldClose(m_window)) {
        processInput();

        // Fundo azul escuro: vec4(R, G, B, alpha) — valores em [0,1].
        glClearColor(0.05f, 0.05f, 0.1f, 1.0f);
        // Limpa cor e profundidade antes de desenhar o próximo frame.
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Matriz de projeção perspectiva depende do aspect ratio atual da janela.
        float aspect = static_cast<float>(m_width) / static_cast<float>(m_height);

        // Ativa o shader e envia as matrizes de câmera como uniforms.
        // Os uniforms ficam no shader até o próximo use() de outro programa.
        m_shader->use();
        m_shader->setMat4("uView",       m_camera.view());
        m_shader->setMat4("uProjection", m_camera.projection(aspect));

        m_particles->draw(); // glDrawArrays(GL_POINTS, ...)

        // Apresenta o back buffer na tela (double buffering).
        glfwSwapBuffers(m_window);

        // Dispara os callbacks registrados acima com os eventos acumulados.
        glfwPollEvents();
    }
}

void Application::processInput() {
    if (glfwGetKey(m_window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(m_window, true);
}

void Application::onResize(int width, int height) {
    m_width  = width;
    m_height = height;
    // Ajusta o viewport para que o OpenGL saiba o novo tamanho da área de desenho.
    glViewport(0, 0, width, height);
}

void Application::onMouseButton(int button, int action) {
    if (button != GLFW_MOUSE_BUTTON_LEFT) return;
    m_mousePressed = (action == GLFW_PRESS);
    // Captura posição atual no momento do clique para evitar salto no primeiro drag.
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
        m_camera.onMouseDrag(fx - m_lastMouseX, fy - m_lastMouseY);
    // Atualiza sempre — necessário para ter lastPos correto no próximo evento.
    m_lastMouseX = fx;
    m_lastMouseY = fy;
}

void Application::onScroll(double dy) {
    m_camera.onScroll(static_cast<float>(dy));
}
