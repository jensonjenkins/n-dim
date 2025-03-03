#include <algorithm>
#include <iostream>
#include <numeric>
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


    ndim::array<ndim::inner_array<int, 10>, 3> b;
    static_assert(std::is_same_v<decltype(b)::reference, ndim::array_ref<int, 10>>);
    static_assert(std::is_same_v<decltype(b.data().data()), int*>);
    static_assert(std::is_same_v<decltype(b)::element_dim_type, ndim::static_dim<ndim::unit_dim, 10>>);

    for(auto x : b) {
        static_assert(std::is_same_v<decltype(x), ndim::array_ref<int, 10>>);
        for(auto& y : x) {
            static_assert(std::is_same_v<decltype(y), int&>);
        }
    }

    for(auto x = b.begin(); x != b.end(); x++) {
        static_assert(std::is_same_v<decltype(x), 
                ndim::iterator_intermediate_impl<ndim::inner_array<int, 10>, false>>);
        static_assert(std::is_same_v<decltype(*x), ndim::array_ref<int, 10>>);
        static_assert(std::is_same_v<decltype(x->begin()), ndim::iterator_lowest_impl<int, false>>);
        for(auto y = x->begin(); y != x->end(); y++) {}
    }

    std::cout<<"2 - ok: 2 dimension ndim::array"<<std::endl;

    for(int i=0;i<3;i++){
        for(int j=0;j<10;j++){
            b[i][j] = 10-j;
        }
    }

    for(auto x : b){
        std::sort(x.begin(), x.end());
        int inc = 1;
        for(auto y : x){
            assert(inc == y);
            inc++;
        }
    }
    std::cout<<"3 - ok: std::sort"<<std::endl;

    int ub = std::upper_bound(b[0].begin(), b[0].end(), 5) - b[0].begin();
    int lb = std::lower_bound(b[0].begin(), b[0].end(), 5) - b[0].begin();
    assert(ub == 5 && b[0][ub] == 6);
    assert(lb == 4 && b[0][lb] == 5);

    std::cout<<"4 - ok: std::lower_bound, std::upper_bound"<<std::endl;

    int mx = *std::max_element(b[0].begin(), b[0].end());
    int mn = *std::min_element(b[0].begin(), b[0].end());
    assert(mx == 10);
    assert(mn == 1);

    std::cout<<"5 - ok: std::max_element, std::min_element"<<std::endl;

    int sum = std::accumulate(b[0].begin(), b[0].end(), 0);
    assert(sum == 55);

    std::cout<<"6 - ok: std::accumulate"<<std::endl;

    exit(0);
}


