#include <iterator>
#include <type_traits>
#include <memory>
#include "construct.h"

namespace hstl{


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


}