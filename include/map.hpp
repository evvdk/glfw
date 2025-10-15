#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>
#include <algorithm>
class Map {
    public:
        Map() = default;
        ~Map();
        void loadMap(const char* filename);
        int getWidth() const {
            return width;
        }

        int getHeight() const {
            return height;
        }

        float getPixel(int x, int y) const {
            int idx = y * height + x;
            return data[idx] / 255.f;
        }

        float bilinearHeight(float x, float z, float step) {
            float fx = (x / step) + height / 2;
            float fz = (z / step) + width / 2;

            int x0 = floor(fx);
            int x1 = x0 + 1;
            int z0 = floor(fz);
            int z1 = z0 + 1;

            float h00 = getPixel(z0, x0);
            float h10 = getPixel(z0, x1);
            float h01 = getPixel(z1, x0);
            float h11 = getPixel(z1, x1);

            float tx = fx - x0;
            float tz = fz - z0;

            float h0 = h00 * (1 - tx) + h10 * tx;
            float h1 = h01 * (1 - tx) + h11 * tx;
            float h = h0 * (1 - tz) + h1 * tz;

            return h * 5.f;
        }
    private:
        uint8_t* data = nullptr;
        int width;
        int height;
    
};

void Map::loadMap(const char* filename) {
    int channels;
    data = stbi_load(filename, &width, &height, &channels, 1);
    if(!data) throw std::runtime_error("Map file not found");
}

Map::~Map(){
    stbi_image_free(data);
}



