#ifndef allocator_hpp
#define allocator_hpp

template<typename T> 
class apAllocatorBase_ {
public:
    apAllocatorBase_(unsigned int n, unsigned int align): pRaw_(0), pData(0), ref_(0), size_(n), align_(align) {}
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

template<typename T>
class apAllocator_ : public apAllocatorBase_<T> {
public:
    explicit apAllocator_ (unsigned int n, unsigned int align = 0): apAllocatorBase_<T> (n, align) {
        allocate();
        addRef();
    }
    virtual ~apAllocator_() { deallocate(); }
private:
    virtual void allocate() {
        if (size_ == 0) {
            // Eliminating possibility of null pointer
            pData_ = new T[1];
            pRaw_ = 0;
            return;
        }
        if (align_ < 2) {
            // No alignment needed
            pData_ = new T[size_];
            pRaw_ = 0;
        } else {
            // Allocate additional bytes to guarantee alignment
            // Then align and cast to desired datatype
            pRaw_ = new char[sizeof(T)*size_ + (align_-1)];
            pData_ = alignPointer(pRaw_);
        }
    }
    // Allocate our memory for size_ elements of type T with the alignment specified
    // by align_. 0 and 1 specify no alignment, 2 = word alignment, 4 = 4 byte
    // alignment. This must be a power of 2
    virtual void deallocate() {
        // Decide which pointer to delete
        if (pRaw_)
            delete [] pRaw_;
        else
            delete [] pData_;
        pRaw_ = 0;
        pData_ = 0;
    }

    apAllocator_(const apAllocator_ &src);
    apAllocator_ &operator=(const apAllocator_ &src);
    // No copy or assignment is allowed
};

#endif