#pragma once
#include "map.hpp"
#include "actor.hpp"
#include "shader.hpp"
#include <GL/glew.h>
#include <vector>
class VolumeMap : public Actor {
    public:
        VolumeMap();
        ~VolumeMap();
        void generate(Map& map, float step, float height);
        void update(float dt) override;

    private:
        unsigned int VAO, VBO, EBO;
        std::vector<float> vertices;
        std::vector<unsigned int> indices;
        void draw();
};

VolumeMap::VolumeMap() {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
}

VolumeMap::~VolumeMap() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}

void VolumeMap::update(float dt) {
    draw();
}

void VolumeMap::generate(Map& map, float step = 0.2f, float maxHeight = 10.f) {
    int width  = map.getWidth();
    int height = map.getHeight();

    for (int z = 0; z <= height; z++) {
        for (int x = 0; x <= width; x++) {
            float h = map.getPixel(z,x) * maxHeight;
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

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
}

void VolumeMap::draw() {
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
}
