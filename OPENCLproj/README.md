# **Bitonic Sort**

![Screenshot](pics/sort.jpg)

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
$ g++ -std=c++2a -Wall -Wextra -Wpedantic -Wsign-promo -Wdouble-promotion -Wfloat-equal -Wdiv-by-zero -O2
```

## To run the program, enter
```
$ ./sort
```
---
## Input data:
```
N - size of array
N - numbers
```
## Output data:
```
The sorting array
```
---
# I implemented parallel variant of bitonic sort on GPU. You can find all idea of sort [here](https://en.wikipedia.org/wiki/Bitonic_sorter)
---
## I choose the first platform, which have GPU device, but if you want run this code on platform "NVIDIA CUDA" add -DNVIDIA=on in cmake params, also if you want to know time of sort add -DTIMER=on too.
---
## Implemented by second-year MIPT student Ivanov Ivan