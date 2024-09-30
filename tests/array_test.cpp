#include <cstdint>
#include <iostream>
#include "../src/array.hpp"

int main(int argc, char* argv[]){    
    std::cout<<"Running array_test.cpp"<<std::endl;

    ndim::array<ndim::inner_array<int, 5>, 3> a;
    static_assert(std::is_same_v<decltype(a.dims_.inner()), const ndim::unit_dim&>);
    
    static_assert(std::is_same_v<decltype(a[0]), ndim::array_ref<int, 5>>);
    static_assert(std::is_same_v<decltype(a[0])::element_dim_type, ndim::unit_dim>);
    static_assert(std::is_same_v<decltype(a[0][0]), int&>);

    static_assert(std::is_same_v<decltype(a)::base_element, int>);
    static_assert(std::is_same_v<decltype(a)::element_dim_type, ndim::static_dim<ndim::unit_dim, 5>>);
    static_assert(std::is_same_v<decltype(a)::buffer_type, ndim::fixed_buffer<int, 15>>);

    assert(a[0].size() == 5);
    a[0][0] = 1;
    a[1][4] = 123;
    assert(a[0][0] == 1);
    assert(a[1][4] == 123);

    ndim::array<ndim::inner_array<ndim::inner_array<int64_t, 10>, 5>, 3> b;
    static_assert(std::is_same_v<decltype(b[0]), ndim::array_ref<ndim::inner_array<int64_t, 10>, 5>>);
    static_assert(std::is_same_v<decltype(b[0][0][0]), int64_t&>);
    static_assert(std::is_same_v<decltype(b)::base_element, int64_t>);
    static_assert(std::is_same_v<decltype(b)::buffer_type, ndim::fixed_buffer<int64_t, 150>>);

    ndim::array<int, 5> c;
    c[0] = 1;
    c[1] = 5;
    c[2] = 123;
    assert(c.size() == 5);
    assert(c[0] == 1);
    assert(c[1] == 5);
    assert(c[2] == 123);

    ndim::array<int, 5>& ref = c;

    ref[0] = 5;
    static_assert(std::is_same_v<decltype(ref[0]), int&>);
    assert(ref[0] == 5);
    assert(c[0] == 5);
    assert(ref[1] == 5);
    assert(ref[2] == 123);

    std::cout<<"ok"<<std::endl;
    
    exit(0);
}

