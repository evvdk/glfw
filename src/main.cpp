#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <vector>
#include "shader.hpp"
#include "map.hpp"

glm::vec3 cameraPos   = glm::vec3(0.0f, 10.0f,  0.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, -0.3f, -1.0f);
glm::vec3 cameraUp    = glm::vec3(0.0f, 1.0f,  0.0f);

float deltaTime = 0.0f;
float lastFrame = 0.0f;

float lastX = 400, lastY = 300;
float yaw = -90.0f, pitch = 0.0f;
bool firstMouse = true;

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    static float sensitivity = 0.1f;

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;

    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw   += xoffset;
    pitch += yoffset;

    if (pitch > 89.0f) pitch = 89.0f;
    if (pitch < -89.0f) pitch = -89.0f;

    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront = glm::normalize(front);
}


void processInput(GLFWwindow *window)
{
    float cameraSpeed = 20.5f * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cameraPos += cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cameraPos -= cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
}

int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(1920, 1080, "Polygon with Shader", NULL, NULL);
    if (!window) {
        std::cout << "Ошибка создания окна!\n";
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        std::cerr << "Ошибка инициализации GLEW!\n";
        return -1;
    }

    Shader mapShader;
    mapShader.compile("../shaders/mapVertShader.glsl", "../shaders/mapFragShader.glsl");

    Shader outlineShader;
    outlineShader.compile("../shaders/outlineVertShader.glsl", "../shaders/outlineFragShader.glsl");

    glEnable(GL_DEPTH_TEST);

    Map map;
    map.loadMap("../textures/test.jpg");

    std::vector<float> vertices;
    std::vector<unsigned int> indices;

    float step = 0.2f;
    int width  = map.getWidth();
    int height = map.getHeight();

    for (int z = 0; z <= height; z++) {
        for (int x = 0; x <= width; x++) {
            float h = map.getPixel(z,x) * 10.f;
            vertices.push_back((x - width/2) * step);
            vertices.push_back(h);
            vertices.push_back((z - height/2) * step);
        }
    }

    for (int z = 0; z < height; z++) {
        for (int x = 0; x < width; x++) {
            int start = z * (width + 1) + x;
            
            indices.push_back(start);
            indices.push_back(start + 1);
            indices.push_back(start + (width + 1));

            indices.push_back(start + 1);
            indices.push_back(start + 1 + (width + 1));
            indices.push_back(start + (width + 1));
        }
    }

    unsigned int VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 view;
    glm::mat4 projection = glm::perspective(glm::radians(90.0f), 1900.0f / 1080.0f, 0.1f, 100.0f);  

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    while (!glfwWindowShouldClose(window)) {
        float timeValue = glfwGetTime();
        deltaTime = timeValue - lastFrame;
        lastFrame = timeValue;
        
        processInput(window);

        view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
        
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        mapShader.activate();
        
        mapShader.pass4fv(model, "model");
        mapShader.pass4fv(view, "view");
        mapShader.pass4fv(projection, "projection");
        mapShader.pass3f(cameraPos, "cameraPos");
        mapShader.pass1f(timeValue, "uTime");

        glBindVertexArray(VAO);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);

        mapShader.deactivate();
        
        outlineShader.activate();
        
        outlineShader.pass4fv(model, "model");
        outlineShader.pass4fv(view, "view");
        outlineShader.pass4fv(projection, "projection");
        outlineShader.pass3f(cameraPos, "cameraPos");
        outlineShader.pass1f(timeValue, "uTime");  

        glBindVertexArray(VAO);
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        outlineShader.deactivate();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    glfwTerminate();
    return 0;
}