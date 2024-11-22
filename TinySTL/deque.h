#ifndef TINYSTL_DEQUE_H
#define TINYSTL_DEQUE_H

#include <cstddef>
#include <iterator>
#include <cassert>
#include "allocator.h"
#include "construct.h"
#include "uninitialized.h"
namespace hstl{


#ifndef DEQUE_MAP_SIZE
#define DEQUE_MAP_SIZE 8
#endif

// buffer size
template <typename T>
class deque_buffer_size{
public:
    static constexpr std::size_t buffer_size = sizeof(T) < 256 ? 4096 / sizeof(T) : 16;
};


// deque iterator
template <typename T, typename Ref, typename Ptr>
class deque_iterator{
public:
    typedef deque_iterator<T, T&, T*>                   iterator;
    typedef deque_iterator<T, const T&, const T*>       const_iterator;
    typedef deque_iterator                              self;

    typedef std::random_access_iterator_tag             iterator_category;
    typedef T                                           value_type;
    typedef Ptr                                         pointer;
    typedef Ref                                         reference;
    typedef std::size_t                                 size_type;
    typedef std::ptrdiff_t                              difference_type;
    typedef T*                                          value_pointer;
    typedef T**                                         map_pointer;

    static const size_type buffer_size = deque_buffer_size<T>::buffer_size;


    value_pointer   cur;
    value_pointer   first;
    value_pointer   last;
    map_pointer     node;

public:

    deque_iterator() noexcept;
    deque_iterator(value_pointer v_ptr, map_pointer n_ptr);
    deque_iterator(const iterator& rhs) noexcept;
    deque_iterator(iterator&& rhs) noexcept;
    deque_iterator(const const_iterator& rhs) noexcept;

    self& operator=(const self& rhs) noexcept;
    self& operator=(iterator&& rhs) noexcept;
    
    reference operator*() const;
    pointer operator->() const;

    difference_type operator-(const self& rhs) const;
    self& operator++();
    self& operator++(int);
    self& operator--();
    self& operator--(int);
    self& operator+=(difference_type n);
    self& operator-=(difference_type n);
    self operator+(difference_type n) const;
    self operator-(difference_type n) const;

    reference operator[](difference_type n) const;
    
    void set_node(map_pointer new_node);

