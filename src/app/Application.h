#pragma once
#include <memory>
#include <unordered_map>

// Forward declarations: o .cpp inclui os headers completos.
class OrbitalSimulation;
class OrbitalRenderer;
class ICommand;
struct GLFWwindow;

class Application {
public:
    Application(int width, int height, const char* title);
    ~Application();
    void run();

private:
    void processInput();
    void onResize(int width, int height);
    void onMouseButton(int button, int action);
    void onCursorPos(double x, double y);
    void onScroll(double dy);

    GLFWwindow* m_window = nullptr;
    int m_width;
    int m_height;

    // unique_ptr porque ambos fazem chamadas GL no construtor —
    // precisam ser criados DEPOIS do GLAD carregar no corpo do construtor.
    std::unique_ptr<OrbitalSimulation> m_sim;
    std::unique_ptr<OrbitalRenderer>   m_renderer;

    std::unordered_map<int, std::unique_ptr<ICommand>> m_commands;

    float m_lastMouseX   = 0.0f;
    float m_lastMouseY   = 0.0f;
    bool  m_mousePressed = false;
    // Detecta borda de subida (pressionar vs segurar): evita executar o comando 60x/s.
    bool  m_prevKeys[512] = {};
};
