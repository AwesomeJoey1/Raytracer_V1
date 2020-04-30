#pragma once
#include <vector>
#include <glm/glm/glm.hpp>

class Image {
public:
    Image(int width, int height);
    Image(const char* path);

    bool hasData();
    void save();

    glm::vec3 getPixel(int x, int y);
    void setPixel(int x, int y, glm::vec3);

    int width();
    int height();

private:
    int _width, _height;
    std::vector<uint8_t> _imageBuffer;
};
