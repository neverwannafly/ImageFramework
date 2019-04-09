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
    apImage thumbnail_;
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

    for (int y=0; y<thumbnail_.height(); y++) {
        for (int x=0; x<thumbnail_.width(); x++) {
            std::cout << (int) thumbnail_.getPixel(x, y) << " ";
        }
        std::cout << "\n";
    }

    std::cout.flags(flags);
    std::cout.width(width);
}

void apThumbnail::createThumbnail(const char *inputFile, const char *outputFile, int factor) {
    if (inputFile==0 || outputFile==0 || factor<1) {
        throw Invalid();
    }

    readImage(inputFile);

    if (!image_.isValid()) {
        throw Invalid();
    }

    thumbnail_ = apImage(image_.width()/factor, image_.height()/factor);
    try {
        for (int y=0; y<thumbnail_.height(); y++) {
            for (int x=0; x<thumbnail_.width(); x++) {
                unsigned char pixel = averagePixels(x*factor, y*factor, factor);
                thumbnail_.setPixel(x, y, pixel);
            }
        }
    } catch(apImage::RangeError) {
        throw Invalid();
    }

    writeImage(outputFile);
}

unsigned char apThumbnail::averagePixels(int x0, int y0, int factor) {
    int sum = 0;
    try {
        for (int y=0; y<factor; y++) {
            for (int x=0; x<factor; x++) {
                sum += image_.getPixel(x+x0, y+y0);
            }
        }
    } catch(apImage::RangeError) {
        throw Invalid();
    }

    return static_cast<unsigned char>(sum/(factor*factor));

}

#endif