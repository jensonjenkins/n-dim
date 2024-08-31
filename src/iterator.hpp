#pragma once

#include <cstddef>
#include <iterator>

namespace ndim {

template <typename T>
class iterator {
public:
    using value_type = T;
    using pointer = T*;
    using reference = T&;
    // NOTE: c++20 has std::contiguous_iterator_tag, maybe can change later
    using iterator_category = std::bidirectional_iterator_tag; 
    using difference_type = std::ptrdiff_t;

    iterator(T* ptr): ptr(ptr) {}

    reference operator*() const { return *ptr; }

    pointer operator->() { return ptr; }

    difference_type operator-(const iterator& other) const { return ptr - other.ptr; }
    iterator operator+(difference_type delta) {
        iterator temp = *this; temp.ptr += delta;
        return temp;
    }
    iterator& operator+=(difference_type delta) { ptr += delta; return *this; }
    iterator& operator-=(difference_type delta) { ptr -= delta; return *this; }

    // Pre-increment (e.g. ++it)
    iterator& operator++() { ptr++; return *this; }
    iterator& operator--() { ptr--; return *this; }

    // Post-increment (e.g. it++)
    iterator& operator++(int) { iterator iterator_cpy = *this; ptr++; return iterator_cpy; }
    iterator& operator--(int) { iterator iterator_cpy = *this; ptr--; return iterator_cpy; }

    bool operator>(const iterator& other) const { return ptr > other.ptr; }
    bool operator<(const iterator& other) const { return ptr < other.ptr; }
    bool operator>=(const iterator& other) const { return ptr >= other.ptr; }
    bool operator<=(const iterator& other) const { return ptr <= other.ptr; }
    bool operator==(const iterator& other) const { return ptr == other.ptr; }
    bool operator!=(const iterator& other) const { return ptr != other.ptr; }
    
private:
    T* ptr;
};

}// namespace ndim
