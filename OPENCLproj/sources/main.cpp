#include <algorithm>

#include "app.hpp"

int main ()
{
    size_t size;
    std::cin >> size;
    
    using type = int;

    cl::vector<type> vec (size);

    std::copy_n (std::istream_iterator<type> (std::cin), size, vec.begin ());
    OpenCLApp::BitonicSort<type> sort ("../sources/kernels/simple.cl");

    sort.GPUBitonicSort (vec);

    std::copy (vec.begin (), vec.end (), std::ostream_iterator<type> (std::cout, " "));

    return 0;
}