#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb/stb_image_write.h"

#include "Common.h"
#include "Image.h"

#define NUM_CHANNELS 3

Image::Image(int width, int height)
{
    _width = width;
    _height = height;
    _imageBuffer.resize(width*height*3, 0.0f);
}

Image::Image(const char* path)
{
    int numChannels;
    unsigned char* data = stbi_load(path, &_width, &_height, &numChannels, NUM_CHANNELS);
    if (data == nullptr) {
        std::cerr << "IMAGE ERROR: Could not load image from: " << path << std::endl;
        return;
    }
    int imgSize = _width * _height * NUM_CHANNELS;
    _imageBuffer.resize(imgSize);
    for (int idx = 0; idx < imgSize; idx++)
    {
        _imageBuffer[idx] = static_cast<int>(data[idx]);
    }

    save();
}

// checks if image data is available
bool Image::hasData()
{
    return !_imageBuffer.empty();
}

// save image as png and jpg.
void Image::save()
{
    stbi_flip_vertically_on_write(true);
    const uint8_t* imgArray = &(_imageBuffer[0]);
    stbi_write_png("image.png", _width, _height, NUM_CHANNELS, imgArray, _width * NUM_CHANNELS);
    stbi_write_jpg("image.jpg", _width, _height, NUM_CHANNELS, imgArray, 100);
}

// return image color at position (x,y)
glm::vec3 Image::getPixel(int x, int y)
{
    glm::vec3 pixelColor;
    int idx = y * _width * 3 + x * 3;
    pixelColor.r = _imageBuffer[idx];
    pixelColor.g = _imageBuffer[++idx];
    pixelColor.b = _imageBuffer[++idx];

    return pixelColor;
}
// set pixel at coordinate (x,y) with specified rayColor
void Image::setPixel(int x, int y, glm::vec3 color)
{
    int index = y * _width * 3 + x * 3;
    _imageBuffer[index] = color.r;
    _imageBuffer[++index] = color.g;
    _imageBuffer[++index] = color.b;
}

// returns width of the image
int Image::width()
{
    return _width;
}

// return height of the image
int Image::height()
{
    return _height;
}

