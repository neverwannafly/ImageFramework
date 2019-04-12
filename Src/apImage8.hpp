#ifndef apImage8_hpp
#define apImage8_hpp

#include "thumbnail.hpp"
#include "Alloc/apAlloc.hpp"

typedef unsigned char Pix8;

class apImage8 {
public:
    apImage8();
    apImage8(int width, int height);

    virtual ~apImage8();

    int width() const { return width_; }
    int height() const { return height_; }

    const Pix8 *pixels() const { return pixels_.data(); }
    Pix8 *pixles() { return pixels_.data(); }

    Pix8 getPixel(int x, int y) const;
    void setPixel(int x, int y, Pix8 pixel);

    // Image operations
    virtual apImage8 thumbnail(int reduction) const;

protected:
    apAlloc<Pix8> pixels_;
    int width_;
    int height_;
};

#endif