    bool operator==(const self& rhs) const { return cur == rhs.cur; }
    bool operator!=(const self& rhs) const { return cur != rhs.cur; }
    bool operator<(const self& rhs) const { return node == rhs.node ? cur < rhs.cur : node < rhs.node; }
    bool operator>(const self& rhs) const { return rhs < *this; }
    bool operator<=(const self& rhs) const { return !(rhs < *this); }
    bool operator>=(const self& rhs) const { return !(*this < rhs); }
    
};

template <typename T, typename Ref, typename Ptr>
deque_iterator<T, Ref, Ptr>::deque_iterator() noexcept
    : cur(nullptr), first(nullptr), last(nullptr), node(nullptr){}

template <typename T, typename Ref, typename Ptr>
deque_iterator<T, Ref, Ptr>::deque_iterator(value_pointer v_ptr, map_pointer n_ptr)
    : cur(v_ptr), first(*n_ptr), last(*n_ptr + buffer_size), node(n_ptr){}

template <typename T, typename Ref, typename Ptr>
deque_iterator<T, Ref, Ptr>::deque_iterator(const iterator& rhs) noexcept
    : cur(rhs.cur), first(rhs.first), last(rhs.last), node(rhs.node){}

template <typename T, typename Ref, typename Ptr>
deque_iterator<T, Ref, Ptr>::deque_iterator(iterator&& rhs) noexcept
    : cur(rhs.cur), first(rhs.first), last(rhs.last), node(rhs.node){
    rhs.cur = nullptr;
    rhs.first = nullptr;
    rhs.last = nullptr;
    rhs.node = nullptr;
}

template <typename T, typename Ref, typename Ptr>
deque_iterator<T, Ref, Ptr>::deque_iterator(const const_iterator& rhs) noexcept
    : cur(rhs.cur), first(rhs.first), last(rhs.last), node(rhs.node){}


template <typename T, typename Ref, typename Ptr>
typename deque_iterator<T, Ref, Ptr>::self&
deque_iterator<T, Ref, Ptr>::operator=(const self& rhs) noexcept{
    cur = rhs.cur;
    first = rhs.first;
    last = rhs.last;
    node = rhs.node;
    return *this;
}

template <typename T, typename Ref, typename Ptr>
typename deque_iterator<T, Ref, Ptr>::self&
deque_iterator<T, Ref, Ptr>::operator=(iterator&& rhs) noexcept{
    cur = rhs.cur;
    first = rhs.first;
    last = rhs.last;
    node = rhs.node;
    rhs.cur = nullptr;
    rhs.first = nullptr;
    rhs.last = nullptr;
    rhs.node = nullptr;
    return *this;
}


template <typename T, typename Ref, typename Ptr>
typename deque_iterator<T, Ref, Ptr>::reference
deque_iterator<T, Ref, Ptr>::operator*() const{ return *cur; }

template <typename T, typename Ref, typename Ptr>
typename deque_iterator<T, Ref, Ptr>::pointer
deque_iterator<T, Ref, Ptr>::operator->() const{ return cur; }

template <typename T, typename Ref, typename Ptr>
typename deque_iterator<T, Ref, Ptr>::difference_type
deque_iterator<T, Ref, Ptr>::operator-(const self& rhs) const{
    return static_cast<difference_type>(buffer_size) * (node - rhs.node - 1) + (cur - first) + (rhs.last - rhs.cur);
}


template <typename T, typename Ref, typename Ptr>
typename deque_iterator<T, Ref, Ptr>::self&
deque_iterator<T, Ref, Ptr>::operator++(){
    ++cur;
    if(cur == last){
        set_node(node + 1);
        cur = first;
    }
    return *this;
}

template <typename T, typename Ref, typename Ptr>
typename deque_iterator<T, Ref, Ptr>::self&
deque_iterator<T, Ref, Ptr>::operator++(int){
    self tmp = *this;
    ++*this;
    return tmp;
}

template <typename T, typename Ref, typename Ptr>
typename deque_iterator<T, Ref, Ptr>::self&
deque_iterator<T, Ref, Ptr>::operator--(){
    if(cur == first){
        set_node(node - 1);
        cur = last;
    }
    --cur;
    return *this;
    
}


template <typename T, typename Ref, typename Ptr>
typename deque_iterator<T, Ref, Ptr>::self&
deque_iterator<T, Ref, Ptr>::operator--(int){
    self tmp = *this;
    --*this;
    return tmp;
}

template <typename T, typename Ref, typename Ptr>
typename deque_iterator<T, Ref, Ptr>::self&
deque_iterator<T, Ref, Ptr>::operator+=(difference_type n){
    difference_type offset = n + (cur - first);
    if(offset >= 0 && offset < difference_type(buffer_size)){
        cur += n;
    }else{
        difference_type node_offset = offset > 0 ? offset / difference_type(buffer_size) : -difference_type((-offset - 1) / buffer_size) - 1;
        set_node(node + node_offset);
        cur = first + (offset - node_offset * difference_type(buffer_size));
    }
    return *this;
}

template <typename T, typename Ref, typename Ptr>
typename deque_iterator<T, Ref, Ptr>::self&
deque_iterator<T, Ref, Ptr>::operator-=(difference_type n){
    return *this += -n;
}

template <typename T, typename Ref, typename Ptr>
typename deque_iterator<T, Ref, Ptr>::self
deque_iterator<T, Ref, Ptr>::operator+(difference_type n) const{
    self tmp = *this;
    return tmp += n;
}

template <typename T, typename Ref, typename Ptr>
typename deque_iterator<T, Ref, Ptr>::self
deque_iterator<T, Ref, Ptr>::operator-(difference_type n) const{
    self tmp = *this;
    return tmp -= n;
}


template <typename T, typename Ref, typename Ptr>
typename deque_iterator<T, Ref, Ptr>::reference
deque_iterator<T, Ref, Ptr>::operator[](difference_type n) const{
    return *(*this + n);
}


template <typename T, typename Ref, typename Ptr>
void deque_iterator<T, Ref, Ptr>::set_node(map_pointer new_node){
    node = new_node;
    first = *node;
    last = first + difference_type(buffer_size);
}



template <typename T>
class deque{
public:
    typedef hstl::allocator<T>                          allocator_type;
    typedef hstl::allocator<T>                          data_allocator;
    typedef hstl::allocator<T*>                         map_allocator;

