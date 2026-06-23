#pragma once
#include <memory>
#include "rendering/Camera.h"

// Forward declarations: o .cpp inclui os headers completos.
// No .h basta informar ao compilador que essas classes existem,
// pois unique_ptr<T> só precisa do tipo completo no ponto de destruição (o .cpp).
class Shader;
class ParticleBuffer;
struct GLFWwindow;

class Application {
public:
    // Inicializa GLFW, cria janela, carrega GLAD e monta a cena inicial.
    Application(int width, int height, const char* title);

    // Destrói objetos GL explicitamente antes de encerrar o contexto OpenGL.
    ~Application();

    // Loop principal: input → render → swap, até fechar a janela.
    void run();

private:
    void processInput();
    void onResize(int width, int height);

    // Delegados dos callbacks C do GLFW — mesma convenção de onResize.
    void onMouseButton(int button, int action);
    void onCursorPos(double x, double y);
    void onScroll(double dy);

    GLFWwindow* m_window = nullptr;
    int m_width;
    int m_height;

    Camera                          m_camera;    // sem GL — pode ser membro direto
    std::unique_ptr<Shader>         m_shader;    // criado após GLAD carregar
    std::unique_ptr<ParticleBuffer> m_particles; // idem

    float m_lastMouseX   = 0.0f;
    float m_lastMouseY   = 0.0f;
    bool  m_mousePressed = false;
};
