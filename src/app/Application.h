#pragma once

struct GLFWwindow;

class Application {
public:
    Application(int width, int height, const char* title);
    ~Application();

    void run();

private:
    void processInput();
    void onResize(int width, int height);

    GLFWwindow* m_window = nullptr;
    int m_width;
    int m_height;
};
