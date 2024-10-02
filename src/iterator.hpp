#pragma once

#include "core.hpp"
#include <cstddef>
#include <iterator>
#include <type_traits>
namespace ndim {

template <typename T, bool IsConst>
class iterator_base_impl {
public:
    using iterator_category = std::random_access_iterator_tag; //ideally contiguous_iterator_tag
    using value_type = T;
    using difference_type = std::ptrdiff_t;
    using pointer = T*;
    using reference = T&;
    
    T& operator*() const { return *ptr_; }
    T* operator->() { return ptr_; }
    
    iterator_base_impl& operator++() { ptr_++; return *this; }
    iterator_base_impl operator++(int) { iterator_base_impl tmp = *this; ptr_++; return tmp; }

    bool operator==(const iterator_base_impl& other) const { return ptr_ == other.ptr_; }
    bool operator!=(const iterator_base_impl& other) const { return ptr_ != other.ptr_; }
    
protected:
    T* ptr_;
};

/**
 * Iterator class for containers that contain other/nested containers.
 * i.e. std::is_base_of_v<inner_container_base>
 */
template <typename T, bool IsConst>
class iterator_intermediate_impl : public iterator_base_impl<T, IsConst> {
    
};

/**
 * Iterator class for containers that contain primitive types 
 * i.e. int, bool, float, etc.
 */
template <typename T, bool IsConst>
class iterator_lowest_impl : public iterator_base_impl<T, IsConst> {
};


} // namespace ndim



