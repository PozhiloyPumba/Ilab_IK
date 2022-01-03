#include <complex>
#include <iostream>

#include "matrix.hpp"

int main ()
{
    int n;
    std::cin >> n;

    matrix::Matrix<std::complex<double>> matr(n, n);// = matrix::Matrix<int>::randomIntMatrix (n, det);

    std::cin >> matr;
    std::cout << matr.det() << std::endl;

    return 0;
}