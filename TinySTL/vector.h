#ifndef TINY_VECTOR_H
#define TINY_VECTOR_H

#include <cstddef>
#include <iostream>
#include <cassert>
#include "allocator.h"
#include "uninitialized.h"


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
    size_type max_size() const noexcept;
    size_type capacity() const noexcept;
    reference operator[](size_type n);
    const_reference operator[](size_type n) const;
    bool empty() const noexcept;

    void reserve(size_type new_cap);
    void resize(size_type new_size);
    void resize(size_type new_size, const value_type& value);

    void push_back(const value_type& value);
    void push_back(value_type&& value);
    
    iterator insert(iterator position, const value_type& value);
    iterator insert(iterator position, value_type&& value);
    iterator insert(iterator position, size_type n, const value_type& value);
    

    template <typename... Args>
    iterator emplace(iterator position, Args&&... args);

    template <typename... Args>
    void emplace_back(Args&&... args);

    void pop_back();
    iterator erase(iterator position);
    iterator erase(iterator first, iterator last);

    void clear() noexcept;
    void swap(vector& rhs) noexcept;
private:
    void try_init() noexcept;
    void fill_init(size_type init_size, const value_type& value);
    void init_place(size_type init_size, size_type init_cap);
    
    template <typename InputIterator>
    void range_init(InputIterator first, InputIterator last);

    void reallocate_insert(iterator position, const value_type& value);
    
    template <typename ... Args>
    void reallocate_emplace(iterator position, Args&&... args);

    size_type get_new_cap(size_type add_size) const noexcept;
};

template <typename T>
vector<T>::vector() noexcept{
    try_init();
}

template <typename T>
vector<T>::vector(size_type n){
    fill_init(n, value_type());
}

template <typename T>
vector<T>::vector(size_type n, const value_type& value){
    fill_init(n, value);
}

template <typename T>
vector<T>::vector(const vector& rhs){
    range_init(rhs.begin_, rhs.end_);
}

template <typename T>
vector<T>::vector(vector&& rhs) noexcept : begin_(rhs.begin_), end_(rhs.end_), cap_(rhs.cap_){
    rhs.begin_ = rhs.end_ = rhs.cap_ = nullptr;
}

template <typename T>
vector<T>::vector(std::initializer_list<value_type> ilist){
    range_init(ilist.begin(), ilist.end());
}


template <typename T>
vector<T>::~vector(){
    data_allocator::destroy(begin_, end_);
    data_allocator::deallocate(begin_, cap_ - begin_);
    begin_ = end_ = cap_ = nullptr;
}

template <typename T>
vector<T>& vector<T>::operator=(const vector& rhs){
    if(this != &rhs){
        const size_type len = rhs.size();
        if(len > capacity()){
            vector tmp(rhs);
            std::swap(begin_, tmp.begin_);
            std::swap(end_, tmp.end_);
            std::swap(cap_, tmp.cap_);
        }else{
            data_allocator::destroy(begin_, end_);
            end_ = hstl::uninitialized_copy(rhs.begin(), rhs.end(), begin_);
        }
        
    }
    return *this;
}

template <typename T>
vector<T>& vector<T>::operator=(vector&& rhs) noexcept{
    if(this != &rhs){
        data_allocator::destroy(begin_, end_);
        data_allocator::deallocate(begin_, cap_ - begin_);
        begin_ = rhs.begin_;
        end_ = rhs.end_;
        cap_ = rhs.cap_;
        rhs.begin_ = rhs.end_ = rhs.cap_ = nullptr;
    }
    return *this;
}


template <typename T>
typename vector<T>::iterator vector<T>::begin() noexcept{
    return begin_;
}
template <typename T>
typename vector<T>::const_iterator vector<T>::begin() const noexcept{
    return begin_;
}
template <typename T>
typename vector<T>::iterator vector<T>::end() noexcept{
    return end_;
}
template <typename T>
typename vector<T>::const_iterator vector<T>::end() const noexcept{
    return end_;
}

template<typename T>
typename vector<T>::size_type vector<T>::size() const noexcept{
    return end_ - begin_;
}

template<typename T>
typename vector<T>::size_type vector<T>::max_size() const noexcept{
    return static_cast<size_type>(-1) / sizeof(T);
}

