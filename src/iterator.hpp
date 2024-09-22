#pragma once

namespace ndim {

/**
 */
template <typename T, bool IsConst>
class iterator_base_impl {
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



