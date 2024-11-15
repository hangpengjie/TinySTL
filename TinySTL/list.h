#ifndef TINYSTL_LIST_H
#define TINYSTL_LIST_H

#include <cstddef>
#include <iostream>
#include <cassert>
#include "allocator.h"
#include "uninitialized.h"

namespace hstl{

template <typename T>
class list_node{
public:
    typedef list_node<T>* pointer;
    pointer prev;
    pointer next;
    T data;
    
    list_node() = default;
    list_node(const T& value) : data(value) {}
    list_node(const T&& value) : data(std::move(value)) {}
};

template <typename T>
class list_iterator{
public:

    typedef list_iterator<T>    self;

    typedef std::bidirectional_iterator_tag iterator_category;
    typedef T                   value_type;
    typedef T*                  pointer;
    typedef T&                  reference;
    typedef list_node<T>*       node_pointer;
    typedef std::ptrdiff_t      difference_type;

    node_pointer node_;

    list_iterator() = default;
    list_iterator(node_pointer x) : node_(x) {}
    list_iterator(const self& rhs) : node_(rhs.node_) {}
    
    reference operator*() const { return node_->data; }
    pointer operator->() const { return &(operator*()); }

    self& operator++(){
        assert(node_ != nullptr);
        node_ = node_->next;
        return *this;
    }

    self operator++(int){
        self tmp = *this;
        ++*this;
        return tmp;
    }

    self& operator--(){
        assert(node_ != nullptr);
        node_ = node_->prev;
        return *this;
    }

    self operator--(int){
        self tmp = *this;
        --*this;
        return tmp;
    }

    bool operator==(const self& rhs) const { return node_ == rhs.node_; }
    bool operator!=(const self& rhs) const { return node_ != rhs.node_; }

};

template <typename T>
class list{
public:
    typedef hstl::allocator<T>                          allocator_type;
    typedef hstl::allocator<T>                          data_allocator;
    typedef hstl::allocator<list_node<T>>               node_allocator;
    

    typedef typename allocator_type::value_type         value_type;
    typedef typename allocator_type::pointer            pointer;
    typedef typename allocator_type::const_pointer      const_pointer;
    typedef typename allocator_type::reference          reference;
    typedef typename allocator_type::const_reference    const_reference;
    typedef typename allocator_type::size_type          size_type;
    typedef typename allocator_type::difference_type    difference_type;

    typedef list_iterator<T>                            iterator;
    typedef typename iterator::node_pointer             node_ptr;

private:
    node_ptr node_;
    size_type size_;

public:
    list();
    explicit list(size_type n);
    explicit list(size_type n, const value_type& value);
    
    template <typename InputIterator, typename = typename std::enable_if<
        std::is_convertible<typename std::iterator_traits<InputIterator>::iterator_category, std::input_iterator_tag>::value>::type>
    list(InputIterator first, InputIterator last);

    list(std::initializer_list<value_type> ilist);
    list(list& rhs);
    list(list&& rhs);

    ~list();

    list& operator=(list& rhs);
    list& operator=(list&& rhs);
    list& operator=(std::initializer_list<value_type> ilist);
    

    iterator begin();
    iterator end();
    size_type size();
    template <typename InputIterator>
    iterator insert(iterator pos ,InputIterator first, InputIterator last);

    iterator erase(iterator first, iterator last);

    void clear();
    
private:
    void fill_init(size_type n, const value_type& value);

    template <typename InputIterator>
    void copy_init(InputIterator first, InputIterator last);

    template <typename ...Args>
    node_ptr create_node(Args &&... args);

    void destory_node(node_ptr p);

    void link_nodes(node_ptr pos, node_ptr first, node_ptr last);

    template <typename InputIterator>
    void copy_assign(InputIterator first, InputIterator last);

