#include <complex>
#include <iostream>

#include "matrix.hpp"

int main ()
{
    int n;
    std::cin >> n;

    matrix::Matrix<double> matr (n, n);

    std::cin >> matr;
    std::cout << matr.det () << std::endl;

    return 0;
}