#pragma once

#include <type_traits>
#include "core.hpp"

namespace ndim {

/**
 * Dimension information that is known at compile time.
 * @tparam D the dimension information of the next inner dimension
 * @tparam N size of the current dimension
 */
template<typename D, size_t N>
class static_dim {
public:
    constexpr size_t stride() const noexcept { return N * inner_static_dim_.stride(); } // recurse?
    constexpr size_t top_dim() const noexcept { return N; }
    constexpr const D& inner() const noexcept { return inner_static_dim_; }
    constexpr static bool is_dynamic = false;
    
    constexpr static_dim() = default;

    constexpr bool operator==(const static_dim& other) noexcept {
        return other.inner() == inner_static_dim_ && N == other.top_dim();
    }
    constexpr bool operator!=(const static_dim& other) noexcept { return !(this == other); }

private:
    D inner_static_dim_;
};

template <typename Array, typename T, size_t N, bool Owning, bool IsConst>
class array_base {
public:
    // Member types
    using value_type = typename element_traits<T>::value_type;
    using size_type = size_t;
    using difference_type = std::ptrdiff_t;
    using reference = typename element_traits<T>::reference;
    using const_reference = typename element_traits<T>::const_reference;
    using pointer = typename element_traits<T>::pointer;
    using const_pointer = typename element_traits<T>::const_pointer;

    using base_element = typename element_traits<T>::base_element;
    using element_dim_type = typename element_traits<T>::dim_type;
    using container_dim_type = static_dim<element_dim_type, N>;

    // still ndim::fixed_buffer (or any other type of ndim buffers) underneath
    using buffer_type = add_dim_to_buffer_type<typename element_traits<T>::buffer_type, N>; 
    
    // Public member functions
    constexpr size_type size() const noexcept { return N; }
    constexpr size_type max_size() const noexcept { return N; }
    constexpr bool empty() const noexcept { return N == 0; }
    
protected:
    using underlying_store = std::conditional_t<Owning, buffer_type, 
          std::conditional_t<IsConst, const base_element*, base_element*>>;

    constexpr array_base(const element_dim_type& dims, const underlying_store& data) 
        : dims_(dims), data_(data) {}
    constexpr array_base(const array_base&) = delete; // no copy constructor
    constexpr array_base& operator=(const array_base&) = delete; // no copy assignment
                                                                 
    /**
     * Swaps current ndim array with another ndim array
     */
    constexpr void swap(array_base& other) noexcept {
        std::swap(data_, other.data_);
        std::swap(dims_, other.dims_);
    }

    /**
     * Get dimension information that the current array contains
     */
    constexpr const element_dim_type& dim() const noexcept {
        return dims_;
    }
    
    underlying_store data_;
    element_dim_type dims_;
};

template <typename T, size_t N>
class array : array_base<array<T, N>, T, N, true, false> { // CRTP?
private:
    using B = array_base<array<T, N>, T, N, true, false>;
public:
    constexpr array(const array& other) : B(other.dims_, other.data_.clone()) {}

    constexpr typename B::reference operator[](typename B::typesize index) noexcept {}
    constexpr typename B::const_reference operator[](typename B::typesize index) const noexcept {}
private:

};

template <typename T, size_t N>
class array_ref : array_base<array_ref<T, N>, T, N, false, false> {
public:
private:
};

template <typename T, size_t N>
class array_const_ref : array_base<array_const_ref<T, N>, T, N, false, true>{
public:
private:
};

} //namespace ndim


