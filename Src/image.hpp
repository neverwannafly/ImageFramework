#ifndef image_hpp
#define image_hpp

#include <cstring>

class apImage {
public:
    apImage();
    apImage(int width, int height);
    ~apImage();

    apImage(const apImage &src);
    apImage &operator=(const apImage &src);

    void swap(apImage &src);

    void setPixel(int x, int y, unsigned char pixel);
    unsigned char getPixel(int x, int y) const;

    bool isValid () const { return pixels_ != 0;}

    int width() const {return width_;}
    int height() const {return height_;}

    class RangeError {};

private:
    void init();
    void cleanup();

    int width_;
    int height_;
    unsigned char *pixels_;

};

void apImage::cleanup() {
    delete [] pixels_;
    width_ = 0;
    height_ = 0;
    pixels_ = 0;
}

void apImage::init() {
    if (width_ > 0 && height_ > 0) {
        pixels_ = new unsigned char[width_ * height_];
    }
}

apImage::apImage(): width_(0), height_(0), pixels_(0) {};
apImage::apImage(int width, int height): width_(width), height_(height), pixels_(0) {
    init();
}
apImage::~apImage() {
    cleanup();
}

apImage::apImage(const apImage &src) : width_(0), height_(0), pixels_(0) {
    if (src.isValid()) {
        width_ = src.width();
        height_ = src.height();
        init();
        memcpy(pixels_, src.pixels_, width_*height_);
    }
}

template<class Type> void swap(Type &a, Type &b) {
    Type copy(a);
    a = b;
    b = copy;
}

void apImage::swap(apImage &src) {
    ::swap(width_, src.width_);
    ::swap(height_, src.height_);
    ::swap(pixels_, src.pixels_);
}

apImage &apImage::operator=(const apImage &src) {
    apImage temp(src);
    swap(temp);
    return *this;
}

void apImage::setPixel(int x, int y, unsigned char pixel) {
    if (x<0 || y<0 || x>=width_ || y>=height_ || !isValid()) {
        throw RangeError();
    }
    unsigned char *p = pixels_ + y*width_ + x;
    *p = pixel;
}

unsigned char apImage::getPixel(int x, int y) const {
    if (x<0 || y<0 || x>=width_ || y>=height_ || !isValid()) {
        throw RangeError();
    }
    unsigned char *p = pixels_ + y*width_ + x;
    return *p;
}

#endif