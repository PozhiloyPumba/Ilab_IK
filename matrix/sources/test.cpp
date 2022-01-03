#include <gtest/gtest.h>

#include <cassert>

#include "matrix.hpp"

namespace {

    const double ACCURACY = 1E-5;
    matrix::Matrix<double> inputMatrix (const char *filenameTests)
    {
        FILE *ftest;

        ftest = freopen (filenameTests, "r", stdin);
        if (ftest == NULL) {
            perror ("File open error:");
            assert (false);
        }

        int size;
        std::cin >> size;
        assert (std::cin.good ());

        matrix::Matrix<double> matr (size, size);

        std::cin >> matr;
        assert (std::cin.good ());

        if (fclose (ftest)) {
            perror ("File close error:\n");
            assert (false);
        }

        return matr;
    }

    //-----------------------------------------------------------------------------------------------------

    void testDet (const char *filenameTests, const char *filenameAnsws)
    {
        matrix::Matrix<double> testMatrix = inputMatrix (filenameTests);

        FILE *ftest = freopen (filenameAnsws, "r", stdin);
        if (ftest == NULL) {
            perror ("File open error:\n");
            return;
        }

        double rightAns;
        std::cin >> rightAns;

        if (fclose (ftest)) {
            perror ("File close error:\n");
            return;
        }

        ASSERT_EQ (rightAns - testMatrix.det () < ACCURACY, true);
    }

    //-----------------------------------------------------------------------------------------------------

    matrix::Matrix<std::complex<double>> inputCmplxMatrix (const char *filenameTests)
    {
        FILE *ftest;

        ftest = freopen (filenameTests, "r", stdin);
        if (ftest == NULL) {
            perror ("File open error:");
            assert (false);
        }

        int size;
        std::cin >> size;
        assert (std::cin.good ());

        matrix::Matrix<std::complex<double>> matr (size, size);

        std::cin >> matr;
        assert (std::cin.good ());

        if (fclose (ftest)) {
            perror ("File close error:\n");
            assert (false);
        }

        return matr;
    }

    //-----------------------------------------------------------------------------------------------------

    void testCmplxDet (const char *filenameTests, const char *filenameAnsws)
    {
        matrix::Matrix<std::complex<double>> testMatrix = inputCmplxMatrix (filenameTests);

        FILE *ftest = freopen (filenameAnsws, "r", stdin);
        if (ftest == NULL) {
            perror ("File open error:\n");
            return;
        }

        std::complex<double> rightAns;
        std::cin >> rightAns;

        if (fclose (ftest)) {
            perror ("File close error:\n");
            return;
        }

        ASSERT_EQ (std::abs (rightAns - testMatrix.det ()) < ACCURACY, true);
    }

}  // namespace

//=====================================================================================================

TEST (detTest, test1)
{
    testDet ("../tests/test1.txt", "../tests/answers/ans1.txt");
}

//-----------------------------------------------------------------------------------------------------

TEST (detTest, test2)
{
    testDet ("../tests/test2.txt", "../tests/answers/ans2.txt");
}

//-----------------------------------------------------------------------------------------------------

TEST (detTest, test3)
{
    testDet ("../tests/test3.txt", "../tests/answers/ans3.txt");
}

//-----------------------------------------------------------------------------------------------------

TEST (detTest, test4)
{
    testDet ("../tests/test4.txt", "../tests/answers/ans4.txt");
}

//-----------------------------------------------------------------------------------------------------

TEST (detTest, test5)
{
    testDet ("../tests/test5.txt", "../tests/answers/ans5.txt");
}

//-----------------------------------------------------------------------------------------------------

TEST (detTest, test6)
{
    testDet ("../tests/test6.txt", "../tests/answers/ans6.txt");
}

//-----------------------------------------------------------------------------------------------------

TEST (detTest, test7)
{
    testDet ("../tests/test7.txt", "../tests/answers/ans7.txt");
}

//-----------------------------------------------------------------------------------------------------

TEST (detCmplxTest, test8)
{
    testCmplxDet ("../tests/complexMatrixTest.txt", "../tests/answers/ansComplex.txt");
}

//=====================================================================================================

int main (int argc, char **argv)
{
    testing::InitGoogleTest (&argc, argv);

    return RUN_ALL_TESTS ();
}