    void unlink_nodes(node_ptr first, node_ptr last);
};


template <typename T>
list<T>::list(): node_(node_allocator::allocate()), size_(0){
    node_->next = node_->prev = node_;
}

template <typename T>
list<T>::list(size_type n){
    fill_init(n, value_type());
}

template <typename T>
list<T>::list(size_type n, const value_type& value){
    fill_init(n, value);
}


template <typename T>
template <typename InputIterator, typename>
list<T>::list(InputIterator first, InputIterator last){
    copy_init(first, last);
}

template <typename T>
list<T>::list(std::initializer_list<value_type> ilist){
    copy_init(ilist.begin(), ilist.end());
}
template <typename T>
list<T>::list(list& rhs){
    copy_init(rhs.begin(), rhs.end());
}


template <typename T>
list<T>::list(list&& rhs){
    node_ = rhs.node_;
    size_ = rhs.size_;
    rhs.node_ = nullptr;
    rhs.size_ = 0;
}

template <typename T>
list<T>::~list(){
    clear();
    node_allocator::deallocate(node_);
}

template <typename T>
list<T>& list<T>::operator=(list& rhs){
    if(this != &rhs){
        copy_assign(rhs.begin(), rhs.end());
    }
    return *this;
}

template <typename T>
list<T>& list<T>::operator=(list&& rhs){
    if(this != &rhs){
        clear();
        node_allocator::deallocate(node_);
        node_ = rhs.node_;
        size_ = rhs.size_;
        rhs.node_ = nullptr;
        rhs.size_ = 0;
    }
    return *this;
    
}

template <typename T>
list<T>& list<T>::operator=(std::initializer_list<value_type> ilist){
    list tmp(ilist);
    std::swap(this->node_, tmp.node_);
    std::swap(this->size_, tmp.size_);
    return *this;
}

template <typename T>
typename list<T>::iterator list<T>::begin(){
    return node_->next;
}

template <typename T>
typename list<T>::iterator list<T>::end(){
    return node_;
}

template <typename T>
typename list<T>::size_type list<T>::size(){
    return size_;
}

template <typename T>
template <typename InputIterator>
typename list<T>::iterator list<T>::insert(iterator pos, InputIterator first, InputIterator last){
    for(; first != last; ++first){
        auto cur = create_node(*first);
        link_nodes(pos.node_, cur, cur);
        ++size_;
    }
    return pos;
}

template <typename T>
typename list<T>::iterator list<T>::erase(iterator first, iterator last){
    if(first == last) return last;
    unlink_nodes(first.node_, last.node_->prev);
    while(first != last){
        node_ptr cur = first.node_;
        ++first;
        destory_node(cur);
        --size_;
    }
    return last;
}

template <typename T>
void list<T>::clear(){
    node_ptr cur = node_->next;
    while(cur != node_){
        node_ptr tmp = cur;
        cur = cur->next;
        destory_node(tmp);
    }
    node_->next = node_->prev = node_;
    size_ = 0;
}

template <typename T>
void list<T>::fill_init(size_type n, const value_type& value){
    node_ = node_allocator::allocate();
    node_->next = node_->prev = node_;
    size_ = n;
    try{
        for(; n > 0; --n){
            auto cur = create_node(value);
            link_nodes(node_, cur, cur);
        }
    }catch(...){
        clear();
        node_allocator::deallocate(node_);
        throw;
    }
}

template <typename T>
template <typename InputIterator>
void list<T>::copy_init(InputIterator first, InputIterator last){
    node_ = node_allocator::allocate();
    node_->next = node_->prev = node_;
    size_ = 0;
    try{
        for(; first != last; ++first){
            auto cur = create_node(*first);
            link_nodes(node_, cur, cur);
            ++size_;
        }
    }catch(...){
        clear();
        node_allocator::deallocate(node_);
        throw;
    }
}


template <typename T>
template <typename ...Args>
typename list<T>::node_ptr list<T>::create_node(Args &&... args){
    node_ptr p = node_allocator::allocate();
    try{
        hstl::construct(&(p->data), std::forward<Args>(args)...);
        p->next = p->prev = nullptr;
    }catch(...){
        node_allocator::deallocate(p);
        throw;
    }
    return p;
}


template <typename T>
void list<T>::destory_node(node_ptr p){
    hstl::destroy(&(p->data));
    node_allocator::deallocate(p);
}

template <typename T>
void list<T>::link_nodes(node_ptr pos, node_ptr first, node_ptr last){
    pos->prev->next = first;
    first->prev = pos->prev;
    last->next = pos;
    pos->prev = last;
}


template <typename T>
template <typename InputIterator>
void list<T>::copy_assign(InputIterator first, InputIterator last){
    auto cur = begin();
    for(; first != last && cur != end(); ++first, ++cur){
        *cur = *first;
    }
    if(first == last){
        erase(cur, end());
    }else{
        insert(end(), first, last);
    }
}

template <typename T>
void list<T>::unlink_nodes(node_ptr first, node_ptr last){
    first->prev->next = last->next;
    last->next->prev = first->prev;
}




}
#endif