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
    matrix::Matrix<T> inputMatrix (const char *filename)
    {
        FILE *ftest = freopenFile (filename);

        size_t size1, size2;
        std::cin >> size1 >> size2;
        assert (std::cin.good ());

        matrix::Matrix<T> matr (size1, size2);

        std::cin >> matr;
        assert (std::cin.good ());

        closeFile (ftest);

        return matr;
    }

    //-----------------------------------------------------------------------------------------------------

    template <typename T>
    void testDet (const char *filenameTests, const char *filenameAnsws)
    {
        matrix::Matrix<T> testMatrix = inputMatrix<T> (filenameTests);

        FILE *ftest = freopenFile (filenameAnsws);

        T rightAns;
        std::cin >> rightAns;

        closeFile (ftest);

        ASSERT_EQ (std::abs (rightAns - testMatrix.det ()) < ACCURACY, true);
    }

    //-----------------------------------------------------------------------------------------------------

    template <typename T>
    void testmul (const char *filenameTest1, const char *filenameTest2, const char *filenameAnsws)
    {
        matrix::Matrix<T> first = inputMatrix<T> (filenameTest1);
        matrix::Matrix<T> second = inputMatrix<T> (filenameTest2);

        matrix::Matrix<T> answer = inputMatrix<T> (filenameAnsws);

        ASSERT_EQ (first * second, answer);
    }

    //-----------------------------------------------------------------------------------------------------

    template <typename T>
    void testtrans (const char *filenameTests, const char *filenameAnsws)
    {
        matrix::Matrix<T> matr = inputMatrix<T> (filenameTests);
        matrix::Matrix<T> answer = inputMatrix<T> (filenameAnsws);

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
    testmul<int> ("../tests/testmul1first.txt", "../tests/testmul1second.txt", "../tests/answers/ansmul1.txt");
}

//-----------------------------------------------------------------------------------------------------

TEST (mul, test2)
{
    testmul<int> ("../tests/testmul2first.txt", "../tests/testmul2second.txt", "../tests/answers/ansmul2.txt");
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

//-----------------------------------------------------------------------------------------------------

TEST (copyCtor, test1)
{
    matrix::Matrix<int> testMatrix = inputMatrix<int> ("../tests/testmul1second.txt");
    matrix::Matrix<int> copy {testMatrix};

    ASSERT_EQ (copy, testMatrix);
}

//-----------------------------------------------------------------------------------------------------

TEST (copyCtor, test2)
{
    matrix::Matrix<double> testMatrix = inputMatrix<double> ("../tests/test7.txt");
    matrix::Matrix<double> copy {testMatrix};

    ASSERT_EQ (copy, testMatrix);
}

//-----------------------------------------------------------------------------------------------------

TEST (copyCtor, test3)
{
    matrix::Matrix<std::complex<int>> testMatrix = inputMatrix<std::complex<int>> ("../tests/complexMatrixTest2.txt");
    matrix::Matrix<std::complex<int>> copy {testMatrix};

    ASSERT_EQ (copy, testMatrix);
}

//-----------------------------------------------------------------------------------------------------

TEST (moveCtor, test1)
{
    matrix::Matrix<int> testMatrix = inputMatrix<int> ("../tests/testmul1second.txt");
    matrix::Matrix<int> tmpMatrix {testMatrix};
    matrix::Matrix<int> move {std::move(tmpMatrix)};

    ASSERT_EQ (move, testMatrix);
}

//-----------------------------------------------------------------------------------------------------

TEST (moveCtor, test2)
{
    matrix::Matrix<double> testMatrix = inputMatrix<double> ("../tests/test7.txt");
    matrix::Matrix<double> tmpMatrix {testMatrix};
    matrix::Matrix<double> move {std::move(tmpMatrix)};

    ASSERT_EQ (move, testMatrix);
}

//-----------------------------------------------------------------------------------------------------

TEST (moveCtor, test3)
{
    matrix::Matrix<std::complex<int>> testMatrix = inputMatrix<std::complex<int>> ("../tests/complexMatrixTest2.txt");
    matrix::Matrix<std::complex<int>> tmpMatrix {testMatrix};
    matrix::Matrix<std::complex<int>> move {std::move(tmpMatrix)};

    ASSERT_EQ (move, testMatrix);
}

//-----------------------------------------------------------------------------------------------------

TEST (copyAssignmentOperator, test1)
{
    matrix::Matrix<int> testMatrix = inputMatrix<int> ("../tests/testmul1second.txt");
    matrix::Matrix<int> copy;
    copy = testMatrix;

    ASSERT_EQ (copy, testMatrix);
}

//-----------------------------------------------------------------------------------------------------

TEST (copyAssignmentOperator, test2)
{
    matrix::Matrix<double> testMatrix = inputMatrix<double> ("../tests/test7.txt");
    matrix::Matrix<double> copy;
    copy = testMatrix;

    ASSERT_EQ (copy, testMatrix);
}

//-----------------------------------------------------------------------------------------------------

TEST (copyAssignmentOperator, test3)
{
    matrix::Matrix<std::complex<int>> testMatrix = inputMatrix<std::complex<int>> ("../tests/complexMatrixTest2.txt");
    matrix::Matrix<std::complex<int>> copy;
    copy = testMatrix;

    ASSERT_EQ (copy, testMatrix);
}

//-----------------------------------------------------------------------------------------------------

TEST (moveAssignmentOperator, test1)
{
    matrix::Matrix<int> testMatrix = inputMatrix<int> ("../tests/testmul1second.txt");
    matrix::Matrix<int> tmpMatrix {testMatrix};
    matrix::Matrix<int> move;
    move = std::move(tmpMatrix);

    ASSERT_EQ (move, testMatrix);
}

//-----------------------------------------------------------------------------------------------------

TEST (moveAssignmentOperator, test2)
{
    matrix::Matrix<double> testMatrix = inputMatrix<double> ("../tests/test7.txt");
    matrix::Matrix<double> tmpMatrix {testMatrix};
    matrix::Matrix<double> move;
    move = std::move(tmpMatrix);

    ASSERT_EQ (move, testMatrix);
}

//-----------------------------------------------------------------------------------------------------

TEST (moveAssignmentOperator, test3)
{
    matrix::Matrix<std::complex<int>> testMatrix = inputMatrix<std::complex<int>> ("../tests/complexMatrixTest2.txt");
    matrix::Matrix<std::complex<int>> tmpMatrix {testMatrix};
    matrix::Matrix<std::complex<int>> move;
    move = std::move(tmpMatrix);

    ASSERT_EQ (move, testMatrix);
}

//=====================================================================================================

int main (int argc, char **argv)
{
    testing::InitGoogleTest (&argc, argv);

    return RUN_ALL_TESTS ();
}