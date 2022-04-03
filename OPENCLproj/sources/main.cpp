#include <algorithm>
#include <chrono>

#include "app.hpp"

int main ()
{
    size_t size;
    std::cin >> size;

    using type = int;

    cl::vector<type> vec (size);

    std::copy_n (std::istream_iterator<type> (std::cin), size, vec.begin ());
    OpenCLApp::BitonicSort<type> sort ("../sources/kernels/simple.cl");

#ifdef TIMER
    auto start = std::chrono::steady_clock::now ();
#endif

    sort.GPUBitonicSort (vec);

#ifdef TIMER
    auto end = std::chrono::steady_clock::now ();
#endif

    std::copy (vec.begin (), vec.end (), std::ostream_iterator<type> (std::cout, " "));
    std::cout << std::endl;

#ifdef TIMER
    std::chrono::duration<double> elapsed_seconds = end - start;
    std::cout << "elapsed time: " << elapsed_seconds.count () << "s" << std::endl;
#endif

    return 0;
}
