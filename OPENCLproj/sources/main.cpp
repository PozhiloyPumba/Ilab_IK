#include "app.hpp"

int main ()
{
    OpenCLApp::App app ("../sources/bitonicSort.cl");

    cl::vector<int> test(10, 3);

    std::cout << "before" << std::endl;

    for (auto el: test){
        std::cout << el << " ";
    }

    app.BitonicSort(test.data(), test.size());

    std::cout << "after" << std::endl;
    for (auto el: test)
        std::cout << el << " ";

    return 0;
}
