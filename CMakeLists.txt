cmake_minimum_required(VERSION 3.10)
project(OpenGL_GLEW_Example)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Находим OpenGL
find_package(OpenGL REQUIRED)

# Используем pkg-config, чтобы найти GLEW и GLFW
find_package(PkgConfig REQUIRED)
pkg_search_module(GLEW REQUIRED glew)
pkg_search_module(GLFW REQUIRED glfw3)

# Пути к заголовочным файлам
include_directories(
    ${OPENGL_INCLUDE_DIRS}
    ${GLEW_INCLUDE_DIRS}
    ${GLFW_INCLUDE_DIRS}
    include
)

# Исполняемый файл
add_executable(OpenGL_GLEW_Example
    src/main.cpp
)

# Линкуем библиотеки
target_link_libraries(OpenGL_GLEW_Example
    ${OPENGL_LIBRARIES}
    ${GLEW_LIBRARIES}
    ${GLFW_LIBRARIES}
)