#ifndef __MATRIX_HPP__
#define __MATRIX_HPP__

#include <complex>
#include <cstring>
#include <ctime>
#include <iostream>

namespace matrix {
    const double EPSILON = 10E-15;

    class MyException {
    private:
        std::string m_error;

    public:
        MyException (std::string error)
            : m_error (error)
        {
        }

        const char *whoami () { return m_error.c_str (); }
    };

    template <typename T = double>
    class Matrix final {
    private:
        int nRows_, nCols_;
        T *arr_;

        //-----------------------------------------------------------------------------------------------------

        int maxSubColElem (T **rows, int *cols, int nCol)
        {
            T *max = rows[cols[nCol]];
            int maxRow = nCol;

            for (int i = nCol + 1; i < nRows_; ++i) {
                if (std::abs (max[cols[nCol]]) < std::abs (rows[i][cols[nCol]])) {
                    max = rows[i];
                    maxRow = i;
                }
            }

            return maxRow;
        }

        //-----------------------------------------------------------------------------------------------------

        int maxSubRowElem (T **rows, int *cols, int nRow)
        {
            int maxCol = nRow;

            for (int i = nRow + 1; i < nCols_; ++i) {
                if (std::abs (rows[nRow][cols[maxCol]]) < std::abs (rows[nRow][cols[i]])) {
                    maxCol = i;
                }
            }

            return maxCol;
        }

        //-----------------------------------------------------------------------------------------------------

        int fakeSwapWithBiggest (T **fakeRows, int *fakeCols, int index, bool param)
        {
            int sign = 1;
            int withMax;

            if (param)
                withMax = maxSubColElem (fakeRows, fakeCols, index);
            else
                withMax = maxSubRowElem (fakeRows, fakeCols, index);

            if (withMax != index) {
                sign = -1;
                if (param)
                    std::swap (fakeRows[index], fakeRows[withMax]);
                else
                    std::swap (fakeCols[index], fakeCols[withMax]);
            }
            return sign;
        }

        //-----------------------------------------------------------------------------------------------------

        T countDet (T **rows, int *cols, int sign)
        {
            T det = (sign == 1) ? rows[0][cols[0]] : -rows[0][cols[0]];
            for (int i = 1; i < nCols_; ++i)
                det *= rows[i][cols[i]];
            return det;
        }

        //-----------------------------------------------------------------------------------------------------

        T det (std::false_type) const
        {
            Matrix<T> support = *this;
            T det = support.fakeGauss ();

            return det;
        }

        //-----------------------------------------------------------------------------------------------------

        T det (std::true_type) const
        {
            Matrix<double> support = *this;
            double det = support.fakeGauss ();

            return round (det);
        }

        //-----------------------------------------------------------------------------------------------------

        int *createFakeCols ()
        {
            int *fakeCols = new int[nCols_];  // for fake swap cols
            for (int i = 0; i < nCols_; ++i)
                fakeCols[i] = i;

            return fakeCols;
        }

        //-----------------------------------------------------------------------------------------------------

        T **createFakeRows ()
        {
            T **fakeRows = new T *[nRows_];  // for fake swap rows
            for (int i = 0; i < nRows_; ++i)
                fakeRows[i] = arr_ + nCols_ * i;

            return fakeRows;
        }

        //-----------------------------------------------------------------------------------------------------

        struct Proxy final {
            T *row_;
            int proxynCols_;

            Proxy (T *row, int nCols)
                : row_ (row),
                  proxynCols_ (nCols)
            {
            }

            //-----------------------------------------------------------------------------------------------------

            const T &operator[] (int col) const
            {
                if (col >= proxynCols_ || col < 0)
                    throw MyException{"you tried to get data from nonexistent column"};

                return row_[col];
            }

            //-----------------------------------------------------------------------------------------------------

            T &operator[] (int col)
            {
                if (col >= proxynCols_ || col < 0)
                    throw MyException{"you tried to get data from nonexistent column"};

                return row_[col];
            }
        };

        //-----------------------------------------------------------------------------------------------------

    public:
        Matrix (const int nRows = 0, const int nCols = 0)  // ctor
            : nRows_ (nRows),
              nCols_ (nCols),
              arr_ (new T[nRows * nCols])
        {
        }

        //-----------------------------------------------------------------------------------------------------

        Matrix (const int nRows, const int nCols, T val)  // ctor
            : nRows_ (nRows),
              nCols_ (nCols),
              arr_ (new T[nRows * nCols])
        {
            std::fill_n (arr_, nRows_ * nCols_, val);
        }

        //-----------------------------------------------------------------------------------------------------

        Matrix (const Matrix<T> &other)
            : nRows_ (other.nRows_),
              nCols_ (other.nCols_),
              arr_ (new T[other.nRows_ * other.nCols_])  // copy ctor
        {
            T *data = other.arr_;
            for (int i = 0, size = nRows_ * nCols_; i < size; ++i)
                arr_[i] = data[i];
        }

        //-----------------------------------------------------------------------------------------------------

        template <typename otherT>
        Matrix (const Matrix<otherT> &other)
            : nRows_ (other.getNRows ()),
              nCols_ (other.getNCols ()),
              arr_ (new T[nRows_ * nCols_])  // copy ctor from other type
        {
            for (int i = 0; i < nRows_; ++i)
                for (int j = 0; j < nCols_; ++j)
                    arr_[i * nCols_ + j] = other[i][j];
        }

        //-----------------------------------------------------------------------------------------------------

        Matrix (Matrix<T> &&other) noexcept
            : nRows_ (other.nRows_),
              nCols_ (other.nCols_),
              arr_ (other.arr_)  // move ctor
        {
            other.arr_ = 0;
            other.nCols_ = 0;
            other.nRows_ = 0;
        }

