#include <iostream>
#include <vector>
#include "../src/array.hpp"

int main(int argc, char* argv[]){    
    std::cout<<"Running iterator_test.cpp"<<std::endl;

    ndim::array<int, 5> a;
    for(int i=0;i<a.size();i++) { a[i] = i*i; }

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

    std::vector<int> res1, res2;
    for(auto it = a.begin(); it != a.end(); it++) { res1.push_back(*it); }
    for(int x : a) { res2.push_back(x); }
    assert(res1 == res2);

    std::cout<<"1 - ok: single dimension"<<std::endl;


    ndim::array<ndim::inner_array<int, 5>, 3> b;
    static_assert(std::is_same_v<decltype(b)::reference, ndim::array_ref<int, 5>>);
    static_assert(std::is_same_v<decltype(b.data().data()), int*>);
    static_assert(std::is_same_v<decltype(b)::element_dim_type, ndim::static_dim<ndim::unit_dim, 5>>);

    auto it2 = b.begin();
 
    for(auto x = b.begin(); x != b.end(); x++){
        std::cout<<"outer"<<std::endl;
        static_assert(std::is_same_v<decltype(x), 
                ndim::iterator_intermediate_impl<ndim::inner_array<int, 5>, false>>);
        static_assert(std::is_same_v<decltype(*x), ndim::array_ref<int, 5>>);
        static_assert(std::is_same_v<decltype(x->begin()), ndim::iterator_lowest_impl<int, true>>);

        for(auto y = x->begin(); y != x->end(); y++){
            std::cout<<"inner"<<std::endl;
        }
    }

    std::cout<<"2 - ok: 2 dimension ndim::array"<<std::endl;

    exit(0);
}


