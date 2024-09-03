# n-dim
A set of header files defining a multi-dimensional array that allocates memory in one contiguous block.

### To build:

```
mkdir build
cd build
cmake ..
cmake --build .
```

### Example:
```cpp
ndim::array<int> vec(3, 3, 3);

for(int i=0;i<3;i++){
    for(int j=0;j<3;j++){
        for(int k=0;k<3;k++){
            vec[i][j][k] = i*j*k;
        }
    }
}

assert(vec[0][0][0] == 0);
assert(vec[1][2][1] == 2);
assert(vec[2][1][2] == 4);
assert(vec[2][2][2] == 8);
```
