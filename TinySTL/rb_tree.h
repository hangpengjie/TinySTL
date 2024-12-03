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

inline void rb_tree_set_red(rb_tree_node_base* x){
    x->color = rb_tree_red;
}
inline void rb_tree_set_black(rb_tree_node_base* x){
    x->color = rb_tree_black;
}

inline bool rb_tree_is_red(const rb_tree_node_base* x){
    return x->color == rb_tree_red;
}
inline bool rb_tree_is_black(const rb_tree_node_base* x){
    return x->color == rb_tree_black;
}
inline bool rb_tree_is_left_child(const rb_tree_node_base* x){
    return x->parent->left == x;
}
inline bool rb_tree_is_right_child(const rb_tree_node_base* x){
    return x->parent->right == x;
}

inline rb_tree_node_base* rb_tree_get_next(rb_tree_node_base* x){
    if(x->right != nullptr){
        return rb_tree_node_base::minimum(x->right);
    }
    rb_tree_node_base* y = x->parent;
    while(x == y->right){
        x = y;
        y = y->parent;
    }
    if(x->right != y){
        return y;
    }else{
        return x;
    }
}

inline void rb_tree_rotate_left(rb_tree_node_base* x, rb_tree_node_base*& root){
    rb_tree_node_base *y = x->right;
    x->right = y->left;
    if(y->left != nullptr){
        y->left->parent = x;
    }
    y->parent = x->parent;
    if(x == root){
        root = y;
    }else if (rb_tree_is_left_child(x)){
        x->parent->left = y;
    }else{
        x->parent->right = y;
    }
    y->left = x;
    x->parent = y;
    
}

inline void rb_tree_rotate_right(rb_tree_node_base* x, rb_tree_node_base*& root){
    rb_tree_node_base *y = x->left;
    x->left = y->right;
    if(y->right != nullptr){
        y->right->parent = x;
    }
    y->parent = x->parent;
    if(x == root){
        root = y;
    }else if (rb_tree_is_left_child(x)){
        x->parent->left = y;
    }else{
        x->parent->right = y;
    }
    y->right = x;
    x->parent = y;
}

// 插入节点后使 rb tree 重新平衡，参数一为新增节点，参数二为根节点
//
// case 1: 新增节点位于根节点，令新增节点为黑
// case 2: 新增节点的父节点为黑，没有破坏平衡，直接返回
// case 3: 父节点和叔叔节点都为红，令父节点和叔叔节点为黑，祖父节点为红，
//         然后令祖父节点为当前节点，继续处理
// case 4: 父节点为红，叔叔节点为 NIL 或黑色，父节点为左（右）孩子，当前节点为右（左）孩子，
//         让父节点成为当前节点，再以当前节点为支点左（右）旋
// case 5: 父节点为红，叔叔节点为 NIL 或黑色，父节点为左（右）孩子，当前节点为左（右）孩子，
//         让父节点变为黑色，祖父节点变为红色，以祖父节点为支点右（左）旋

inline void rb_tree_insert_rebalance(rb_tree_node_base* x, rb_tree_node_base*& root){
    rb_tree_set_red(x);
    while(x != root && rb_tree_is_red(x->parent)){
        if(rb_tree_is_left_child(x->parent)){
            rb_tree_node_base* uncle = x->parent->parent->right;
            if(uncle != nullptr && rb_tree_is_red(uncle)){ // case 3
                rb_tree_set_black(x->parent);
                rb_tree_set_black(uncle);
                rb_tree_set_red(x->parent->parent);
                x = x->parent->parent;
            }else{
                if(rb_tree_is_right_child(x)){  // case 4
                    x = x->parent;
                    rb_tree_rotate_left(x, root);
                }
                // case 5
                rb_tree_set_black(x->parent); 
                rb_tree_set_red(x->parent->parent);
                rb_tree_rotate_right(x->parent->parent, root);
                break;
            }
        }else{
            rb_tree_node_base* uncle = x->parent->parent->left;
            if(uncle != nullptr && rb_tree_is_red(uncle)){ // case 3
                rb_tree_set_black(x->parent);
                rb_tree_set_black(uncle);
                rb_tree_set_red(x->parent->parent);
                x = x->parent->parent;
            }else{
                if(rb_tree_is_left_child(x)){  // case 4
                    x = x->parent;
                    rb_tree_rotate_right(x, root);
                }
                // case 5
                rb_tree_set_black(x->parent); 
                rb_tree_set_red(x->parent->parent);
                rb_tree_rotate_left(x->parent->parent, root);
                break;
            }
        }
    }
    rb_tree_set_black(root); // keep root black
}