template<typename T>
typename vector<T>::size_type vector<T>::capacity() const noexcept{
    return cap_ - begin_;
}

template<typename T>
typename vector<T>::reference vector<T>::operator[](size_type n){
    return *(begin_ + n);
}

template<typename T>
typename vector<T>::const_reference vector<T>::operator[](size_type n) const{
    return *(begin_ + n);
}

template <typename T>
void vector<T>::try_init() noexcept{
    try{
        begin_ = data_allocator::allocate(16);
        end_ = begin_;
        cap_ = begin_ + 16;
    }catch(...){
        begin_ = end_ = cap_ = nullptr;
    }
}

template <typename T>
bool vector<T>::empty() const noexcept{
    return begin_ == end_;
}

template <typename T>
void vector<T>::reserve(size_type new_cap){
    if(capacity() < new_cap){
        assert(new_cap < max_size());
        auto old_size = size();
        auto new_begin = data_allocator::allocate(new_cap);
        hstl::uninitialized_move(begin_, end_, new_begin);
        data_allocator::deallocate(begin_, cap_ - begin_);
        begin_ = new_begin;
        end_ = begin_ + old_size;
        cap_ = begin_ + new_cap;
    }    
}


template <typename T>
void vector<T>::resize(size_type new_size){
    resize(new_size, value_type());
}

template <typename T>
void vector<T>::resize(size_type new_size, const value_type& value){
    if(new_size < size()){
        erase(begin() + new_size, end());
    }else{
        insert(end(), new_size - size(), value);
    }
}

template <typename T>
void vector<T>::push_back(const value_type& value){
    if(end_ != cap_){
        data_allocator::construct(end_++, value);
    }else{
        reallocate_insert(end_, value);
    }
}

template <typename T>
void vector<T>::push_back(value_type&& value){
    emplace_back(std::move(value));
}

template <typename T>
typename vector<T>::iterator vector<T>::insert(iterator position, const value_type& value){
    assert(position >= begin() && position <= end());
    const size_type xpos = position - begin_;
    if(position == end_ && end_ != cap_){
        data_allocator::construct(end_++, value);
    } else if (end_ != cap_){
        std::copy_backward(position, end_, end_ + 1);
        data_allocator::construct(position, value);
        ++end_;
    }else{
        reallocate_insert(position, value);
    }
    return begin_ + xpos;
}

template <typename T>
typename vector<T>::iterator vector<T>::insert(iterator position, value_type&& value){
    return emplace(position, std::move(value));
}

template <typename T>
typename vector<T>::iterator vector<T>::insert(iterator position, size_type n, const value_type& value){
    assert(position >= begin() && position <= end());
    if(n == 0) return position;
    const size_type xpos = position - begin_;
    if(n <= static_cast<size_type>(cap_ - end_)){
        std::copy_backward(position, end_, end_ + n);
        hstl::uninitialized_fill_n(position, n, value);
        end_ += n;
    }else{
        const size_type new_cap = get_new_cap(n);
        iterator new_begin = data_allocator::allocate(new_cap);
        iterator new_end = new_begin;
        try{
            new_end = hstl::uninitialized_move(begin_, position, new_begin);
            new_end = hstl::uninitialized_fill_n(new_end, n,value);
            new_end = hstl::uninitialized_move(position, end_, new_end);
        }catch(...){
            data_allocator::destroy(new_begin, new_end);
            data_allocator::deallocate(new_begin, new_cap);
            throw;
        }
        data_allocator::deallocate(begin_, cap_ - begin_);
        begin_ = new_begin;
        end_ = new_end;
        cap_ = new_begin + new_cap;
    }
    return begin_ + xpos;
}


template <typename T>
template <typename... Args>
typename vector<T>::iterator  vector<T>::emplace(iterator position, Args&& ...args){
    assert(position >= begin() && position <= end());
    const size_type xpos = position - begin_;
    if(position == end_ && end_ != cap_){
        data_allocator::construct(end_++, std::forward<Args>(args)...);
    } else if (end_ != cap_){
        std::copy_backward(position, end_, end_ + 1);
        data_allocator::construct(position, std::forward<Args>(args)...);
        ++end_;
    }else{
        reallocate_emplace(position, std::forward<Args>(args)...);
    }
    return begin_ + xpos;
    
}

