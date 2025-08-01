#pragma once
#include "../scratch/image.hpp"
#include <3ds.h>
#include <citro2d.h>
#include <citro3d.h>
#include <string>
#include <unordered_map>

struct ImageData {
    C2D_Image image;
    u16 freeTimer = 240;
};

bool get_C2D_Image(Image::ImageRGBA rgba);
bool queueC2DImage(Image::ImageRGBA &rgba);
void freeRGBA(const std::string &imageName);
unsigned char *SVGToRGBA(const void *svg_data, size_t svg_size, int &width, int &height);

extern std::unordered_map<std::string, ImageData> imageC2Ds;