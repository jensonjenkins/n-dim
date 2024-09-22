#pragma once

#include <array>

namespace ndim{

/**
 * Immutable buffer after defined at compile time
 */
template<typename T, size_t N>
class fixed_buffer{
public:
    constexpr T* data() noexcept { return buf_.data(); }
    constexpr const T* data() const noexcept { return buf_.data(); }

    constexpr fixed_buffer() = default;
    constexpr fixed_buffer(const std::array<T, N>& buf) noexcept : buf_(buf) {};
    
    constexpr fixed_buffer(const fixed_buffer& other) noexcept = delete; //copy 
    constexpr fixed_buffer(fixed_buffer&& other) noexcept = default; // move
    constexpr fixed_buffer& operator=(const fixed_buffer& other) noexcept = delete; //copy assignment 
    constexpr fixed_buffer& operator=(fixed_buffer&& other) noexcept = default; //move assignment

    constexpr T& operator[](size_t index) noexcept { return buf_[index]; }
    constexpr const T& operator[](size_t index) const noexcept { return buf_[index]; }

    constexpr size_t size() const noexcept { return N; }

    constexpr fixed_buffer clone() noexcept(std::is_nothrow_copy_constructible_v<T>) { return fixed_buffer(buf_); }
    constexpr void swap(const fixed_buffer& a, const fixed_buffer& b) noexcept(std::is_nothrow_swappable_v<T>) { 
        std::swap(a, b);
    }

private:
    std::array<T, N> buf_;
};

} // namespace ndim

