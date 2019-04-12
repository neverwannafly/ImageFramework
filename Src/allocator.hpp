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

#endif