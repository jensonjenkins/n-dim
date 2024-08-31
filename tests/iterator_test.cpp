#include <algorithm>
#include <iostream>
#include "../src/array.hpp"
#include "../src/iterator.hpp"

int main(int argc, char* argv[]){    

    std::cout<<"Running iterator_test.cpp"<<std::endl;

    ndim::array<int> myvec(3, 3, 3);

    myvec[0][0][0] = 988423;
    myvec[2][2][2] = 123456;

    ndim::iterator<int> it_begin =  myvec.begin();
    ndim::iterator<int> it_end = myvec.end();
    --it_end;

    assert(*it_begin == 988423);
    assert(*it_end == 123456);
    std::cout<<"1 - ok"<<std::endl;
    
    myvec[0][0][0] = 3;
    myvec[0][0][1] = 2;
    myvec[0][0][2] = 1;
    ndim::inner_array<int> inner = myvec[0][0];

    std::sort(inner.begin(), inner.end());
    assert(inner[0] == 1);
    assert(inner[1] == 2);
    assert(inner[2] == 3);
    std::cout<<"2 - ok"<<std::endl;

    std::reverse(inner.begin(), inner.end());
    assert(inner[0] == 3);
    assert(inner[1] == 2);
    assert(inner[2] == 1);
    std::cout<<"3 - ok"<<std::endl;
    
    myvec[1][1][0] = 44444;
    myvec[1][1][2] = 66666;
    ndim::inner_array<int> inner2 = myvec[1][1];

    ndim::iterator<int> ib = inner2.begin();
    ndim::iterator<int> ie = inner2.end();
    --ie;

    assert(*ib == 44444);
    assert(*ie == 66666);
    std::cout<<"4 - ok"<<std::endl;
    
    exit(0);
}

