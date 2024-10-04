#pragma once

#include <type_traits>
#include "core.hpp"
#include "iterator.hpp"

namespace ndim {

/**
 * Dimension information that is known at compile time.
 * @tparam D the dimension information of the next inner dimension
 * @tparam N size of the current dimension
 */
template <typename D, size_t N>
class static_dim {
public:
    constexpr size_t stride() const noexcept { return N * inner_static_dim_.stride(); } // recurse?
    constexpr size_t top_dim() const noexcept { return N; }
    constexpr const D& inner() const noexcept { return inner_static_dim_; }
    constexpr static bool is_dynamic = false;
    
    constexpr static_dim() = default;

private:
    D inner_static_dim_;
};

/**
 * n-dimensional array base class
 * @tparam T        element contents of derived class
 * @tparam N        top dimension size
 * @tparam Owning   object owns the data (false if a reference type)
 * @tparam IsConst   
 */
template <typename T, size_t N, bool Owning, bool IsConst>
class array_base {
public:
    // Member types
    using size_type             = size_t;
    using difference_type       = std::ptrdiff_t;
    using value_type            = typename element_traits<T>::value_type;
    using reference             = typename element_traits<T>::reference;
    using const_reference       = typename element_traits<T>::const_reference;
    using pointer               = typename element_traits<T>::pointer;
    using const_pointer         = typename element_traits<T>::const_pointer;

    using iterator              = ndim::iterator<T>;
    using const_iterator        = ndim::const_iterator<T>;

    using base_element          = typename element_traits<T>::base_element;
    using element_dim_type      = typename element_traits<T>::dim_type;
    using container_dim_type    = static_dim<element_dim_type, N>;
    
    //still type of ndim::fixed_buffer or whatever buffer under the hood
    using buffer_type           = add_dim_to_buffer_t<typename element_traits<T>::buffer_type, N>; 
    
    // Public member functions
    constexpr size_type size() const noexcept { return N; }
    constexpr size_type max_size() const noexcept { return N; }
    constexpr bool empty() const noexcept { return N == 0; }
 
protected:
    using underlying_store = std::conditional_t<Owning, buffer_type, 
          std::conditional_t<IsConst, const base_element*, base_element*>>;
    
    constexpr array_base() = default;
    constexpr array_base(base_element* data, const element_dim_type& dim) : data_(data), dims_(dim) {};
    constexpr array_base(const array_base&) = delete; // no copy constructor
    constexpr array_base& operator=(const array_base&) = delete; // no copy assignment
    
    underlying_store data_;
    element_dim_type dims_;
};

template <typename T, size_t N>
class array : public array_base<T, N, true, false> {
private:
    using B = array_base<T, N, true, false>;
public:
    constexpr array() : B() {};

    constexpr typename B::underlying_store data() noexcept { return this->data_; }
    constexpr const typename B::underlying_store data() const noexcept { return this->data_; }

    constexpr typename B::base_element* data_offset(typename B::size_type index) noexcept {
        return this->data_.data() + index * this->dims_.stride();
    }
    
    /**
     * Returns a reference to the inner referenced element
     *
     * @param index of the ndim::array
     *
     * @return a reference to the indexed subdimension, constructed by a call to B::reference(),
     *     B::reference() calls the underlying inner element's constructor (i.e. ndim::array_ref).
     */
    constexpr typename B::reference operator[](typename B::size_type index) noexcept {
        assert(index < N);
        if constexpr (element_traits<T>::is_inner_container){
            return typename B::reference(data_offset(index), this->dims_.inner());
        } else {
            static_assert(std::is_same_v<unit_dim, typename B::element_dim_type>, "Must be unit_dim");
            return this->data_[index];
        }
    }
    constexpr const typename B::reference operator[](typename B::size_type index) const noexcept {
        assert(index < N);
        if constexpr (element_traits<T>::is_inner_container){
            return typename B::reference(data_offset(index), this->dims_.inner());
        } else {
            static_assert(std::is_same_v<unit_dim, typename B::element_dim_type>, "Must be unit_dim");
            return this->data_[index];
        }
    }
    
    constexpr typename B::iterator begin() noexcept { 
        return typename B::iterator(this->data_.data(), this->dims_); 
    }
    constexpr typename B::iterator end() noexcept {
        return typename B::iterator(this->data_.data() + N, this->dims_); 
    }
    constexpr typename B::const_iterator begin() const noexcept {
        return typename B::const_iterator(this->data_.data(), this->dims_);
    }
    constexpr typename B::const_iterator end() const noexcept {
        return typename B::const_iterator(this->data_.data() + N, this->dims_);
    }
    constexpr typename B::const_iterator cbegin() const noexcept {
        return typename B::const_iterator(this->data_.data(), this->dims_);
    }
    constexpr typename B::const_iterator cend() const noexcept {
        return typename B::const_iterator(this->data_.data() + N, this->dims_);
    }

    constexpr typename B::reference front() noexcept { return operator[](0); }
    constexpr typename B::const_reference front() const noexcept { return operator[](0); }
    constexpr typename B::reference back() noexcept { return operator[](N - 1); }
    constexpr typename B::const_reference back() const noexcept { return operator[](N - 1); }
};

/**
 * A reference to a subdimension of the ndim::array. As it is a reference to some subdimension 
 * in the array, it will have constant size (i.e. will not copy/create a copy of the referenced subdimension.)
 */
template <typename T, size_t N>
class array_ref : public array_base<T, N, false, false> {
private:
    using B = array_base<T, N, false, false>;
public:
    constexpr array_ref() : B() {};
    constexpr array_ref(typename B::base_element* data, const typename B::element_dim_type& dim) noexcept 
        : B(data, dim) {};
    
    constexpr typename B::base_element* data_offset(typename B::size_type index) noexcept {
        return this->data_.data() + index * this->dims_.stride();
    }
    
    /**
     * Shifts the current pointer n objects away. 
     * n objects away meaning maintaining the same dimension as the originally pointed object.
     */
    constexpr void shift(typename B::size_type n) noexcept { this->data_ + n * this->dims_.stride(); }

    constexpr typename B::reference operator[](typename B::size_type index) noexcept {
        assert(index < N);
        if constexpr (element_traits<T>::is_inner_container){
            return typename B::reference(data_offset(index), this->dims_.inner());
        } else {
            static_assert(std::is_same_v<unit_dim, typename B::element_dim_type>, "Must be unit_dim");
            return this->data_[index];
        }
    }
    constexpr const typename B::reference operator[](typename B::size_type index) const noexcept {
        assert(index < N);
        if constexpr (element_traits<T>::is_inner_container){
            return typename B::reference(data_offset(index), this->dims_.inner());
        } else {
            static_assert(std::is_same_v<unit_dim, typename B::element_dim_type>, "Must be unit_dim");
            return this->data_[index];
        }
    }
};

template <typename T, size_t N>
class array_const_ref : public array_base<T, N, false, true> {};

template <typename T, size_t N>
class inner_array : public inner_container<array_ref<T, N>, array_const_ref<T, N>> {};

} //namespace ndim


