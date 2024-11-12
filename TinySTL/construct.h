#ifndef TINYSTL_CONSTRUCT_H
#define TINYSTL_CONSTRUCT_H

#include <new>
#include <type_traits>
#include <utility>

namespace hstl{

template <typename T>
inline void construct(T* ptr){
    ::new(static_cast<void*>(ptr)) T();
}

template <typename Ty1, typename Ty2>
inline void construct(Ty1* ptr, const Ty2& value){
    ::new(static_cast<void*>(ptr)) T(value);
}

template <typename T, typename... Args>
inline void construct(T* ptr, Args&&... args){
    ::new(static_cast<void*>(ptr)) T(std::forward<Args>(args)...);
}

template <typename T>
inline void destroy(T* ptr, std::true_type){
}

template <typename T>
inline void destroy(T* ptr, std::false_type){
    if(ptr == nullptr) return;
    ptr->~T();
}

template <typename T>
inline void destroy(T* ptr){
    destroy(ptr, std::is_trivially_destructible<T>());
}

};

#endif