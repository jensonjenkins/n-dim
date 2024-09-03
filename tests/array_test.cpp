#include <iostream>
#include <utility>
#include "../src/array.hpp"

int main(int argc, char* argv[]){    

    std::cout<<"Running array_test.cpp"<<std::endl;

    ndim::array<int> myvec(2, 2, 3);
    assert(myvec.get_size() == 12);
    std::cout<<"1 - ok"<<std::endl;
    
    ndim::inner_array<int> inner = myvec[0];
    assert(inner.get_size() == 6);
    assert(inner.get_offset() == 0);
    assert(inner.get_dim_idx() == 1);
    std::cout<<"2 - ok"<<std::endl;
    
    inner = myvec[1];
    assert(inner.get_size() == 6);
    assert(inner.get_offset() == 6);
    assert(inner.get_dim_idx() == 1);
    std::cout<<"3 - ok"<<std::endl;

    inner = myvec[1][1];
    assert(inner.get_size() == 3);
    assert(inner.get_offset() == 9);
    assert(inner.get_dim_idx() == 2);
    std::cout<<"4 - ok"<<std::endl;
    
    myvec[0][0][1] = 10;
    inner = myvec[0][0][1];
    assert(inner.get_size() == 1);
    assert(inner.get_offset() == 1);
    assert(inner.get_dim_idx() == 3);
    assert(inner == 10);
    std::cout<<"5 - ok"<<std::endl;
    
    ndim::array<int> vec1(3, 3, 3);
    ndim::array<int> vec2(3, 3, 3);

    vec1[0][0][0] = 11111;
    vec1[0][0][1] = 22222;
    vec1[2][2][1] = 88888;
    vec1[2][2][2] = 99999;

    vec2[0][0][0] = 111;
    vec2[0][0][1] = 222;
    vec2[2][2][1] = 888;
    vec2[2][2][2] = 999;
    
    ndim::array<int> vec3 = vec1 - vec2;

    assert(vec3[0][0][0] == 11000);
    assert(vec3[0][0][1] == 22000);
    assert(vec3[2][2][1] == 88000);
    assert(vec3[2][2][2] == 99000);
    std::cout<<"6 - ok"<<std::endl;

    ndim::array<int> vec4(2, 2);
    vec4[0][0] = 1;
    vec4[0][1] = 2;
    vec4[1][0] = 3;
    vec4[1][1] = 4;

    vec4*=vec4;
    assert(vec4[0][0] == 1);
    assert(vec4[0][1] == 4);
    assert(vec4[1][0] == 9);
    assert(vec4[1][1] == 16);
    std::cout<<"7 - ok"<<std::endl;

    ndim::array<int> veca(2, 3), vecb(4, 5);
    std::swap(veca, vecb);
    assert(veca.get_size() == 20);
    assert(vecb.get_size() == 6);
    std::cout<<"8 - ok"<<std::endl;

    exit(0);
}

