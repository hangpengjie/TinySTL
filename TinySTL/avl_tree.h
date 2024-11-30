#ifndef TINYSTL_TEST_AVL_TREE_H
#define TINYSTL_TEST_AVL_TREE_H

#include <iterator>
#include <type_traits>
#include <cassert>
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

    avl_iterator(node_pointer x,node_pointer header):node(x), header(header) {}
    avl_iterator(const iterator& it):node(it.node),header(it.header) {}

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
    void remove_value(const T& value);
    iterator begin();
    iterator end();
    size_type size();
    bool is_balanced();


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
    void rotate_left_right(node_ptr x);
    void rotate_right_left(node_ptr x);
    std::pair<bool, int> is_balanced(node_ptr x);
    node_ptr remove_node(node_ptr& p,const T& value);

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
        p->parent = header_;
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
void avl_tree<T>::remove_value(const T& value){
    node_ptr p = remove_node(root(),value);
    --size_;
    if(p == header_) return;
    if(p->right){
        p->bf += 1;
    }else if(p->left){
        p->bf -= 1;
    }else{
        p->bf = 0;
    }
    node_ptr y = p->parent;
    while(p != header_){
        bool is_left = y->left == p;
        if(p->bf == 1 || p->bf == -1){
            break;
        }else if (p->bf == -2){
            node_ptr x = p->left;
            if(x->bf == 1){
                rotate_left_right(p);
            }else{
                rotate_right(p);
            }
            
        } else if (p->bf == 2){
            node_ptr x = p->right;
            if(x->bf == -1){
                rotate_right_left(p);
            }else{
                rotate_left(p);
            }
        
        }
        p = is_left ? y->left: y->right;
        if(p->bf == 0){
            if(y->left == p){
                y->bf += 1;
            }else{
                y->bf -= 1;
            }
            p = y;
            y = y->parent;
        }
    
    }

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
typename avl_tree<T>::size_type avl_tree<T>::size(){
    return size_;
}

template <typename T>
bool avl_tree<T>::is_balanced(){
    return is_balanced(root()).first;
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
    while(x != root()){
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
                rotate_right(y);
            }else{
                rotate_left_right(y);
            }
            break;
        }else if (y->bf == 2){
            if(x->bf == 1){
                rotate_left(y);
            }else{
                rotate_right_left(y);
            }
            break;
        }
        
    }
}

template <typename T>
void avl_tree<T>::rotate_left(node_ptr x){
    node_ptr parent = x->parent;
    node_ptr sub_r = x->right;
    node_ptr sub_rl = sub_r->left;
    x->right = sub_rl;
    if(sub_rl != nullptr){
        sub_rl->parent = x;
    }
    sub_r->left = x;
    x->parent = sub_r;
    if(parent == header_){
        root() = sub_r;
        sub_r->parent = header_;
    }else{
        if(x == parent->left){
            parent->left = sub_r;
        }else{
            parent->right = sub_r;
        }
        sub_r->parent = parent;
    }
    if(sub_r->bf == 0){
        x->bf = 1;
        sub_r->bf = -1;
    }else{
        x->bf = sub_r->bf = 0;
    }
    
    
}

template <typename T>
void avl_tree<T>::rotate_right(node_ptr x){
    node_ptr parent = x->parent;
    node_ptr sub_l = x->left;
    node_ptr sub_lr = sub_l->right;
    x->left = sub_lr;
    if(sub_lr != nullptr){
        sub_lr->parent = x;
    }
    sub_l->right = x;
    x->parent = sub_l;
    if(parent == header_){
        root() = sub_l;
        sub_l->parent = header_;
    }else{
        if(x == parent->left){
            parent->left = sub_l;
        }else{
            parent->right = sub_l;
        }
        sub_l->parent = parent;
    }
    if(sub_l->bf == 0){
        x->bf = -1;
        sub_l->bf = 1;
    }else{
        x->bf = sub_l->bf = 0;
    }
    

}

template <typename T>
void avl_tree<T>::rotate_left_right(node_ptr x){
    node_ptr sub_l = x->left;
    node_ptr sub_lr = sub_l->right;
    int sub_lr_bf = sub_lr->bf;
    rotate_left(sub_l);
    rotate_right(x);
    if(sub_lr_bf == 0){
        x->bf = sub_l->bf = sub_lr->bf = 0;
    }else if (sub_lr_bf == -1){
        sub_l->bf = sub_lr->bf = 0;
        x->bf = 1;
    }else if (sub_lr_bf == 1){
        sub_l->bf = -1;
        x->bf = sub_lr->bf = 0;
    }else{
        assert(false);
    }
    
    
}

template <typename T>
void avl_tree<T>::rotate_right_left(node_ptr x){
    node_ptr sub_r = x->right;
    node_ptr sub_rl = sub_r->left;
    int sub_rl_bf = sub_rl->bf;
    rotate_right(sub_r);
    rotate_left(x);
    if(sub_rl_bf == 0){
        x->bf = sub_r->bf = sub_rl->bf = 0;
    }else if (sub_rl_bf == 1){
        sub_r->bf= sub_rl->bf = 0;
        x->bf = -1;
    }else if (sub_rl_bf == -1){
        sub_r->bf = 1;
        x->bf = sub_rl->bf = 0;
    }else{
        assert(false);
    }
}

template <typename T>
std::pair<bool, int> avl_tree<T>::is_balanced(node_ptr x){
    if(x == nullptr){
        return std::make_pair(true, 0);
    }
    auto l = is_balanced(x->left);
    auto r = is_balanced(x->right);
    bool flag = l.first && r.first && (r.second - l.second == x->bf) && abs(x->bf) < 2;
   
    return std::make_pair(flag, std::max(l.second, r.second) + 1);
}


template <typename T>
typename avl_tree<T>::node_ptr avl_tree<T>::remove_node(node_ptr &p, const T& value){
    if(p == nullptr)    return header_;
    if(p->data < value){
        return remove_node(p->right, value);
    }else if (p->data > value){
        return remove_node(p->left, value);
    }else{
        node_ptr ret = p;
        if(p->left == nullptr || p->right == nullptr){
            if(p == leftmost()){
                leftmost() = (++iterator(p, header_)).node;
            }
            if(p == rightmost()){
                rightmost() = (--iterator(p, header_)).node;
            }
            node_ptr temp = p->left != nullptr ? p->left : p->right;
            if(temp == nullptr){
                ret = p->parent;
                temp = p;
                p = nullptr;
            }else{
                p->data = temp->data;
                p->left = p->right = nullptr;
            }
            destroy_node(temp);
            return ret;
        }else{
            iterator it = iterator(p, header_);
            ++it;
            p->data = it.node->data;
            return remove_node(p->right, it.node->data);
        }
    }
    
}

} // namespace hstl
#endif