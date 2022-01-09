#include <gtest/gtest.h>

#include <cassert>

#include "matrix.hpp"

namespace {

    const double ACCURACY = 1E-4;

    //-----------------------------------------------------------------------------------------------------

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
    matrix::Matrix<T> inputMatrix ()
    {
        int size1, size2;
        std::cin >> size1 >> size2;
        assert (std::cin.good ());

        matrix::Matrix<T> matr (size1, size2);

        std::cin >> matr;
        assert (std::cin.good ());

        return matr;
    }

    //-----------------------------------------------------------------------------------------------------

    template <typename T>
    void testDet (const char *filenameTests, const char *filenameAnsws)
    {
        FILE *ftest = freopenFile (filenameTests);

        matrix::Matrix<T> testMatrix = inputMatrix<T> ();

        closeFile (ftest);

        ftest = freopenFile (filenameAnsws);

        T rightAns;
        std::cin >> rightAns;

        closeFile (ftest);

        ASSERT_EQ (std::abs (rightAns - testMatrix.det ()) < ACCURACY, true);
    }

    //-----------------------------------------------------------------------------------------------------
    
    template <typename T>
    void testmul (const char *filenameTests, const char *filenameAnsws)
    {
        FILE *ftest = freopenFile (filenameTests);

        matrix::Matrix<T> first = inputMatrix<T> ();
        matrix::Matrix<T> second = inputMatrix<T> ();

        closeFile (ftest);

        ftest = freopenFile (filenameAnsws);

        matrix::Matrix<T> answer = inputMatrix<T> ();

        closeFile (ftest);

        ASSERT_EQ (first * second, answer);
    }

    template <typename T>
    void testtrans (const char *filenameTests, const char *filenameAnsws)
    {
        FILE *ftest = freopenFile (filenameTests);

        matrix::Matrix<T> matr = inputMatrix<T> ();

        closeFile (ftest);

        ftest = freopenFile (filenameAnsws);

        matrix::Matrix<T> answer = inputMatrix<T> ();

        closeFile (ftest);

        ASSERT_EQ (~matr, answer);
    }
}  // namespace

//=====================================================================================================

TEST (detTest, test1)
{
    testDet<int> ("../tests/test1.txt", "../tests/answers/ans1.txt");
}

//-----------------------------------------------------------------------------------------------------

TEST (detTest, test2)
{
    testDet<int> ("../tests/test2.txt", "../tests/answers/ans2.txt");
}

//-----------------------------------------------------------------------------------------------------

TEST (detTest, test3)
{
    testDet<int> ("../tests/test3.txt", "../tests/answers/ans3.txt");
}

//-----------------------------------------------------------------------------------------------------

TEST (detTest, test4)
{
    testDet<int> ("../tests/test4.txt", "../tests/answers/ans4.txt");
}

//-----------------------------------------------------------------------------------------------------

TEST (detTest, test5)
{
    testDet<int> ("../tests/test5.txt", "../tests/answers/ans5.txt");
}

//-----------------------------------------------------------------------------------------------------

TEST (detTest, test6)
{
    testDet<int> ("../tests/test6.txt", "../tests/answers/ans6.txt");
}

//-----------------------------------------------------------------------------------------------------

TEST (detTest, test7)
{
    testDet<int> ("../tests/test7.txt", "../tests/answers/ans7.txt");
}

//-----------------------------------------------------------------------------------------------------

TEST (detCmplxTest, test1)
{
    testDet<std::complex<double>> ("../tests/complexMatrixTest1.txt", "../tests/answers/ansComplex1.txt");
}

//-----------------------------------------------------------------------------------------------------

TEST (detCmplxTest, test2)
{
    testDet<std::complex<double>> ("../tests/complexMatrixTest2.txt", "../tests/answers/ansComplex2.txt");
}

//-----------------------------------------------------------------------------------------------------

TEST (mul, test1)
{
    testmul<int> ("../tests/testmul1.txt", "../tests/answers/ansmul1.txt");
}

//-----------------------------------------------------------------------------------------------------

TEST (mul, test2)
{
    testmul<int> ("../tests/testmul2.txt", "../tests/answers/ansmul2.txt");
}

//-----------------------------------------------------------------------------------------------------

TEST (transpose, test1)
{
    testtrans<int> ("../tests/testtrans1.txt", "../tests/answers/anstrans1.txt");
}

//-----------------------------------------------------------------------------------------------------

TEST (transpose, test2)
{
    testtrans<int> ("../tests/testtrans2.txt", "../tests/answers/anstrans2.txt");
}
//TODO: unit-tests for big-5
//=====================================================================================================

int main (int argc, char **argv)
{
    testing::InitGoogleTest (&argc, argv);

    return RUN_ALL_TESTS ();
}