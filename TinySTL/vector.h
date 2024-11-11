#ifndef TINY_VECTOR_H
#define TINY_VECTOR_H

#include <cstddef>
#include "allocator.h"

namespace hstl{

template <typename T>
class vector{
public:
    typedef hstl::allocator<T>                          allocator_type;
    typedef hstl::allocator<T>                          data_allocator;

    typedef typename allocator_type::value_type         value_type;
    typedef typename allocator_type::pointer            pointer;
    typedef typename allocator_type::const_pointer      const_pointer;
    typedef typename allocator_type::reference          reference;
    typedef typename allocator_type::const_reference    const_reference;
    typedef typename allocator_type::size_type          size_type;
    typedef typename allocator_type::difference_type    difference_type;


    typedef value_type*                                 iterator;
    typedef const value_type*                           const_iterator;


private:
    iterator begin_;    // Iterator pointing to the beginning of the vector.
    iterator end_;      // Iterator pointing to the end of the vector.
    iterator cap_;      // Iterator pointing to the capacity end of the vector.

public:
    vector() noexcept;
    explicit vector(size_type n);
    vector(size_type n, const value_type& value);
    vector(const vector& rhs);
    vector(vector&& rhs) noexcept;
    vector(std::initializer_list<value_type> ilist);
    
    ~vector();

    vector& operator=(const vector& rhs);
    vector& operator=(vector&& rhs) noexcept;
    vector& operator=(std::initializer_list<value_type> ilist);

public:
    iterator begin() noexcept;
    const_iterator begin() const noexcept;
    iterator end() noexcept;
    const_iterator end() const noexcept;

    size_type size() const noexcept;
    size_type capacity() const noexcept;
    bool empty() const noexcept;

    void reserve(size_type new_cap);
    void resize(size_type count);
    void resize(size_type count, const value_type& value);

    void push_back(const value_type& value);
    void push_back(value_type&& value);

    template <typename... Args>
    void emplace_back(Args&&... args);

    void pop_back();

    void clear() noexcept;

    reference operator[](size_type n);
    const_reference operator[](size_type n) const;
};









}


#endif