    typedef typename allocator_type::value_type         value_type;
    typedef typename allocator_type::pointer            pointer;
    typedef typename allocator_type::const_pointer      const_pointer;
    typedef typename allocator_type::reference          reference;
    typedef typename allocator_type::const_reference    const_reference;
    typedef typename allocator_type::size_type          size_type;
    typedef typename allocator_type::difference_type    difference_type;
    typedef pointer*                                    map_pointer;
    typedef const_pointer*                              const_map_pointer;

    typedef deque_iterator<T, T&, T*>                   iterator;
    typedef deque_iterator<T, const T&, const T*>       const_iterator;
    
    static const size_type buffer_size = deque_buffer_size<T>::buffer_size;
private:
    iterator        begin_;
    iterator        end_;
    map_pointer     map_;
    size_type       map_size_;

public:
    deque();
    explicit deque(size_type n);
    deque(size_type n, const value_type& value);
    
    template <typename Iterator, typename = typename std::enable_if<
        std::is_convertible<typename std::iterator_traits<Iterator>::iterator_category, std::input_iterator_tag>::value>::type>
    deque(Iterator first, Iterator last);

public:
    iterator begin() noexcept;
    iterator end() noexcept;
    const_iterator begin() const noexcept;
    const_iterator end() const noexcept;
    size_type size() const noexcept;
    bool empty() const noexcept;
    reference front();
    reference back();
    
    template <typename... Args>
    void emplace_front(Args&&... args); 

    template <typename... Args>
    void emplace_back(Args&&... args);
     
    void push_front(const value_type& value);
    void push_back(const value_type& value);
    void push_front(value_type&& value);
    void push_back(value_type&& value);
    
    void pop_front();
    void pop_back();
    void clear() noexcept;
    
private:
    void map_init(size_type n_elements);
    map_pointer allocate_map(size_type map_size);
    void allocate_node(map_pointer nstart, map_pointer nfinish);
    void fill_init(size_type n, const value_type& value);

    template <typename Iterator>
    void copy_init(Iterator first, Iterator last, std::input_iterator_tag);

    template <typename Iterator>
    void copy_init(Iterator first, Iterator last, std::forward_iterator_tag);

    void require_capacity(size_type n_elements, bool is_front);
    
    void reallocate_map(size_type need_nodes, bool is_front);

