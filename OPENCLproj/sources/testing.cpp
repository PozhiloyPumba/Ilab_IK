#include <gtest/gtest.h>

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
            assert (false);
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
    void testSort (const char *filenameTests, const char *filenameAnsws)
    {
        cl::vector<T> test = inputArr<T> (filenameTests);
        cl::vector<T> right = inputArr<T> (filenameAnsws);

        OpenCLApp::BitonicSort sort (test);

        ASSERT_EQ (test, right);
    }
}  // namespace

//=====================================================================================================

TEST (intTest, test1)
{
    testSort<int> ("../tests/test1.dat", "../tests/ans/test1.ans");
}

//-----------------------------------------------------------------------------------------------------

TEST (intTest, test2)
{
    testSort<int> ("../tests/test2.dat", "../tests/ans/test2.ans");
}

//-----------------------------------------------------------------------------------------------------

TEST (intTest, test3)
{
    testSort<int> ("../tests/test3.dat", "../tests/ans/test3.ans");
}

//-----------------------------------------------------------------------------------------------------

TEST (intTest, test4)
{
    testSort<int> ("../tests/test4.dat", "../tests/ans/test4.ans");
}

//-----------------------------------------------------------------------------------------------------

TEST (floatTest, test1)
{
    testSort<float> ("../tests/ftest1.dat", "../tests/ans/ftest1.ans");
}

//-----------------------------------------------------------------------------------------------------

TEST (shortTest, test1)
{
    testSort<short> ("../tests/shtest1.dat", "../tests/ans/shtest1.ans");
}

//-----------------------------------------------------------------------------------------------------

TEST (doubleTest, test1)
{
    testSort<double> ("../tests/dtest1.dat", "../tests/ans/dtest1.ans");
}

//-----------------------------------------------------------------------------------------------------

TEST (unsignedTest, test1)
{
    testSort<unsigned int> ("../tests/utest1.dat", "../tests/ans/utest1.ans");
}

//=====================================================================================================

int main (int argc, char **argv)
{
    testing::InitGoogleTest (&argc, argv);

    return RUN_ALL_TESTS ();
}