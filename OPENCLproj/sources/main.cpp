#include <algorithm>
#include <chrono>

#include "app.hpp"

int main ()
{
    size_t size;
    std::cin >> size;

    using type = int;

    cl::vector<type> test (size);

    std::copy_n (std::istream_iterator<type> (std::cin), size, test.begin ());

    auto start = std::chrono::steady_clock::now ();

    OpenCLApp::BitonicSort sort (test);
    // std::sort (test.begin(), test.end());

    auto end = std::chrono::steady_clock::now ();

    std::chrono::duration<double> elapsed_seconds = end - start;

    std::copy (test.begin (), test.end (), std::ostream_iterator<type> (std::cout, " "));
    std::cout << std::endl;

#ifdef TIMER
    std::cout << "elapsed time: " << elapsed_seconds.count () << "s" << std::endl;
#endif

    return 0;
}
