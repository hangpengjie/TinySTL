#include "../TinySTL/allocator.h"
#include <iostream>
struct A{
    A(){
        std::cout << "A()" << std::endl;
    }
    ~A(){
        std::cout << "~A()" << std::endl;
    }

};


void test_1(){
    hstl::allocator<A> alloc;
    A *a = alloc.allocate();
    alloc.construct(a);
    alloc.destroy(a);
    alloc.deallocate(a);
    std::cout << "allocate test 1 passed" << std::endl;

}
void test_2(){
    hstl::allocator<A> alloc;
    
    A *a = alloc.allocate(10);
    std::cout << "Allocated " << 10 * sizeof(A) << " bytes at " << static_cast<void*>(a) << std::endl;
    // 构造10个对象
    for (size_t i = 0; i < 10; ++i) {
        alloc.construct(a + i);
    }

    // 销毁10个对象
    for (size_t i = 0; i < 10; ++i) {
        alloc.destroy(a + i);
    }
    
    
    // 释放内存
    alloc.deallocate(a,10);
    std::cout << "Deallocated " << 10 * sizeof(A) << " bytes at " << static_cast<void*>(a) << std::endl;
    std::cout << "allocate test 2 passed" << std::endl;

}