## Кушать либы
```
sudo apt update
sudo apt install libglm-dev
sudo apt install libglfw3 libglfw3-dev
```

Old code
```
for (int z = 0; z <= height; z++) {
        for (int x = 0; x <= width; x++) {
            float h = map.getPixel(z,x)*5.f;
            vertices.push_back((x - width/2) * step);   // X
            vertices.push_back(h);                       // Y
            vertices.push_back((z - height/2) * step);  // Z
        }
    }

    for (int z = 0; z < height; z++) {
        for (int x = 0; x < width; x++) {
            int start = z * (width + 1) + x;

            indices.push_back(start);
            indices.push_back(start + 1);
            indices.push_back(start + (height + 1));

            indices.push_back(start + 1);
            indices.push_back(start + (height + 2));
            indices.push_back(start + (height + 1));
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
```