        //-----------------------------------------------------------------------------------------------------

        Matrix &operator= (const Matrix<T> &other)  // assignment operator
        {
            if (this == &other)
                return *this;

            delete[] arr_;

            arr_ = new T[other.nRows_ * other.nCols_];
            nRows_ = other.nRows_;
            nCols_ = other.nCols_;

            T *data = other.arr_;
            for (int i = 0, size = nRows_ * nCols_; i < size; ++i)
                arr_[i] = data[i];

            return *this;
        }

        //-----------------------------------------------------------------------------------------------------

        Matrix &operator= (Matrix<T> &&other) noexcept  // move operator
        {
            if (this == &other)
                return *this;

            std::swap (other.arr_, arr_);
            std::swap (nRows_, other.nRows_);
            std::swap (nCols_, other.nCols_);

            return *this;
        }

        //-----------------------------------------------------------------------------------------------------

        ~Matrix ()  // dtor
        {
            delete[] arr_;
        }

        //-----------------------------------------------------------------------------------------------------

        int getNRows () const
        {
            return nRows_;
        }

        //-----------------------------------------------------------------------------------------------------

        int getNCols () const
        {
            return nCols_;
        }

        //-----------------------------------------------------------------------------------------------------

        static Matrix<int> randomIntMatrix (int size, int det)
        {
            Matrix<int> rndMtrx (size, size);

            std::srand (time (nullptr));
            for (int i = 0; i < size - 1; ++i) {
                rndMtrx[i][i] = 1;

                for (int j = i + 1; j < size; ++j)
                    rndMtrx[i][j] = rand () % (2 * det) - det;
            }
            rndMtrx[size - 1][size - 1] = det;

            int randCoef;
            for (int i = 1; i < size; ++i) {
                while (!(randCoef = rand () % 11 - 5))
                    ;

                for (int j = 0; j < size; ++j)
                    rndMtrx[i][j] += randCoef * rndMtrx[0][j];
            }

            for (int i = 0; i < size - 1; ++i) {
                while (!(randCoef = rand () % 11 - 5))
                    ;

                for (int j = 0; j < size; ++j)
                    rndMtrx[j][i] += randCoef * rndMtrx[j][size - 1];
            }

            return rndMtrx;
        }

        //=====================================================================================================

        Proxy operator[] (const int row)
        {
            if (row >= nRows_ || row < 0)
                throw MyException{"you tried to get data from nonexistent row"};

            return Proxy (arr_ + nCols_ * row, nCols_);
        }

        //-----------------------------------------------------------------------------------------------------

        const Proxy operator[] (const int row) const
        {
            if (row >= nRows_ || row < 0)
                throw MyException{"you tried to get data from nonexistent row"};

            return Proxy (arr_ + nCols_ * row, nCols_);
        }

        //=====================================================================================================

        void dump () const
        {
            for (int i = 0; i < nRows_; ++i) {
                for (int j = 0; j < nCols_; ++j)
                    std::cout << arr_[nCols_ * i + j] << " ";

                std::cout << std::endl;
            }
        }

        //-----------------------------------------------------------------------------------------------------

        void input (std::istream &in)
        {
            for (int i = 0; i < nRows_; ++i)
                for (int j = 0; j < nCols_; ++j)
                    in >> (*this)[i][j];
        }

        //-----------------------------------------------------------------------------------------------------

        T det () const
        {
            if (nRows_ != nCols_)
                throw MyException{"matrix is not square! I don't know what are you want from me"};

            return det (std::is_integral<T> ());
        }

        //-----------------------------------------------------------------------------------------------------

        T fakeGauss ()
        {
            int sign = 1;
            T **fakeRows = createFakeRows ();
            int *fakeCols = createFakeCols ();

            for (int i = 0; i < nRows_; ++i) {
                sign *= fakeSwapWithBiggest (fakeRows, fakeCols, i, true);
                sign *= fakeSwapWithBiggest (fakeRows, fakeCols, i, false);

                if (std::abs (fakeRows[i][fakeCols[i]]) <= EPSILON)
                    return T{};

                T max = fakeRows[i][fakeCols[i]];

                for (int j = i + 1; j < nRows_; ++j) {
                    T del = fakeRows[j][fakeCols[i]] / max;

                    for (int k = i + 1; k < nCols_; ++k)  // we can not nullify the column that we will not pay attention to and thn k = i +1 (not i)
                        fakeRows[j][fakeCols[k]] -= del * fakeRows[i][fakeCols[k]];
                }
            }

            T determinant = countDet (fakeRows, fakeCols, sign);

            delete[] fakeRows;
            delete[] fakeCols;

            return determinant;
        }
    };

    //=====================================================================================================

    template <typename T = double>
    std::ostream &operator<< (std::ostream &out, Matrix<T> &matrix)
    {
        matrix.dump ();
        return out;
    }

    //-----------------------------------------------------------------------------------------------------

    template <typename T = double>
    std::istream &operator>> (std::istream &in, Matrix<T> &matrix)
    {
        matrix.input (in);
        return in;
    }

    //-----------------------------------------------------------------------------------------------------

    template <typename T = double>
    Matrix<T> operator+ (const Matrix<T> &first, const Matrix<T> &second)  // overload for fun
    {
        if (first.getNCols () != second.getNCols () || first.getNRows () != second.getNRows ()) {
            std::cout << "I can't add these matrix because their size isn't equal" << std::endl;
            return Matrix<T>{};
        }

        int nRow = first.getNRows ();
        int nCol = first.getNCols ();

        Matrix<T> result (nRow, nCol);

        for (int i = 0; i < nRow; ++i)
            for (int j = 0; j < nCol; ++j)
                result[i][j] = first[i][j] + second[i][j];

        return result;
    }

}  // namespace matrix

#endif