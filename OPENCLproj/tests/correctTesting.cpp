#include <algorithm>
#include <cassert>
#include <iostream>
#include <vector>

#include "app.hpp"

namespace {

    FILE *freopenFile (const char *filename)
    {
        FILE *ftest = freopen (filename, "r", stdin);
        if (ftest == NULL) {
            perror ("File open error:");
            throw std::invalid_argument (filename);
        }
        return ftest;
    }

    //-----------------------------------------------------------------------------------------------------

    void closeFile (FILE *filestruct)
    {
        if (fclose (filestruct)) {
            perror ("File close error:\n");
            assert (false);
        }
    }

    //-----------------------------------------------------------------------------------------------------

    template <typename T>
    cl::vector<T> inputArr (const char *filename)
    {
        FILE *ftest = freopenFile (filename);

        size_t size;
        std::cin >> size;

        cl::vector<T> test (size);

        std::copy_n (std::istream_iterator<T> (std::cin), size, test.begin ());
        assert (std::cin.good ());

        closeFile (ftest);

        return test;
    }

    //-----------------------------------------------------------------------------------------------------

    template <typename T>
    int testSort (const char *filenameTests)
    {
        cl::vector<T> test = inputArr<T> (filenameTests);
        cl::vector<T> right (test.size ());
        std::copy (test.begin (), test.end (), right.begin ());

        OpenCLApp::BitonicSort<T> sort;
        sort.GPUBitonicSort (test);

        std::sort (right.begin (), right.end ());
        if (test == right)
            return EXIT_SUCCESS;
        return EXIT_FAILURE;
    }
}  // namespace

//=====================================================================================================

#ifndef TEST_TYPE
#define TEST_TYPE int
#endif

int main (int argc, char **argv)
{
    if (argc != 2)
        return EXIT_FAILURE;
    return testSort<TEST_TYPE> (argv[1]);    // TYPE from cmake 
}