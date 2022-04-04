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

## On my GPU (NVIDIA GeForce RTX 2060) and CPU (AMD Ryzen 5 3550H) I got this data

<image
  src="pics/Figure_1.png"
  alt="schedule of time"
  caption="dependence of the total sorting time on the size of the input data">

## Also I compare cl::Buffer and SVM:

<image
  src="pics/Figure_2.png"
  alt="schedule of time"
  caption="Comparing SVM & cl::buffer">

---
## I choose the first platform, which have GPU device, but if you want run this code on platform "NVIDIA CUDA" add -DNVIDIA=on in cmake params. Also you can add -DSHARED for using shared memory instead cl::Buffer 
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