// TODO
inline void rb_tree_erase_rebalance(rb_tree_node_base* z, rb_tree_node_base*& root, rb_tree_node_base*& leftmost, rb_tree_node_base*& rightmost){
    rb_tree_node_base* y = (z->left == nullptr || z->right == nullptr) ? z : rb_tree_get_next(z); // y is the node to be deleted
    rb_tree_node_base* x = nullptr;  // x is the child of y or nullptr
    rb_tree_node_base* xp = nullptr; // xp is the parent of x
    if(z != y){ // z have two children
        x = y->right; // y must have no left child, x may be nullptr
        z->left->parent = y;
        y->left = z->left;
        if(y != z->right){
            xp = y->parent;
            if(x != nullptr){
                x->parent = y->parent;
            }
            y->parent->left = x;
            y->right = z->right;
            z->right->parent = y;
        }else{
            xp = y;
        }
        if(z == root){
            root = y;
        }else if(rb_tree_is_left_child(z)){
            z->parent->left = y;
        }else{
            z->parent->right = y;
        }
        y->parent = z->parent;
        std::swap(y->color, z->color);
        y = z;
    }else{
        x = y->left != nullptr ? y->left : y->right;
        if(x != nullptr){
            x->parent = y->parent;
        }
        if(z == root){
            root = x;
        }else if(rb_tree_is_left_child(z)){
            z->parent->left = x;
        }else{
            z->parent->right = x;
        }
        xp = y->parent;
        if(z == leftmost){
            leftmost = (x == nullptr) ? xp : rb_tree_node_base::minimum(x);
        }
        if(z == rightmost){
            rightmost = (x == nullptr) ? xp : rb_tree_node_base::maximum(x);
        }
    }

    // 此时，y 指向要删除的节点，x 为替代节点，从 x 节点开始调整。
    // 如果删除的节点为红色，树的性质没有被破坏，否则按照以下情况调整（x 为左子节点为例）：
    // case 1: 兄弟节点为红色，令父节点为红，兄弟节点为黑，进行左（右）旋，继续处理
    // case 2: 兄弟节点为黑色，且两个子节点都为黑色或 NIL，令兄弟节点为红，父节点成为当前节点，继续处理
    // case 3: 兄弟节点为黑色，左子节点为红色或 NIL，右子节点为黑色或 NIL，
    //         令兄弟节点为红，兄弟节点的左子节点为黑，以兄弟节点为支点右（左）旋，继续处理
    // case 4: 兄弟节点为黑色，右子节点为红色，令兄弟节点为父节点的颜色，父节点为黑色，兄弟节点的右子节点
    //         为黑色，以父节点为支点左（右）旋，树的性质调整完成，算法结束
    if(rb_tree_is_black(y)){
        while(x != root &&(x == nullptr || rb_tree_is_black(x))){
            if(x == xp->left){
                rb_tree_node_base* bro = xp->right;
                if(rb_tree_is_red(bro)){ // case 1
                    rb_tree_set_black(bro);
                    rb_tree_set_red(xp);
                    rb_tree_rotate_left(xp, root);
                    bro = xp->right;
                }
                if((bro->left == nullptr || rb_tree_is_black(bro->left)) && (bro->right == nullptr || rb_tree_is_black(bro->right))){ // case 2
                    rb_tree_set_red(bro);
                    x = xp;
                    xp = xp->parent;
                }else{
                    if(bro->right == nullptr || rb_tree_is_black(bro->right)){ // case 3
                        rb_tree_set_black(bro->left);
                        rb_tree_set_red(bro);
                        rb_tree_rotate_right(bro, root);
                        bro = xp->right;
                    }
                    // case 4
                    bro->color = xp->color;
                    rb_tree_set_black(xp);
                    if(bro->right != nullptr){
                        rb_tree_set_black(bro->right);
                    }
                    rb_tree_rotate_left(xp, root);
                    break;
                }
            }else{
                rb_tree_node_base* bro = xp->left;
                if(rb_tree_is_red(bro)){ // case 1
                    rb_tree_set_black(bro);
                    rb_tree_set_red(xp);
                    rb_tree_rotate_right(xp, root);
                    bro = xp->left;
                }
                if((bro->left == nullptr || rb_tree_is_black(bro->left)) && (bro->right == nullptr || rb_tree_is_black(bro->right))){ // case 2
                    rb_tree_set_red(bro);
                    x = xp;
                    xp = xp->parent;
                }else{
                    if(bro->left == nullptr || rb_tree_is_black(bro->left)){ // case 3
                        rb_tree_set_black(bro->right);
                        rb_tree_set_red(bro);
                        rb_tree_rotate_left(bro, root);
                        bro = xp->left;
                    }
                    // case 4
                    bro->color = xp->color;
                    rb_tree_set_black(xp);
                    if(bro->left != nullptr){
                        rb_tree_set_black(bro->left);
                    }
                    rb_tree_rotate_right(xp, root);
                    break;
                }
            }
        }
        if(x != nullptr){
            rb_tree_set_black(x);
        }
    }
    return;
}


