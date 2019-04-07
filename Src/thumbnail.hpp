#ifndef thumbnail_hpp
#define thumbnail_hpp

#include "image.hpp"

#include <iostream>

class apThumbnail {
public:
    apThumbnail();
    ~apThumbnail();

    void createThumbnail(const char *inputFile, const char *outputFile, int factor);
    class Invalid {};

private:
    void readImage(const char *inputFile);
    void writeImage(const char *outputFile) const;
    unsigned char averagePixels(int x0, int y0, int factor);

    apImage image_;
    apImage thumbail_;
};

apThumbnail::apThumbnail(){}
apThumbnail::~apThumbnail(){}

void apThumbnail::readImage(const char *inputFile) {
    image_ = apImage(64, 64);
    try {
        for (int y=0; y<image_.height(); y++) {
            for (int x=0; x<image_.width(); x++) {
                image_.setPixel(x, y, (unsigned char)(y % 255));
            }
        }
    } catch (apImage::RangeError) {
        throw Invalid();
    }
}

void apThumbnail::writeImage(const char* outputFile) const {
    // Save formatting state of stream
    std::ios_base::fmtflags flags = std::cout.flags(std::cout.hex);
    int width = std::cout.width(2);

    for (int y=0; y<thumbail_.height(); y++) {
        for (int x=0; x<thumbail_.width(); x++) {
            std::cout << (int) thumbail_.getPixel(x, y) << " ";
        }
        std::cout << "\n";
    }

    std::cout.flags(flags);
    std::cout.width(width);
}

#endif