template <typename T>
template <typename... Args>
void vector<T>::emplace_back(Args&&... args){
    if(end_ != cap_){
        data_allocator::construct(end_++, std::forward<Args>(args)...);
    }else{
        reallocate_emplace(end_, std::forward<Args>(args)...);
    }
}

template <typename T>
void vector<T>::pop_back(){
    assert(!empty());
    data_allocator::destroy(--end_);
}

template <typename T>
typename vector<T>::iterator vector<T>::erase(iterator position){
    assert(position >= begin_ && position < end_);
    if(position + 1 != end_){
        std::move(position + 1, end_, position);
    }
    data_allocator::destroy(--end_);
    return position;
}

template <typename T>
typename vector<T>::iterator vector<T>::erase(iterator first, iterator last){
    assert(first >= begin_ && first <= last && last <= end_);
    iterator new_end = std::move(last, end_, first);
    data_allocator::destroy(new_end, end_);
    end_ = new_end;
    return first;
}

template <typename T>
void vector<T>::clear() noexcept{
    erase(begin(), end());
}

template <typename T>
void vector<T>::swap(vector& rhs) noexcept{
    if(this != &rhs){
        std::swap(begin_, rhs.begin_);
        std::swap(end_, rhs.end_);
        std::swap(cap_, rhs.cap_);
    }
}

template <typename T>
void vector<T>::fill_init(size_type init_size, const value_type& value){
    const size_type init_cap = std::max(static_cast<size_type>(16), init_size);
    init_place(init_size, init_cap);
    hstl::uninitialized_fill_n(begin_, init_size, value);
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

template <typename T>
template <typename InputIterator>
void vector<T>::range_init(InputIterator first, InputIterator last){
    const size_type init_size = static_cast<size_type>(std::distance(first, last));
    const size_type init_cap = std::max(static_cast<size_type>(16), init_size);
    init_place(init_size, init_cap);
    hstl::uninitialized_copy(first, last, begin_);
}

template <typename T>
void vector<T>::reallocate_insert(iterator position, const value_type& value){
    const size_type new_cap = get_new_cap(1);
    iterator new_begin = data_allocator::allocate(new_cap);
    iterator new_end = new_begin;
    try{
        new_end = hstl::uninitialized_move(begin_, position, new_begin);
        data_allocator::construct(new_end++, value);
        new_end = hstl::uninitialized_move(position, end_, new_end);
    }catch(...){
        data_allocator::deallocate(new_begin, new_cap);
        throw;
    }
    hstl::destroy(begin_, end_);
    data_allocator::deallocate(begin_, cap_ - begin_);
    begin_ = new_begin;
    end_ = new_end;
    cap_ = begin_ + new_cap;
}

template  <typename T>
template <typename ... Args>
void vector<T>::reallocate_emplace(iterator position, Args&&... args){
    const size_type new_cap = get_new_cap(1);
    iterator new_begin = data_allocator::allocate(new_cap);
    iterator new_end = new_begin;
    try{
        new_end = hstl::uninitialized_move(begin_, position, new_begin);
        data_allocator::construct(new_end++, std::forward<Args>(args)...);
        new_end = hstl::uninitialized_move(position, end_, new_end);
    }catch(...){
        data_allocator::deallocate(new_begin, new_cap);
        throw;
    }
    hstl::destroy(begin_, end_);
    data_allocator::deallocate(begin_, cap_ - begin_);
    begin_ = new_begin;
    end_ = new_end;
    cap_ = begin_ + new_cap;
}

template <typename T>
typename vector<T>::size_type vector<T>::get_new_cap(size_type add_size) const noexcept{
    const size_type old_cap = capacity();
    assert(old_cap  <= max_size() - add_size);
    if (old_cap > max_size() - old_cap){
        return old_cap + add_size > max_size() - 16 ? old_cap + add_size : old_cap + add_size + 16;
    }
    const size_type new_cap = old_cap == 0 ? std::max(add_size, static_cast<size_type>(16)) : std::max(old_cap + old_cap / 2, old_cap + add_size);
    return new_cap;
}

}


#endif