#ifndef TINYSTL_RB_TREE_H
#define TINYSTL_RB_TREE_H

#include <iterator>
#include <type_traits>
#include <cassert>
#include "allocator.h"
#include "construct.h"


namespace hstl{

typedef bool rb_tree_color_type;
static constexpr rb_tree_color_type rb_tree_red = false;
static constexpr rb_tree_color_type rb_tree_black = true;


struct rb_tree_node_base{
    typedef rb_tree_color_type color_type;
    typedef rb_tree_node_base* base_ptr;


    color_type color;
    base_ptr parent;
    base_ptr left;
    base_ptr right;


    base_ptr get_base_ptr(){
        return &*this;
    }


    static base_ptr minimum(base_ptr x){
        while (x->left != nullptr){
            x = x->left;
        }
        return x;
    }

    static base_ptr maximum(base_ptr x){
        while (x->right != nullptr){
            x = x->right;
        }
        return x;
    }
};


template <typename T>
struct rb_tree_node : public rb_tree_node_base{
    typedef rb_tree_node<T>* link_type;
    T data;
};


struct rb_tree_iterator_base{
    typedef rb_tree_node_base::base_ptr                             base_ptr;
    typedef std::bidirectional_iterator_tag                         iterator_category;
    typedef ptrdiff_t                                               difference_type;
    
    base_ptr node;

    void increment(){
        if (node->right != nullptr){
            node = node->right;
            while (node->left != nullptr){
                node = node->left;
            }
        }
        else{
            base_ptr y = node->parent;
            while (node == y->right){
                node = y;
                y = y->parent;
            }
            if(node->right != y){
                node = y;
            }
        }
    }

    void decrement(){
        if(node->color == rb_tree_red && node->parent->parent == node){
            node = node->right;
        }else if(node->left != nullptr){
            base_ptr y = node->left;
            while (y->right != nullptr){
                y = y->right;
            }
            node = y;
        }else{
            base_ptr y = node->parent;
            while (node == y->left){
                node = y;
                y = y->parent;
            }
            node = y;
        }
    }
};

template <typename Value, typename Ref, typename Ptr>
struct rb_tree_iterator : public rb_tree_iterator_base{
    typedef Value                                                   value_type;
    typedef Ref                                                     reference;
    typedef Ptr                                                     pointer;
    typedef rb_tree_iterator<Value, Value&, Value*>                 iterator;
    typedef rb_tree_iterator<Value, const Value&, const Value*>     const_iterator;
    typedef rb_tree_iterator<Value, Ref, Ptr>                       self;
    typedef rb_tree_node<Value>*                                    link_type;


    rb_tree_iterator(){

    }
    rb_tree_iterator(link_type x){
        node = x;
    }
    rb_tree_iterator(const iterator& it){
        node = it.node;
    }

    reference operator*() const{
        return link_type(node)->data;
    }

    pointer operator->() const{
        return &(operator*());
    }

    self& operator++(){
        increment();
        return *this;
    }

    self operator++(int){
        self tmp = *this;
        increment();
        return tmp;
    }

    self& operator--(){
        decrement();
        return *this;
    }

    self operator--(int){
        self tmp = *this;
        decrement();
        return tmp;
    }
};

template <typename T, typename Compare>
class rb_tree{
public:
    typedef hstl::allocator<T>                          allocator_type;
    typedef hstl::allocator<T>                          data_allocator;
    typedef hstl::allocator<rb_tree_node<T>>            node_allocator;
    typedef rb_tree_node<T>::link_type                  link_type;
    typedef rb_tree_node_base::base_ptr                 base_ptr;
    typedef Compare                                     key_compare;

    typedef typename allocator_type::value_type         value_type;
    typedef typename allocator_type::pointer            pointer;
    typedef typename allocator_type::const_pointer      const_pointer;
    typedef typename allocator_type::reference          reference;
    typedef typename allocator_type::const_reference    const_reference;
    typedef typename allocator_type::size_type          size_type;

    typedef rb_tree_iterator<T, T&, T*>                 iterator;

private:
    link_type header_;
    size_type size_;
    key_compare key_compare_;

public:
    key_compare key_comp() const{ return key_compare_; }
    iterator begin() const{ return leftmost(); }
    iterator end() const{ return header_; }
    size_type size() const{ return size_; }
    bool empty() const{ return size_ == 0; }


private:
    link_type& root() const{ return header_->parent; }
    link_type& leftmost() const{ return header_->left; }
    link_type& rightmost() const{ return header_->right; }

public:
    rb_tree();
    iterator insert_equal(const value_type& value); // insert value into rb_tree (allowing duplicate values)
private:
    link_type get_node();
    link_type create_node(const value_type& value);
    iterator insert_node(base_ptr x_, base_ptr y_, const value_type& value);
    void rb_tree_insert_rebalance(base_ptr x, base_ptr& root);
};

template <typename T, typename Compare>
rb_tree<T, Compare>::rb_tree(){
    header_ = get_node();
    header_->color = rb_tree_red;
    header_->parent = nullptr;
    header_->left = header_;
    header_->right = header_;
    size_ = 0;
}

template <typename T, typename Compare>
typename rb_tree<T, Compare>::iterator rb_tree<T, Compare>::insert_equal(const value_type& value){
    link_type y = header_;
    link_type x = root();
    while (x != nullptr){
        y = x;
        x = key_compare_(value, x->data) ? x->left : x->right;
    }
    return insert_node(x, y, value);

}

template <typename T, typename Compare>
typename rb_tree<T, Compare>::link_type rb_tree<T, Compare>::get_node(){
    return node_allocator::allocate();
}


template <typename T, typename Compare>
typename rb_tree<T, Compare>::link_type rb_tree<T, Compare>::create_node(const value_type& value){
    link_type p = get_node();
    try{
        hstl::construct(&(p->data), value);
        p->parent = p->left = p->right = nullptr;
    }catch(...){
        node_allocator::deallocate(p);
        throw;
    }
    return p;
}

template <typename T, typename Compare>
typename rb_tree<T, Compare>::iterator rb_tree<T, Compare>::insert_node(base_ptr x_, base_ptr y_, const value_type& value){
    link_type x = static_cast<link_type>(x_);
    link_type y = static_cast<link_type>(y_);
    link_type z;
    if(y == header_ || x != nullptr || key_compare_(value, y->data)){
        z = create_node(value);
        y->left = z;
        if(y == header_){
            root() = z;
            rightmost() = z;
        }
        if(y == leftmost()){
            leftmost() = z;
        }
    }
    else{
        z = create_node(value);
        y->right = z;
        if(y == rightmost()){
            rightmost() = z;
        }
    }
    z->parent = y;
    z->left = z->right = nullptr;
    rb_tree_insert_rebalance(z, header_->parent);
    ++size_;
    return iterator(z);
}

template <typename T, typename Compare>
void rb_tree<T, Compare>::rb_tree_insert_rebalance(base_ptr x, base_ptr& root){
    x->color = rb_tree_red;
    // TODO
}



 
} // namespace hstl
#endif