#pragma once

#include "core.hpp"
#include <cstddef>
#include <iterator>
#include <type_traits>

namespace std {
    struct contiguous_iterator_tag;
}

namespace ndim {

template <typename T, bool IsConst>
class iterator_base_impl {
public:
    using difference_type   = std::ptrdiff_t;
    using iterator_category = std::random_access_iterator_tag; //ideally contiguous_iterator_tag
    using value_type        = std::conditional_t<IsConst, 
          const typename element_traits<T>::value_type, typename element_traits<T>::value_type>;
    using pointer           = std::conditional_t<IsConst,
          typename element_traits<T>::const_pointer, typename element_traits<T>::pointer>;
    using reference         = std::conditional_t<IsConst, 
          typename element_traits<T>::const_reference, typename element_traits<T>::reference>;
    using base_element      = std::conditional_t<IsConst, 
          const typename element_traits<T>::base_element, typename element_traits<T>::base_element>;
    using element_dim_type  = typename element_traits<T>::dim_type;
};

/**
 * Iterator class for containers that contain other/nested containers.
 * i.e. std::is_base_of_v<inner_container_base>
 */
template <typename T, bool IsConst>
class iterator_intermediate_impl : public iterator_base_impl<T, IsConst> {
private:     
    using B = iterator_base_impl<T, IsConst>;
public:
    constexpr typename B::reference operator*() const noexcept { return *ref_; }
    constexpr const typename B::reference* operator->() noexcept { return &ref_; }

    constexpr iterator_intermediate_impl() noexcept = default;
    constexpr iterator_intermediate_impl(const iterator_intermediate_impl& other) noexcept 
        : ref_(other.ref_) {}
    constexpr iterator_intermediate_impl(typename B::base_element* data, 
            const typename B::element_dim_type& dim) noexcept : ref_(data, dim.inner()) {}

    constexpr iterator_intermediate_impl& operator=(const iterator_intermediate_impl& other) noexcept {
        assert(ref_.dims() == other.ref_.dims());
        ref_.copy(other.ref_);
    }
    constexpr iterator_intermediate_impl& operator++() noexcept { ref_.shift(1); return *this; }
    constexpr iterator_intermediate_impl operator++(int) noexcept { 
        auto tmp = *this; ++(*this); return tmp;
    }
    constexpr iterator_intermediate_impl& operator--() noexcept { ref_.shift(-1); return *this; }
    constexpr iterator_intermediate_impl operator--(int) noexcept { 
        auto tmp = *this; --(*this); return tmp;
    }
    constexpr iterator_intermediate_impl& operator+=(typename B::difference_type n) noexcept { 
        ref_.shift(n); return *this; 
    }
    constexpr iterator_intermediate_impl operator+(typename B::difference_type n) const noexcept {
        auto tmp = *this; return tmp += n;
    }
    constexpr iterator_intermediate_impl& operator-=(typename B::difference_type n) noexcept { 
        ref_.shift(-n); return *this; 
    }
    constexpr iterator_intermediate_impl operator-(typename B::difference_type n) const noexcept {
        auto tmp = *this; return tmp -= n;
    }

    constexpr bool operator==(const iterator_intermediate_impl& other) const { return ref_ == other.ref_; }
    constexpr bool operator!=(const iterator_intermediate_impl& other) const { return ref_ != other.ref_; }
    
// private:
    static_assert(std::is_base_of_v<inner_container_base, T>);
    typename B::reference ref_; // reference meaning reference to element T i.e. ndim::array_ref 
                                // which in reality is a wrapper for B::base_element* which
                                // is basically a pointer.
};

/**
 * Iterator class for containers that contain primitive types 
 * i.e. int, bool, float, etc.
 */
template <typename T, bool IsConst>
class iterator_lowest_impl : public iterator_base_impl<T, IsConst> {
private:
    using B = iterator_base_impl<T, IsConst>;

public: 
    constexpr typename B::reference operator*() const noexcept { return *ptr_; }
    constexpr typename B::pointer operator->() noexcept { return ptr_; }

    constexpr iterator_lowest_impl(typename B::base_element* data, 
            const typename B::element_dim_type&) noexcept : ptr_(data) {}
    constexpr iterator_lowest_impl() noexcept = default;

    constexpr iterator_lowest_impl& operator++() noexcept { ptr_++; return *this; }
    constexpr iterator_lowest_impl operator++(int) noexcept { 
        auto tmp = *this; ptr_++; return tmp; 
    }
    constexpr iterator_lowest_impl& operator--() noexcept { ptr_--; return *this; }
    constexpr iterator_lowest_impl operator--(int) noexcept { 
        auto tmp = *this; ptr_--; return tmp; 
    }
    constexpr iterator_lowest_impl& operator+=(typename B::difference_type n) noexcept { 
        ptr_ += n; return *this; 
    }
    constexpr iterator_lowest_impl operator+(typename B::difference_type n) const noexcept {
        auto tmp = *this; return tmp += n;
    }
    constexpr iterator_lowest_impl& operator-=(typename B::difference_type n) noexcept { 
        ptr_ -= n; return *this; 
    }
    constexpr iterator_lowest_impl operator-(typename B::difference_type n) const noexcept {
        auto tmp = *this; return tmp -= n;
    }

    constexpr bool operator==(const iterator_lowest_impl& other) const { return ptr_ == other.ptr_; }
    constexpr bool operator!=(const iterator_lowest_impl& other) const { return ptr_ != other.ptr_; }
    
private:
    static_assert(std::is_same_v<unit_dim, typename element_traits<T>::dim_type>);
    typename B::pointer ptr_;
};

template <typename T, bool IsConst>
using iterator_impl = std::conditional_t<std::is_base_of_v<inner_container_base, T>, 
      iterator_intermediate_impl<T, IsConst>, iterator_lowest_impl<T, IsConst>>;

template <typename T>
using iterator = iterator_impl<T, false>;

template <typename T>
using const_iterator = iterator_impl<T, true>;

} // namespace ndim



