#ifndef apAlloc_hpp
#define apAlloc_hpp

#include "allocator.hpp"

template<typename T, typename A = apAllocator_<T> >
class apAlloc {
public:
    static apAlloc &gNull();
    // We return this object for any null allocations
    // It actually allocates 1 byte to make all the member
    // functions valid
    
    apAlloc();
    // Null allocation Returns pointer to gNull() memory
    
    explicit apAlloc(unsigned int size, unsigned int align=0);
    ~apAlloc();
    
    apAlloc(const apAlloc &src);
    apAlloc &operator=(const apAlloc &src);
    // Need own copy constructor and assignment operator
    
    unsigned int size() const { return pMem_->size(); }
    unsigned int ref() const { return pMem_->ref(); }
    bool isNull() const { return pMem_ == gNull().pMem_; }
    
    const T* data() const { return *pMem_; }
    T* data() { return *pMem_; }
    // Access to the beginning of our memory location
    
    const T &operator[] (unsigned int index) const;
    T & operator[](unsigned int index);
    // Access a specific element. Throws the STL range_error if index is invalid
    
    virtual A* clone();
    // Duplicate the memory in the underlying allocator.
    
    void duplicate();
    // Breaks any reference counting and forces this object to have it's own copy.
    
protected:
    A *pMem_; // Pointer to our allocated memory
    static apAlloc *sNull_; // Our Null object
};

template<typename T, typename A>
apAlloc<T, A> *apAlloc<T, A>::sNull_ = 0; // just like int *apAlloc::sNull_ = 0

template<typename T, typename A>
apAlloc<T, A> &apAlloc<T, A>::gNull() { // just like int &apAlloc::gNull()
    if (!sNull_) {
        sNull_ = new apAlloc(0);
    }
    return sNull_;
}

template<typename T, typename A>
apAlloc<T, A>::apAlloc() : pMem_(0) {
    pMem_ = gNull().pMem_;
    pMem_->addRef();
}

template<typename T, typename A>
apAlloc<T, A> &apAlloc<T, A>::operator=(const apAlloc &src) {
    if (pMem_ == src.pMem_) return *this;
    
    // Remove reference from existing object. addRef() and subRef()
    // donot throw so we dont have to worry about catching an error
    
    pMem_->subRef();
    pMem_ = src.pMem_;
    pMem_->addRef();
    
    return *this;
}

template<typename T, typename A>
apAlloc<T, A>::apAlloc(const apAlloc &src) {
    pMem_->subRef();
    pMem_ = src.pMem_;
    pMem_->addRef();
}

template<typename T, typename A>
T &apAlloc<T,A>::operator[] (unsigned int index) {
    if (index >= size()) {
        throw std::range_error("Index out of range");
    }
    return *(data() + index);
}

template<typename T, typename A>
const T &apAlloc<T,A>::operator[] (unsigned int index) const {
    if (index >= size()) {
        throw std::range_error("Index out of range");
    }
    return *(data() + index);
}

template<typename T, typename A>
void apAlloc<T, A>::duplicate() {
    if (ref()==1) return;
    
    A* copy = clone();
    pMem_->subRef();
    pMem_ = copy();
    
}

// Does a shallow copy
template<typename T, typename A>
A *apAlloc<T, A>::clone() {
    A *copy = new A(pMem_->size(), pMem_->align());
    
    T *src = *pMem_;
    T *dst = *copy;
    std::copy(src, &(src[pMem_->size()]), dst);
    
    return copy;
}

#endif