template <typename T, typename Compare>
class rb_tree{
public:
    typedef hstl::allocator<T>                          allocator_type;
    typedef hstl::allocator<T>                          data_allocator;
    typedef hstl::allocator<rb_tree_node<T>>            node_allocator;
    typedef typename rb_tree_node<T>::link_type         link_type;
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
    link_type& root() const{ return (link_type&) header_->parent; }
    link_type& leftmost() const{ return (link_type&) header_->left; }
    link_type& rightmost() const{ return (link_type&) header_->right; }

public:
    rb_tree();
    iterator insert_equal(const value_type& value); // insert value into rb_tree (allowing duplicate values)
    iterator erase(iterator position); // erase node at position
private:
    link_type get_node();
    link_type create_node(const value_type& value);
    void destroy_node(link_type p);
    iterator insert_node(base_ptr y_, const value_type& value);

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
        x = key_compare_(value, x->data) ? link_type(x->left) : link_type(x->right);
    }
    return insert_node(y, value);

}

template <typename T, typename Compare>
typename rb_tree<T, Compare>::iterator rb_tree<T, Compare>::erase(iterator position){
    link_type y = static_cast<link_type>(position.node);
    iterator next(y);
    ++next;
    rb_tree_erase_rebalance(base_ptr(y), header_->parent, header_->left, header_->right);
    destroy_node(y);
    --size_;
    return next;

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
void rb_tree<T, Compare>::destroy_node(link_type p){
    hstl::destroy(&(p->data));
    node_allocator::deallocate(p);
}

template <typename T, typename Compare>
typename rb_tree<T, Compare>::iterator rb_tree<T, Compare>::insert_node(base_ptr y_, const value_type& value){
    link_type y = static_cast<link_type>(y_);
    link_type z;
    if(y == header_ || key_compare_(value, y->data)){
        z = create_node(value);
        y->left = z;
        if(y == header_){
            root() = z;
            rightmost() = z;
        } else if(y == leftmost()){
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



 
} // namespace hstl
#endif