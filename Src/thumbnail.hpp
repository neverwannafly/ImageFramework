#ifndef thumbnail_hpp
#define thumbnail_hpp

#include "image.hpp"

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

#endif