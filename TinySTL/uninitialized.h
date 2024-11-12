#include <iterator>
#include <type_traits>
#include <memory>
#include "construct.h"

namespace hstl{

/**
 *  uninitialized_fill_n
 */
template <typename ForwardIterator, typename Size, typename T>
ForwardIterator __uninitialized_fill_n_aux(ForwardIterator first, Size n, const T& x, std::true_type) {
    return std::fill_n(first, n, x);
}

template <typename ForwardIterator, typename Size, typename T>
ForwardIterator __uninitialized_fill_n_aux(ForwardIterator first, Size n, const T& x, std::false_type) {
    ForwardIterator cur = first;
    try {
        for (; n > 0; --n, ++cur) {
            hstl::construct(&*cur, x);
        }
        return cur;
    } catch (...) {
        for (; first != cur; ++first) {
            hstl::destroy(&*first);
        }
        throw;
    }
}

template <typename ForwardIterator, typename Size, typename T>
ForwardIterator __uninitialized_fill_n(ForwardIterator first, Size n, const T& x){
    // std::is_pod is a type trait that checks if a type is a plain old data type.                        
    // std::is_trivially_copy_assignable is a type trait that checks if a type is trivially copyable.
    // std::is_pod is stricter than std::is_trivially_copy_assignable
    return __uninitialized_fill_n_aux(first, n, x, typename std::is_pod<typename std::iterator_traits<ForwardIterator>::value_type>());
}

template <typename ForwardIterator, typename Size, typename T>
ForwardIterator uninitialized_fill_n(ForwardIterator first, Size n, const T& x){
    return __uninitialized_fill_n(first, n, x);
}

/**
 * uninitialized_copy
 */

template <typename InputIterator, typename ForwardIterator>
ForwardIterator __uninitialized_copy_aux(InputIterator first, InputIterator last, ForwardIterator result, std::true_type) {
    return std::copy(first, last, result);
}

template <typename InputIterator, typename ForwardIterator>
ForwardIterator __uninitialized_copy_aux(InputIterator first, InputIterator last, ForwardIterator result, std::false_type) {
    ForwardIterator cur = result;
    try {
        for (; first != last; ++first, ++cur) {
            hstl::construct(&*cur, *first);
        }
        return cur;
    } catch (...) {
        for (; result != cur; ++result) {
            hstl::destroy(&*result);
        }
        throw;
    }
}

template <typename InputIterator, typename ForwardIterator>
ForwardIterator __uninitialized_copy(InputIterator first, InputIterator last, ForwardIterator result){
    // std::is_pod is a type trait that checks if a type is a plain old data type.                        
    // std::is_trivially_copy_assignable is a type trait that checks if a type is trivially copyable.
    // std::is_pod is stricter thanstd::is_trivially_copy_assignable
    return __uninitialized_copy_aux(first, last, result, typename std::is_pod<typename std::iterator_traits<InputIterator>::value_type>());
}

template <typename InputIterator, typename ForwardIterator>
ForwardIterator uninitialized_copy(InputIterator first, InputIterator last, ForwardIterator result){
    return __uninitialized_copy(first, last, result);
}


/**
 * uninitialized_move
 */

template <typename InputIterator, typename ForwardIterator>
ForwardIterator __uninitialized_move_aux(InputIterator first, InputIterator last, ForwardIterator result, std::true_type) {
    return std::move(first, last, result);
}

template <typename InputIterator, typename ForwardIterator>
ForwardIterator __uninitialized_move_aux(InputIterator first, InputIterator last, ForwardIterator result, std::false_type) {
    ForwardIterator cur = result;
    try {
        for (; first != last; ++first, ++cur) {
            hstl::construct(&*cur, std::move(*first));
        }
    }catch (...) {
        for (; result != cur; ++result) {
            hstl::destroy(&*result);
        }
        throw;
    }
    return cur;
}

template <typename InputIterator, typename ForwardIterator>
ForwardIterator __uninitialized_move(InputIterator first, InputIterator last, ForwardIterator result){
    // std::is_pod is a type trait that checks if a type is a plain old data type.                        
    // std::is_trivially_copy_assignable is a type trait that checks if a type is trivially copyable.
    // std::is_pod is stricter thanstd::is_trivially_copy_assignable
    return __uninitialized_move_aux(first, last, result, typename std::is_pod<typename std::iterator_traits<InputIterator>::value_type>());
}

template <typename InputIterator, typename ForwardIterator>
ForwardIterator uninitialized_move(InputIterator first, InputIterator last, ForwardIterator result){
    return __uninitialized_move(first, last, result);
}



}