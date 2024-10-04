#include <iostream>
#include "../src/array.hpp"

int main(int argc, char* argv[]){    
    std::cout<<"Running iterator_test.cpp"<<std::endl;

    ndim::array<int, 5>a;
    a[0] = 1123;
    a[4] = 1234;
    auto it = a.begin();
    assert(*it == a.front());
    it = a.end();
    it--;
    assert(*it == a.back());

    auto cit = a.cbegin();
    static_assert(std::is_same_v<decltype(cit), ndim::iterator_lowest_impl<int, true>>);
    static_assert(std::is_same_v<decltype(cit)::pointer, const int*>);
    static_assert(std::is_same_v<decltype(cit)::base_element, const int>);
    static_assert(std::is_same_v<decltype(cit)::reference, const int&>);
    static_assert(std::is_same_v<decltype(*cit), const int&>);

    assert(*cit == a.front());
    cit = a.cend();
    cit--;
    assert(*cit == a.back());

    std::cout<<"1 - ok"<<std::endl;


    exit(0);
}

