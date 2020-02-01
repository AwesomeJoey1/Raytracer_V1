#pragma once
#include <vector>
#include <glm/glm/glm.hpp>

class Image {
public:
    Image(int width, int height);
    void setPixel(int x, int y, glm::vec3);
    void save();

private:
    int _width, _height;
    std::vector<uint8_t> _imageBuffer;
};
