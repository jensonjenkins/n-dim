#include <iostream>
#include "../src/array.hpp"

int main(int argc, char* argv[]){    
    std::cout<<"Running iterator_test.cpp"<<std::endl;

    ndim::array<int, 5>a;
    a[0] = 1123;
    auto it = a.begin();
    assert(*it == 1123);

    std::cout<<"1 - ok"<<std::endl;

    exit(0);
}

