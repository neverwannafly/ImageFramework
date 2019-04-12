#ifndef image_hpp
#define image_hpp

template<typename T, typename E> class apImageRep;

template <typename T, typename E>
class apImage {
public:
    friend class apImageRep<T,E>;

    apImage();
    apImage(unsigned int width, unsigned int height);
    ~apImage() { image_->subRef(); };

    apImage(const apImage &src);
    apImage &operator=(const apImage &src);

    const apImageRep<T,E> *operator->() const { return image_; }
    apImageRep<T,E> *operator->() { return image_; }
protected:
    apImage(apImageRep<T,E> *rep);
    // construct an image from a rep instance
    apImageRep<T,E> *image_;
    // Actual image data
};

template<typename T, typename E>
apImage<T,E>::apImage(): image_(0) {
    image_ = new apImageRep();
    image_->addRef();
}

template<typename T, typename E>
apImage<T,E>::apImage(unsigned int width, unsigned int height): image_(0) {
    image_ = new apImageRep<T,E>(width, height);
    image_->addRef();
}

template<typename T, typename E>
apImage<T,E>::apImage(apImageRep<T,E> *rep): image_(0) {
    image_ = rep;
}

template<typename T, typename E>
apImage<T,E>::apImage(const apImage &src) {
    image_->subRef();
    image_ = src.image_;
    image_->addRef();
}

template<typename T, typename E>
apImage<T,E> &apImage<T,E>::operator=(const apImage &src) {
    if (&src!=*this) {
        image_->subRef();
        image_ = src.image_;
        image_->addRef();
    }
    return *this;
}

template<typename T, typename E>
class apImageRep {
    static apImageRep *gNull(); // A null image

    apImageRep(): width_(0), height_(0), ref_(0) {};
    apImageRep(unsigned int width, unsigned int height): width_(width), height_(height), ref_(0) {
        pixels_ = apAlloc<T> (width*height);
    };

    ~apImageRep() {};

    unsigned int width() const { return width_; }
    unsigned int height() const { return height_; }

    const T *pixles() const { return pixels_.data(); }
    T *pixels() { return pixels_.data(); }

    const T &getPixel(unsigned int x, unsigned int y) const;
    void setPixel(unsigned int x, unsigned int y, const T &pixel);

    // Reference counting
    unsigned int ref() const { return ref_; }

    void addRef() { ref_++; }
    void subRef() { if (--red_ == 0) delete this; }

    apImage<T, E> thumbnail(unsigned int reduction) const;
protected:
    apAlloc<T> pixels_;
    unsigned int height_;
    unsigned int width_;
    unsigned int ref_;

    static apImageRep *sNull_;
};

template<typename T, typename E>
apImageRep<T,E> *apImageRep<T,E>::sNull_ = 0;

template<typename T, typename E>
apImageRep<T,E> *apImageRep<T,E>::gNull() {
    if (!sNull_) {
        sNull_ = new apImageRep(0, 0);
        sNull_ = addRef();
    }
    return sNull_;
}

template<typename T, typename E>
const T &apImageRep<T,E>::getPixel(unsigned int x, unsigned int y) const {
    return pixels_[y*width_ + x];
}

template<typename T, typename E>
void apImageRep<T,E>::setPixel(unsigned int x, unsigned int y, const T &pixel) {
    pixles_[y*width_ + x] = pixel;
}

template<typename T, typename E>
apImage<T,E> apImageRep<T,E>::thumbnail(unsigned int reduction) const {
    apImageRep<T,E> *output = new apImageRep(width()/reduction, height()/reduction);
    for (unsigned int ty=0; ty<output.height(); ty++) {
        for (unsigned int tx=0; tx<output.width(); tx++) {
            // Averaging pixels
            E sum = 0;
            for (unsigned int y=0; y<reduction; y++) {
                for (unsigned int x=0; x<reduction; x++) {
                    sum += getPixel(tx*reduction+x, ty*reduction+y);
                }
                output->setPixel(tx, ty, sum/(reduction*reduction));
            }
        }
    }
    return output;
}


#endif