    void destroy_nodes(map_pointer nstart, map_pointer nfinish);
};


template <typename T>
deque<T>::deque(){
    map_init(0);
}

template <typename T>
deque<T>::deque(size_type n){
    map_init(n);
    fill_init(n, value_type());
}

template <typename T>
deque<T>::deque(size_type n, const value_type& value){
    map_init(n);
    fill_init(n, value);
}

template <typename T>
template <typename Iterator, typename>
deque<T>::deque(Iterator first, Iterator last){
    typedef typename std::iterator_traits<Iterator>::iterator_category category;
    copy_init(first, last, category());
}

template <typename T>
typename deque<T>::iterator deque<T>::begin() noexcept{
    return begin_;
}

template <typename T>
typename deque<T>::iterator deque<T>::end() noexcept{
    return end_;
}

template <typename T>
typename deque<T>::const_iterator deque<T>::begin() const noexcept{
    return begin_;
}

template <typename T>
typename deque<T>::const_iterator deque<T>::end() const noexcept{
   return end_;
}

template <typename T>
typename deque<T>::size_type deque<T>::size() const noexcept{
    return end_ - begin_;
}

template <typename T>
bool deque<T>::empty() const noexcept{
    return begin_ == end_;
}

template <typename T>
typename deque<T>::reference deque<T>::front(){
    return *begin_;
}

template <typename T>
typename deque<T>::reference deque<T>::back(){
    return *(end_ - 1);
}

template <typename T>
template <typename ...Args>
void deque<T>::emplace_front(Args&&... args){
    if(begin_.cur != begin_.first){
        data_allocator::construct(begin_.cur - 1, std::forward<Args>(args)...);
        --begin_.cur;
    }else{
        require_capacity(1, true);
        --begin_;
        data_allocator::construct(begin_.cur, std::forward<Args>(args)...);
    }

}

template <typename T>
template <typename ...Args>
void deque<T>::emplace_back(Args&&... args){
    // actually, there are two positions to use in the end, to prevent unauthorized access
    if(end_.cur != end_.last - 1){
        data_allocator::construct(end_.cur, std::forward<Args>(args)...);
        ++end_.cur;
    }else{
        require_capacity(1, false);
        data_allocator::construct(end_.cur, std::forward<Args>(args)...);
        ++end_;
    }
}

template <typename T>
void deque<T>::push_front(const value_type& value){
    if(begin_.cur != begin_.first){
        data_allocator::construct(begin_.cur - 1, value);
        --begin_.cur;
    }else{
        require_capacity(1, true);
        --begin_;
        data_allocator::construct(begin_.cur, value);
    }
}

template <typename T>
void deque<T>::push_back(const value_type& value){
    if(end_.cur != end_.last - 1){
        data_allocator::construct(end_.cur, value);
        ++end_.cur;
    }else{
        require_capacity(1, false);
        data_allocator::construct(end_.cur, value);
        ++end_;
    }
}

template <typename T>
void deque<T>::push_front(value_type&& value){
    emplace_front(std::move(value));
}

template <typename T>
void deque<T>::push_back(value_type&& value){
    emplace_back(std::move(value));
}

template <typename T>
void deque<T>::pop_front(){
    assert(!empty());
    if(begin_.cur != begin_.last - 1){
        data_allocator::destroy(begin_.cur);
        ++begin_.cur;
    }else{
        data_allocator::destroy(begin_.cur);
        ++begin_;
        destroy_nodes(begin_.node - 1, begin_.node - 1);
        
    }
}

template <typename T>
void deque<T>::pop_back(){
    assert(!empty());
    if(end_.cur != end_.first){
        --end_.cur;
        data_allocator::destroy(end_.cur);
    }else{
        --end_;
        data_allocator::destroy(end_.cur);
        destroy_nodes(end_.node + 1, end_.node + 1);
    }
}

template <typename T>
void deque<T>::clear() noexcept{
    for(auto cur = begin_.node + 1; cur < end_.node; ++cur){
        data_allocator::destroy(*cur, *cur + buffer_size);
    }
    if(begin_.node != end_.node){
        data_allocator::destroy(begin_.cur, begin_.last);
        data_allocator::destroy(end_.first, end_.cur);
    }else{
        data_allocator::destroy(begin_.cur, end_.cur);
    }
    destroy_nodes(begin_.node + 1, end_.node);
    end_ = begin_;
}


template <typename T>
void deque<T>::map_init(size_type n_elements){
    size_type num_nodes = n_elements / buffer_size + 1; // need nodes
    map_size_ = std::max(num_nodes + 2, size_type(DEQUE_MAP_SIZE));
    try{
        map_ = allocate_map(map_size_);
    }catch(...){
        map_ = nullptr;
        map_size_ = 0;
        throw;
    }
    // point to the middle of map
    map_pointer nstart = map_ + (map_size_ - num_nodes) / 2;
    map_pointer nfinish = nstart + num_nodes - 1;
    try{
        allocate_node(nstart, nfinish);
    }catch(...){    
        map_allocator::deallocate(map_, map_size_);
        map_ = nullptr;
        map_size_ = 0;
        throw;
    }
    begin_.set_node(nstart);
    end_.set_node(nfinish);
    begin_.cur = begin_.first;
    end_.cur = end_.first + n_elements % buffer_size;
    
}

template <typename T>
typename deque<T>::map_pointer deque<T>::allocate_map(size_type map_size){
    return map_allocator::allocate(map_size);
}

template <typename T>
void deque<T>::allocate_node(map_pointer nstart, map_pointer nfinish){
    map_pointer cur = nstart;
    try{
        for(; cur <= nfinish; ++cur){
            *cur = data_allocator::allocate(buffer_size);
        }
    }catch(...){
        for(--cur; cur >= nstart; --cur){
            data_allocator::deallocate(*cur, buffer_size);
        }
        throw;
    }
}

template <typename T>
void deque<T>::fill_init(size_type n, const value_type& value){
    for(map_pointer cur = begin_.node; cur < end_.node; ++cur){
        hstl::uninitialized_fill_n(*cur, buffer_size, value);
    }
    hstl::uninitialized_fill_n(end_.first, end_.cur - end_.first, value);
    
}


template <typename T>
template <typename Iterator>
void deque<T>::copy_init(Iterator first, Iterator last, std::input_iterator_tag){
    const size_type n = std::distance(first, last);
    map_init(n);
    for(;first != last; ++first){
        emplace_back(*first);
    }
}

template <typename T>
template <typename Iterator>
void deque<T>::copy_init(Iterator first, Iterator last, std::forward_iterator_tag){
    const size_type n = std::distance(first, last);
    map_init(n);
    for(auto cur = begin_.node; cur < end_.node; ++cur){
        auto next = first;
        std::advance(next, buffer_size);
        hstl::uninitialized_copy(first, next, *cur);
        first = next;
    }
    hstl::uninitialized_copy(first, last, end_.first);
}


template <typename T>
void deque<T>::require_capacity(size_type n_elements, bool is_front){
    if(is_front && (static_cast<size_type> (begin_.cur - begin_.first) < n_elements )){
        const size_type need_nodes = (n_elements - (begin_.cur - begin_.first)) / buffer_size + 1;
        if(need_nodes > static_cast<size_type> (begin_.node - map_)){
            reallocate_map(need_nodes, is_front);
        }else{
            allocate_node(begin_.node - need_nodes, begin_.node - 1);
        }
        
    }else if (!is_front && (static_cast<size_type> (end_.last - end_.cur - 1) < n_elements )){
        const size_type need_nodes = (n_elements - (end_.last - end_.cur - 1)) / buffer_size + 1;
        if(need_nodes > static_cast<size_type> ((map_ + map_size_) - end_.node - 1)){
            reallocate_map(need_nodes, is_front);
        }else{
            allocate_node(end_.node + 1, end_.node + need_nodes);
        }
    }
}

template <typename T>
void deque<T>::reallocate_map(size_type need_nodes, bool is_front){
    const size_type new_map_size  = std::max(map_size_ << 1, map_size_ + need_nodes + DEQUE_MAP_SIZE);
    map_pointer new_map = allocate_map(new_map_size);
    const size_type old_nodes = end_.node - begin_.node + 1;
    const size_type new_nodes = old_nodes + need_nodes;
    
    if(is_front){
        auto begin = new_map + (new_map_size - new_nodes) / 2;
        auto mid = begin + need_nodes;
        auto end = mid + old_nodes;
        allocate_node(begin, mid-1);
        for(auto cur = mid, b = begin_.node; cur != end; ++cur, ++b){
            *cur = *b;
        }
        map_allocator::deallocate(map_, map_size_);
        map_ = new_map;
        map_size_ = new_map_size;
        begin_ = iterator(*mid + (begin_.cur - begin_.first), mid);
        end_ = iterator(*(end - 1) + (end_.cur - end_.first), end - 1);
        
    }else{
        auto begin = new_map + (new_map_size - new_nodes) / 2;
        auto mid = begin + old_nodes;
        auto end = mid + need_nodes;
        for(auto cur = begin, b = begin_.node; cur != mid; ++cur, ++b){
            *cur = *b;
        }
        allocate_node(mid, end-1);
        map_allocator::deallocate(map_, map_size_);
        map_ = new_map;
        map_size_ = new_map_size;
        begin_ = iterator(*begin + (begin_.cur - begin_.first), begin);
        end_ = iterator(*(mid - 1) + (end_.cur - end_.first), mid - 1);
    }
}

template <typename T>
void deque<T>::destroy_nodes(map_pointer first, map_pointer last){
    for(auto cur = first; cur <= last; ++cur){
        data_allocator::deallocate(*cur, buffer_size);
    }
}

} // namespace hstl

#endif