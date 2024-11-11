#include "../TinySTL/vector.h"
#include <iostream>
#include <cassert>
void test_1(){
    hstl::vector<int> v;
    assert(v.size() == 16);
    std::cout << "vector test 1 passed" << std::endl;    
}
