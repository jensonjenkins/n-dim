#include <iostream>
#include "../src/array.hpp"

int main(int argc, char* argv[]){    
    std::cout<<"Running array_test.cpp"<<std::endl;

    
    ndim::array<ndim::inner_array<ndim::inner_array<int, 2>, 5>, 2> a1;
    /** 
     * static_dim and unit_dim appears to not have any member variables, so they 
     * take up one byte each for memory address, i think?
     * thats why sizeof(element_dim_type) is always 1 for ndim::arrays
     */
    assert(sizeof(decltype(a1)::buffer_type) == 80);
    assert(sizeof(decltype(a1)::element_dim_type) == 1);
    
    /**
     * size of ndim::inner_array is also always 1 no matter the element type and N value, 
     * it has no member variables on its own, its just a container that contains types
     */
    assert(sizeof(ndim::inner_array<int, 1000>) == 1);
    
    assert(sizeof(decltype(a1)) == 84); // does the extra 3 bytes come from padding? 
                                        // does this depend on compiler/machine?

    std::cout<<"1 - ok"<<std::endl;


    ndim::array<ndim::inner_array<int, 5>, 3> a;

    static_assert(std::is_same_v<decltype(a[0]), ndim::array_ref<int, 5>>);
    static_assert(std::is_same_v<decltype(a[0])::element_dim_type, ndim::unit_dim>);
    static_assert(std::is_same_v<decltype(a[0][0]), int&>);

    static_assert(std::is_same_v<decltype(a)::base_element, int>);
    static_assert(std::is_same_v<decltype(a)::element_dim_type, ndim::static_dim<ndim::unit_dim, 5>>);
    static_assert(std::is_same_v<decltype(a)::buffer_type, ndim::fixed_buffer<int, 15>>);

    a[0][0] = 1;
    a[1][4] = 123;
    assert(a[0][0] == 1);
    assert(a[1][4] == 123);
    assert(a[0].size() == 5);

    std::cout<<"2 - ok"<<std::endl;


    ndim::array<ndim::inner_array<int, 10>, 8> b;

    /**
     *  Calling operator[] on both ndim::array (or ndim::array_ref) will return 
     *  an ndim::array_ref (or base_element&) which represents a reference to the 
     *  indexed element of the ndim array. This ndim::array_ref will consistently
     *  take up 16 bytes, as indexing will not create a copy of the of the indexed
     *  element in the ndim::array. This is due to the underlying_store of the array_ref
     *  being a pointer to the base element.
     */
    assert(sizeof(int*) == 8);
    assert(sizeof(decltype(b[0])::base_element*) == 8);
    assert(sizeof(decltype(b[0])::element_dim_type) == 1);
    assert(sizeof(b[0]) == 16); // does the extra 7 bytes come from padding? 
                                // does this depend on compiler/machine?

    std::cout<<"3 - ok"<<std::endl;
    
    exit(0);
}

