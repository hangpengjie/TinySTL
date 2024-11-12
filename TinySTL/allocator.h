#ifndef TINYSTL_ALLOCATOR_H
#define TINYSTL_ALLOCATOR_H

#include "construct.h"
#include <cstddef>
#include <utility>

namespace hstl {

template <typename T>
class allocator {
public:
    typedef T           value_type;
    typedef T*          pointer;
    typedef const T*    const_pointer;
    typedef T&          reference;
    typedef const T&    const_reference;
    typedef size_t      size_type;
    typedef ptrdiff_t   difference_type;

public:
    static T *allocate();
    static T *allocate(size_type n);

    static void deallocate(T *ptr);
    static void deallocate(T *ptr, size_type n);

    static void construct(T *ptr);
    static void construct(T *ptr, const T &value);  
    static void construct(T *ptr, T &&value);

    template <typename... Args>
    static void construct(T* ptr, Args&& ...args);

    static void destroy(T *ptr);

    template <typename ForwardIterator>
    static void destroy(ForwardIterator first, ForwardIterator last);
};


template <typename T>
T *allocator<T>::allocate() {
    return static_cast<T *>(::operator new(sizeof(T)));
}

template <typename T>
T *allocator<T>::allocate(size_type n) {
    if (n == 0) return nullptr;
    return static_cast<T *>(::operator new(n * sizeof(T)));
}

template <typename T>
void allocator<T>::deallocate(T *ptr) {
    if (ptr == nullptr) return;
    ::operator delete(ptr);
}

template <typename T>
void allocator<T>::deallocate(T *ptr, size_type n) {
    if (ptr == nullptr) return;
    // release memory c++14 or later
    ::operator delete(ptr, n * sizeof(T)); 
}

template <typename T>
void allocator<T>::construct(T *ptr) {
    hstl::construct(ptr);
}

template <typename T>
void allocator<T>::construct(T *ptr, const T &value) {
    hstl::construct(ptr, value);
}

template <typename T>
void allocator<T>::construct(T *ptr, T &&value) {
    hstl::construct(ptr, std::move(value));
}

template <typename T>
template <typename... Args>
void allocator<T>::construct(T* ptr, Args&& ...args) {
    hstl::construct(ptr, std::forward<Args>(args)...);
}

template <typename T>
void allocator<T>::destroy(T *ptr) {
    hstl::destroy(ptr);
}

template <typename T>
template <typename ForwardIterator>
void allocator<T>::destroy(ForwardIterator first, ForwardIterator last) {
    hstl::destroy(first, last);
}


} // namespace hstl

#endif // TINYSTL_ALLOCATOR_H