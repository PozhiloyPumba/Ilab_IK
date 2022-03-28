# **Matrix**

![Screenshot](pics/cat.jpg)

---
## How to build my project:
```
$ mkdir build && cd build
$ cmake ..
$ make
```
## If you haven't gtests you can install it go to 3rdParty/ from root of repository
```
$ git submodule init
$ git submodule update
$ sudo cp googletest /usr/src/gtest && cd /usr/src/gtest
$ sudo cmake CMakeLists.txt
$ sudo make
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
N - size of a square matrix
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