#include "app.hpp"
#include <algorithm>
#include <chrono>

int main ()
{
    OpenCLApp::App<int> app ("../sources/bitonicSort.cl");
    
    size_t size;
    std::cin >> size;

    cl::vector<int> test;

    int tmp;
    for (size_t i = 0; i < size; ++i) {
        std::cin >> tmp;
        test.push_back (tmp);
    }

    auto start = std::chrono::steady_clock::now ();

    app.GPUBitonicSort (test);
    // std::sort (test.begin(), test.end());

    auto end = std::chrono::steady_clock::now ();

    std::chrono::duration<double> elapsed_seconds = end - start;
    std::cout << "elapsed time: " << elapsed_seconds.count () << "s\n";

    // std::cout << "sort " << std::endl;

    // for (auto el: test)
    //     std::cout << el << " ";

    // std::cout << std::endl;

    return 0;
}
