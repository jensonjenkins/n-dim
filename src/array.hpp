#pragma once

#include <vector>
#include "iterator.hpp"

namespace ndim {

template <typename T> 
class inner_array {
public:
    inner_array(T* data, const std::vector<size_t>& dims, size_t offset, size_t size, size_t dim_idx) 
        : data(data), dims(dims), offset(offset), size(size), dim_idx(dim_idx) {
            assert(dim_idx <= dims.size() && "dim_idx should not exceed dimensions size!");
        }

    inner_array<T> operator[](size_t index) {
        size_t stride = size / dims[dim_idx];
        assert(index < dims[dim_idx]);
        size_t next_offset = index * stride;
        return inner_array<T>(data, dims, offset + next_offset, stride, dim_idx + 1);
    }

    inner_array<T>& operator=(const T& value) {
        assert(this->size == 1 && "Unable to assign value to a range of elements!");
        data[offset] = value;
        return *this;
    }

    operator T&() { return data[offset]; }

    iterator<T> begin() { return iterator<T>(data + offset); }
    iterator<T> end() { return iterator<T>(data + offset + this->size); }
    iterator<T> rbegin(){ return iterator<T>(data + offset - 1); }
    iterator<T> rend(){ return iterator<T>(data + offset + this->size - 1); }

    size_t get_size() { return size; }
    size_t get_offset() { return offset; }
    size_t get_dim_idx() { return dim_idx; }

    std::vector<size_t> get_dims() { return dims; }

private:
    T* data;
    size_t size;
    size_t offset;
    size_t dim_idx;
    std::vector<size_t> dims;
};

template <typename T> 
class array {
public: 
    template <typename... Dims>
    array(Dims... dim) : dims{static_cast<size_t>(dim)...} {
        size = 1;
        for(size_t dim : dims) { assert(dim > 0 && "dimension must be > 0"); size *= dim; }
        data = new T[size]();
    }   
    array(const std::vector<size_t>& dims) : dims(dims) {
        size = 1;
        for(size_t dim : dims) { assert(dim > 0 && "dimension must be > 0"); size *= dim; }
        data = new T[size]();
    };

    ~array() { delete[] data; }

    inner_array<T> operator[](size_t index) {
        size_t stride = size / dims[0];
        assert(index < dims[0]);
        size_t offset = index * stride;
        return inner_array<T>(data, dims, offset, stride, 1);
    }
    
    array<T> operator+(const array<T>& other) {
        assert(dims == other.dims && "Dimensions do not match!");
        array<T> result(dims);
        for (int i = 0; i < size; i++) { result.data[i] = data[i] + other.data[i]; }
        return result;
    }
    array<T> operator-(const array<T>& other) {
        assert(dims == other.dims && "Dimensions do not match!");
        array<T> result(dims);
        for (int i = 0; i < size; i++) { result.data[i] = data[i] - other.data[i]; }
        return result;
    }
    array<T> operator*(const array<T>& other) {
        assert(dims == other.dims && "Dimensions do not match!");
        array<T> result(dims);
        for (int i = 0; i < size; i++) { result.data[i] = data[i] * other.data[i]; }
        return result;
    }
    array<T> operator/(const array<T>& other) {
        assert(dims == other.dims && "Dimensions do not match!");
        array<T> result(dims);
        for (int i = 0; i < size; i++) { result.data[i] = data[i] / other.data[i]; }
        return result;
    }
    array<T>& operator+=(const array<T>& other) {
        assert(dims == other.dims && "Dimensions do not match!");
        for (int i = 0; i < size; i++) { data[i] += other.data[i]; }
        return *this;
    }
    array<T>& operator-=(const array<T>& other) {
        assert(dims == other.dims && "Dimensions do not match!");
        for (int i = 0; i < size; i++) { data[i] -= other.data[i]; }
        return *this;
    }
    array<T>& operator*=(const array<T>& other) {
        assert(dims == other.dims && "Dimensions do not match!");
        for (int i = 0; i < size; i++) { data[i] *= other.data[i]; }
        return *this;
    }
    array<T>& operator/=(const array<T>& other) {
        assert(dims == other.dims && "Dimensions do not match!");
        for (int i = 0; i < size; i++) { data[i] /= other.data[i]; }
        return *this;
    }

    iterator<T> begin(){ return iterator<T>(data); }
    iterator<T> end(){ return iterator<T>(data + size); }
    iterator<T> rbegin(){ return iterator<T>(data - 1); }
    iterator<T> rend(){ return iterator<T>(data + size - 1); }

    size_t get_size() { return size; }

    std::vector<size_t> get_dims() { return dims; } 

private:
    T* data;
    size_t size;
    std::vector<size_t> dims;
};

} //namespace ndim

