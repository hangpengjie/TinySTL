#ifndef TINY_VECTOR_H
#define TINY_VECTOR_H

#include <cstddef>
#include <iostream>
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
    // vector(size_type n, const value_type& value);
    // vector(const vector& rhs);
    // vector(vector&& rhs) noexcept;
    // vector(std::initializer_list<value_type> ilist);
    
    // ~vector();

    // vector& operator=(const vector& rhs);
    // vector& operator=(vector&& rhs) noexcept;
    // vector& operator=(std::initializer_list<value_type> ilist);

public:
    // iterator begin() noexcept;
    // const_iterator begin() const noexcept;
    // iterator end() noexcept;
    // const_iterator end() const noexcept;

    size_type size() const noexcept;
    // size_type capacity() const noexcept;
    // bool empty() const noexcept;

    // void reserve(size_type new_cap);
    // void resize(size_type count);
    // void resize(size_type count, const value_type& value);

    // void push_back(const value_type& value);
    // void push_back(value_type&& value);

    // template <typename... Args>
    // void emplace_back(Args&&... args);

    // void pop_back();

    // void clear() noexcept;

    // reference operator[](size_type n);
    // const_reference operator[](size_type n) const;
private:
    void try_init() noexcept;
    void fill_init(size_type init_size, const value_type& value);     // TODO
    void init_place(size_type init_size, size_type init_cap);
};

template <typename T>
vector<T>::vector() noexcept{
    try_init();
    
}

template <typename T>
vector<T>::vector(size_type n){
    
}


template<typename T>
typename vector<T>::size_type vector<T>::size() const noexcept{
    return end_ - begin_;
}


template <typename T>
void vector<T>::try_init() noexcept{
    try{
        begin_ = data_allocator::allocate(16);
        end_ = cap_ = begin_ + 16;
    }catch(...){
        begin_ = end_ = cap_ = nullptr;
    }
}


template <typename T>
void vector<T>::fill_init(size_type init_size, const value_type& value){
    const size_type init_cap = std::max(static_cast<size_type>(16), init_size);
    init_place(init_size, init_cap);
       
}


template <typename T>
void vector<T>::init_place(size_type init_size, size_type init_cap){
    try{
        begin_ = data_allocator::allocate(init_cap);
        end_ = begin_ + init_size;
        cap_ = begin_ + init_cap;
    }catch(...){
        begin_ = end_ = cap_ = nullptr;
        throw;
    }
}








}


#endif