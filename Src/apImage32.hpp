#ifndef apImage32_hpp
#define apImage32_hpp

#include "Alloc/apAlloc.hpp"
#include "thumbnail.hpp"

typedef unsigned int Pix32;

class apImage32 {
public:
    apImage32();
    apImage32(int width, int height);

    virtual ~apImage32();

    int width() const { return width_; }
    int height() const { return height_; }

    const Pix32 *pixels() const { return pixels_.data(); }
    Pix32 *pixels() { return pixels_.data(); }

    Pix32 getPixel(int x, int y);
    void setPixel(int x, int y, Pix32 pixel);

    // Image operations
    virtual apImage32 thumbnail(int reduction) const;

protected:
    apAlloc<Pix32> pixels_;
    int width_;
    int height_;
};

#endif