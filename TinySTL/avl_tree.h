#ifndef TINYSTL_TEST_AVL_TREE_H
#define TINYSTL_TEST_AVL_TREE_H

#include <iterator>
#include <type_traits>
#include "allocator.h"
#include "construct.h"

namespace hstl{

template <typename T>
class avl_node{
public:
    typedef avl_node<T>* pointer;
    pointer parent;
    pointer left;
    pointer right;
    T data;
    int32_t bf; // balance factor
    avl_node() = default;
    avl_node(const T& value) : data(value){}

    avl_node(const T&& value) : data(std::move(value)){}


    static pointer minimum(pointer x){
        while (x->left != nullptr){
            x = x->left;
        }
        return x;
    }

    static pointer maximum(pointer x){
        while (x->right != nullptr){
            x = x->right;
        }
        return x;
    }
};


template <typename Value, typename Ref, typename Ptr>
class avl_iterator{
public:
    typedef avl_node<Value>*                                        node_pointer;
    typedef std::bidirectional_iterator_tag                         iterator_category;
    typedef ptrdiff_t                                               difference_type;
    typedef Value                                                   value_type;
    typedef Ref                                                     reference;
    typedef Ptr                                                     pointer;
    typedef avl_iterator<Value,Ref, Ptr>                            self;
    typedef avl_iterator<Value, Value&, Value*>                     iterator;
    typedef avl_iterator<Value, const Value&, const Value*>         const_iterator;   

    node_pointer node;
    node_pointer header;

    avl_iterator(node_pointer x,node_pointer header) node(x), header(header) {}
    avl_iterator(const iterator& it) node(it.node),header(it.header) {}

    reference operator*() const{
        return node->data;
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

    bool operator==(const self& rhs) const{
        return node == rhs.node;
    }

    bool operator!=(const self& rhs) const{
        return node != rhs.node;
    }

private:
    
    void increment(){
        if (node->right != nullptr){
            node = node->right;
            while (node->left != nullptr){
                node = node->left;
            }
        }else{
            node_pointer y = node->parent;
            while (node == y->right){
                node = y;
                y = y->parent;
            }
            if(node != header){
                node = y;
            }
        }
    }

    void decrement(){
        if(node == header){
            node = node->right;
        }else if(node->left != nullptr){
            node = node->left;
            while (node->right != nullptr){
                node = node->right;
            }
        }else{
            node_pointer y = node->parent;
            while (node == y->left){
                node = y;
                y = y->parent;
            }
            node = y;
        }
    }
};

template <typename T>
class avl_tree{
public:
    typedef hstl::allocator<T>                          allocator_type;
    typedef hstl::allocator<T>                          data_allocator;
    typedef hstl::allocator<avl_node<T>>                node_allocator;


    typedef typename allocator_type::value_type         value_type;
    typedef typename allocator_type::pointer            pointer;
    typedef typename allocator_type::const_pointer      const_pointer;
    typedef typename allocator_type::reference          reference;
    typedef typename allocator_type::const_reference    const_reference;
    typedef typename allocator_type::size_type          size_type;
    typedef typename allocator_type::difference_type    difference_type;

    typedef avl_iterator<T, T&, T*>                     iterator;
    typedef avl_iterator<T, const T&, const T*>         const_iterator;
    typedef typename iterator::node_pointer             node_ptr;

private:
    node_ptr header_;   // header node
    size_type size_;    // node number


public:
    avl_tree();
    iterator insert_equal(const T& value);
    iterator begin();
    iterator end();


private:
    node_ptr get_node();
    node_ptr create_node(const T& value);
    void destroy_node(node_ptr p);
    node_ptr& root() const;
    node_ptr& leftmost() const;
    node_ptr& rightmost() const;
    void balance_avl(node_ptr x);
    void rotate_left(node_ptr x);
    void rotate_right(node_ptr x);

};

template <typename T>
avl_tree<T>::avl_tree(){
    size_ = 0;
    header_ = get_node();
    root() = nullptr;
    leftmost() = header_;
    rightmost() = header_;
}

template <typename T>
typename avl_tree<T>::iterator avl_tree<T>::insert_equal(const T& value){
    node_ptr p = create_node(value);
    node_ptr cur = root();
    if(cur == nullptr){
        root() = p;
        leftmost() = p;
        rightmost() = p;
    }else{
        while(true){
            if(value < cur->data){
                if(cur->left == nullptr){
                    cur->left = p;
                    p->parent = cur;
                    if(cur == leftmost()){
                        leftmost() = p;
                    }
                    break;
                }else{
                    cur = cur->left;
                }
            }else{
                if(cur->right == nullptr){
                    cur->right = p;
                    p->parent = cur;
                    if(cur == rightmost()){
                        rightmost() = p;
                    }
                    break;
                }else{
                    cur = cur->right;
                }
            }
        }
        balance_avl(p);
    }
    ++size_;
    return iterator(p, header_);
}

template <typename T>
typename avl_tree<T>::iterator avl_tree<T>::begin(){
    return iterator(leftmost(), header_);
}

template <typename T>
typename avl_tree<T>::iterator avl_tree<T>::end(){
    return iterator(header_, header_);
}

template <typename T>
typename avl_tree<T>::node_ptr avl_tree<T>::get_node(){
    return node_allocator::allocate();
}

template <typename T>
typename avl_tree<T>::node_ptr avl_tree<T>::create_node(const T& value){
    node_ptr p = get_node();
    try{
        hstl::construct(&(p->data), value);
        p->parent = p->left = p->right = nullptr;
        p->bf = 0;     
    }catch(...){
        node_allocator::deallocate(p);
        throw;
    }
    return p;
}

template <typename T>
void avl_tree<T>::destroy_node(node_ptr p){
    hstl::destroy(&(p->data));
    node_allocator::deallocate(p);
    return;
}

template <typename T>
typename avl_tree<T>::node_ptr& avl_tree<T>::root() const{
    return (node_ptr&)header_->parent;
}

template <typename T>
typename avl_tree<T>::node_ptr& avl_tree<T>::leftmost() const{
    return (node_ptr&)header_->left;
}

template <typename T>
typename avl_tree<T>::node_ptr& avl_tree<T>::rightmost() const{
    return (node_ptr&)header_->right;
}

template <typename T>
void avl_tree<T>::balance_avl(node_ptr x){
    while(x != header_){
        node_ptr y = x->parent;
        if(x == y->left){
            --y->bf;
        }else{
            ++y->bf;
        }
        if(y->bf == 0){
            break;
        }else if(y->bf == -1 || y->bf == 1){
            x = y;
        }else if(y->bf == -2){
            if(x->bf == -1){
                rotate_left(y);
            }else{
                rotate_right(x);
                rotate_left(y);
            }
            break;
        }else if (y->bf == 2){
            if(x->bf == 1){
                rotate_right(y);
            }else{
                rotate_left(x);
                rotate_right(y);
            }
            break;
        }
        
    }
}

template <typename T>
void avl_tree<T>::rotate_left(node_ptr x){
}

template <typename T>
void avl_tree<T>::rotate_right(node_ptr x){
}

} // namespace hstl
#endif