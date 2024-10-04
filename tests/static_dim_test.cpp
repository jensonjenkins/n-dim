#include <iostream>
#include "../src/core.hpp"
#include "../src/array.hpp"

int main(int argc, char* argv[]){    
    std::cout<<"Running static_dim_test.cpp"<<std::endl;
    
    ndim::static_dim<ndim::static_dim<ndim::unit_dim, 2>, 3> a; 
    
    static_assert(a.stride() == 6);
    static_assert(a.top_dim() == 3);
    static_assert(a.inner().top_dim() == 2);

    std::cout<<"1 - ok"<<std::endl;

    exit(0);
}

