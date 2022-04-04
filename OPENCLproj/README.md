# **Bitonic Sort**

![Screenshot](pics/sort.jpg)

---
## How to build my project:
```
$ cmake -B build && cd build
$ cmake --build .
```

## Compiler settings:
```
$ std=c++17, O2, (-Wall -Wextra | /W4)
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
## I choose the first platform, which have GPU device, but if you want run this code on platform "NVIDIA CUDA" add -DNVIDIA=on in cmake params. 
---
## If you want to check correct of this program you should run 
```
$ cmake --build . --target check_correct
```
## If you want to compare speed of this program on your GPU device with std::sort you should run
```
$ cmake --build . --target check_perf
```

---
## Implemented by second-year MIPT student Ivanov Ivan