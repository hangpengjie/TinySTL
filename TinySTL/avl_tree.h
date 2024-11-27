#ifndef TINYSTL_TEST_AVL_TREE_H
#define TINYSTL_TEST_AVL_TREE_H

#include <iterator>
#include <type_traits>

namespace hstl{

template <typename T>
class avl_node{
public:
    typedef avl_node<T>* pointer;
    pointer parent;
    pointer left;
    pointer right;
    T data;
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

    avl_iterator() : node(nullptr){}
    avl_iterator(node_pointer x,node_pointer header) node(x), header(header) {}
    avl_iterator(const iterator& it) node(it.node){}

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


};





}
#endif