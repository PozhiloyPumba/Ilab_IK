#include "app.hpp"

int main ()
{
    OpenCLApp::App app ("../sources/bitonicSort.cl");

    cl::vector<int> test {10,7,-2,9,0,-9,5,3};

    // app.GPUBitonicSort(test.data(), test.size());
    app.CPUBitonicSort (test.data(), test.size());

    std::cout << "sort" << std::endl;
    for (auto el: test)
        std::cout << el << " ";

    std::cout << std::endl;

    return 0;
}
