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

    inline bool isValid () const { return pixels_ != 0;}

    inline int width() const {return width_;}
    inline int height() const {return height_;}

    class rangeError {};

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

#endif