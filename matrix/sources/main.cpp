#include <complex>
#include <iostream>

#include "matrix.hpp"

int main ()
{
    int n;
    std::cin >> n;

    matrix::Matrix<int> matr (n, n);

    std::cin >> matr;
    
    std::cout << matr.det () << std::endl;

    return 0;
}