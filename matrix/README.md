# **Matrix**

## How to build my project:
```
$ git submodule init
$ git submodule update
$ mkdir build && cd build
$ cmake ..
$ make
```

## Compiler settings:
```
$ g++ -std=c++2a -Wall -Wextra -Wpedantic -Wsign-promo -Wsign-conversion -Wdouble-promotion -Wfloat-equal -Wbool-compare -Wdiv-by-zero -O2
```

## To run the program, enter
```
$ ./matrix
```
---
## Input data:
```
n - size of a square matrix
NхN numbers
```
## Output data:
```
The determinant of the matrix
```
---
# The calculation of the determinant is based on the Gaussian elimination
---
## Implemented by second-year MIPT student Ivanov Ivan