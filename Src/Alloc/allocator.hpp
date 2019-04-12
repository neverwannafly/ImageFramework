#ifndef allocator_hpp
#define allocator_hpp

#include "allocatorBase.hpp"

template<typename T>
class apAllocator_ : public apAllocatorBase_<T> {
public:
    explicit apAllocator_ (unsigned int n, unsigned int align = 0) {
        apAllocatorBase_<T> (n, align);
        allocate();
        apAllocatorBase_<T>::addRef();
    }
    virtual ~apAllocator_() { deallocate(); }
private:
    virtual void allocate() {
        if (apAllocatorBase_<T>::size_ == 0) {
            // Eliminating possibility of null pointer
            apAllocatorBase_<T>::pData_ = new T[1];
            apAllocatorBase_<T>::pRaw_ = 0;
            return;
        }
        if (apAllocatorBase_<T>::align_ < 2) {
            // No alignment needed
            apAllocatorBase_<T>::pData_ = new T[apAllocatorBase_<T>::size_];
            apAllocatorBase_<T>::pRaw_ = 0;
        } else {
            // Allocate additional bytes to guarantee alignment
            // Then align and cast to desired datatype
            apAllocatorBase_<T>::pRaw_ = new char[sizeof(T)*apAllocatorBase_<T>::size_ + (apAllocatorBase_<T>::align_-1)];
            apAllocatorBase_<T>::pData_ = alignPointer(apAllocatorBase_<T>::pRaw_);
        }
    }
    // Allocate our memory for size_ elements of type T with the alignment specified
    // by align_. 0 and 1 specify no alignment, 2 = word alignment, 4 = 4 byte
    // alignment. This must be a power of 2
    virtual void deallocate() {
        // Decide which pointer to delete
        if (apAllocatorBase_<T>::pRaw_)
            delete [] apAllocatorBase_<T>::pRaw_;
        else
            delete [] apAllocatorBase_<T>::pData_;
        apAllocatorBase_<T>::pRaw_ = 0;
        apAllocatorBase_<T>::pData_ = 0;
    }
    
    apAllocator_(const apAllocator_ &src);
    apAllocator_ &operator=(const apAllocator_ &src);
    // No copy or assignment is allowed
};

#endif