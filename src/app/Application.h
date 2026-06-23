#pragma once

// Forward declaration: evita incluir o header inteiro do GLFW aqui.
// O .cpp que usa as funções GLFW é quem inclui <GLFW/glfw3.h>.
struct GLFWwindow;

class Application {
public:
    // Inicializa GLFW, cria janela OpenGL 3.3 Core e registra callbacks.
    Application(int width, int height, const char* title);

    // Destrói a janela e encerra o GLFW (chamado automaticamente ao sair do escopo).
    ~Application();

    // Loop principal: processa input → renderiza → troca buffers, até fechar a janela.
    void run();

private:
    // Verifica teclas pressionadas a cada frame.
    void processInput();

    // Chamado pelo GLFW quando a janela é redimensionada.
    void onResize(int width, int height);

    GLFWwindow* m_window = nullptr; // handle opaco da janela criada pelo GLFW
    int m_width;
    int m_height;
};
