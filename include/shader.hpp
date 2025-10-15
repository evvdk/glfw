#pragma once
#include <GL/glew.h>
#include <iostream>
#include <fstream>
class Shader {
    private:
        unsigned int shaderProgram;
        void checkCompile(unsigned int shader, std::string type);
        bool loadFile(const char* filename, char*& data)
        {
            std::ifstream f(filename, std::ios::in | std::ios::binary);
            if (!f.is_open())
                return false;

            f.seekg(0, std::ios::end);
            size_t size = (size_t)f.tellg();
            f.seekg(0, std::ios::beg);
            
            data = new char[size + 1];
            f.read(data, size);
            data[size] = '\0';
            f.close();
            return true;
        }
    public:
        Shader() = default;
        ~Shader();
        void compile(const char* vertexShaderSourceFile, const char* fragmentShaderSourceFile);
        void activate() {
            glUseProgram(shaderProgram);
        }

        void deactivate() {
            glUseProgram(0);
        }

        void pass1f(float value, const char* param){
            int location = glGetUniformLocation(shaderProgram, param);
            glUniform1f(location, value);
        }

        void pass3f(glm::vec3 value, const char* param) {
            GLint location = glGetUniformLocation(shaderProgram, param);
            glUniform3fv(location, 1, glm::value_ptr(value));
        }

        void pass1i(int value, const char* param) {
            int location = glGetUniformLocation(shaderProgram, param);
            glUniform1i(location, value);
        }

        void pass4fv(glm::mat4& mat, const char* param) {
            int location = glGetUniformLocation(shaderProgram, param);
            glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(mat));
        }
};

void Shader::checkCompile(unsigned int shader, std::string type) {
    int success;
    char infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        throw std::runtime_error("Ошибка компиляции " + type + " шейдера:\n" + infoLog);
    }
}

void Shader::compile(const char* vertexShaderSourceFile, const char* fragmentShaderSourceFile) {
    
    char* vertexShaderSource;
    if(!loadFile(vertexShaderSourceFile, vertexShaderSource)) throw std::runtime_error("File " 
        + std::string(vertexShaderSourceFile) + "not found");;
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    checkCompile(vertexShader, "вершинного");

    char* fragmentShaderSource;
    if(!loadFile(fragmentShaderSourceFile, fragmentShaderSource)) throw std::runtime_error("File " 
        + std::string(fragmentShaderSourceFile) + "not found");;
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    checkCompile(fragmentShader, "фрагментного");
    
    this->shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    int success;
    char infoLog[512];
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "Ошибка линковки программы:\n" << infoLog << std::endl;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

Shader::~Shader() {
    glDeleteProgram(shaderProgram);
}
