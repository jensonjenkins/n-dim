#pragma once

#include <type_traits>
#include "fixed_buffer.hpp"

namespace ndim {

struct inner_container_base {};

template <typename Container, typename Ref, typename ConstRef>
struct inner_container : inner_container_base {
    using container_type = Container;
    using container_ref_type = Ref;
    using container_const_ref_type = ConstRef;
    using container_dim_type = typename Container::container_dim_type;
};

struct unit_dim {
    constexpr unit_dim() = default;
    constexpr static size_t stride() noexcept { return 1; }
};

template <typename Buffer, size_t M>
struct add_dim_to_buffer;

template <typename T, size_t N, size_t M>
struct add_dim_to_buffer<fixed_buffer<T, N>, M> {
    using type = fixed_buffer<T, N * M>;
};

template <typename Buffer, size_t M>
using add_dim_to_buffer_t = typename add_dim_to_buffer<Buffer, M>::type;


/**
 * Type traits for container (subclass of inner_container_base)
 * or non-container (i.e. int, float, etc.)
 */
template <typename T, typename = void>
struct element_traits {
    using value_type = T;
    using reference = T&;
    using const_reference = const T&;
    using pointer = T*;
    using const_pointer = const T*;
    using dim_type = unit_dim;
    using base_element = value_type;
    using buffer_type = fixed_buffer<T, 1>;
    constexpr static bool is_inner_container = false;
};

template <typename T>
struct element_traits<T, std::enable_if_t<std::is_base_of_v<inner_container_base, T>>> {
    using value_type = void;
    using reference = typename T::container_ref_type;
    using const_reference = typename T::container_const_ref_type;
    using pointer = void;
    using const_pointer = void;
    using dim_type = typename T::container_dim_type;
    using base_element = typename reference::base_element;
    using buffer_type = typename reference::buffer_type;
    constexpr static bool is_inner_container = true;
};

} // namespace ndim



