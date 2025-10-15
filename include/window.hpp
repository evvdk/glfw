#include <GLFW/glfw3.h>
#include <stdexcept>
#include "scene.hpp"
class Window {
    private:
        GLFWwindow* window;
        Scene* scene;
    public:
        Window();
        ~Window();
        void initGL();
        void run();
};

Window::Window() {
    if (!glfwInit())
        throw std::runtime_error("Failed to initialize GLFW");

    this->window = glfwCreateWindow(800, 600, "Window", nullptr, nullptr);
    if (!window) {
        throw std::runtime_error("Failed to create window");
    }

    glfwMakeContextCurrent(window);

    scene = new Scene();
}

void Window::initGL() {
    glClearColor(255, 0, 0, 255);
}

void Window::run(){
    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);

        glColor3f(0.2f, 0.7f, 0.3f);
        
        glBegin(GL_POLYGON);
            glVertex2f(-0.5f, -0.5f);
            glVertex2f(0.5f, -0.5f);
            glVertex2f(0.0f, 0.5f);
        glEnd();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
}

Window::~Window(){
    delete scene;
}