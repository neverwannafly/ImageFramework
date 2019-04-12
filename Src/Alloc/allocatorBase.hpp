#ifndef allocatorBase_hpp
#define allocatorBase_hpp

#include <iostream>

template<typename T>
class apAllocatorBase_ {
public:
    apAllocatorBase_(unsigned int n, unsigned int align): pRaw_(0), pData_(0), ref_(0), size_(n), align_(align) {}
    virtual ~apAllocatorBase_() {}
    
    operator T*() { return pData_; }
    operator const T*() const { return pData_; }
    // Conversion to pointer of allocated memory type
    
    unsigned int size() const { return size_; } // Number of elements
    unsigned int ref() const { return ref_; } // Number of references
    unsigned int align() const { return align_; } // Alignment
    
    void addRef() { ref_++; }
    void subRef() { if (--ref_ == 0) delete this; }
    // Increment/Decrement reference counter
    
protected:
    virtual void allocate() = 0;
    virtual void deallocate() = 0;
    // Pure Virtual Functions
    
    T *alignPointer(void *raw) {
        T *p = reinterpret_cast<T*>((reinterpret_cast<uintptr_t>(raw) + align_ -1) & ~(align_-1));
        return p;
    }
    // Align a specified pointer to match alignment
    
    apAllocatorBase_(const apAllocatorBase_ &src);
    apAllocatorBase_ &operator=(const apAllocatorBase_ &src);
    // No copy or assignment would be allowed
    
    char * pRaw_; // Raw allocated pointer
    T *pData_; // Aligned pointer to our memory
    unsigned int size_; // Number of elements allocated
    unsigned int ref_;  // Reference count
    unsigned int align_; // Memory alignment
    
};

#endif