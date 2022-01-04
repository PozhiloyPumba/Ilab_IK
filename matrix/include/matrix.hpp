#ifndef MATRIX_HPP_
#define MATRIX_HPP_

#include <complex>
#include <cstring>
#include <ctime>
#include <iostream>

namespace matrix {
    const double EPSILON = 10E-15;

    template <typename T = double>
    class Matrix final {
    private:
        int nRows_, nCols_;
        T *arr_;

        //-----------------------------------------------------------------------------------------------------

        void nullify () noexcept
        {
            arr_ = nullptr;
            nCols_ = 0;
            nRows_ = 0;
        }

        //-----------------------------------------------------------------------------------------------------

        int maxSubColElem (T **rows, const int *cols, const int nCol) const noexcept
        {
            int maxRow = nCol;

            for (int i = nCol + 1; i < nRows_; ++i)
                if (std::abs (rows[maxRow][cols[nCol]]) < std::abs (rows[i][cols[nCol]]))
                    maxRow = i;

            return maxRow;
        }

        //-----------------------------------------------------------------------------------------------------

        int maxSubRowElem (T **rows, const int *cols, const int nRow) const noexcept
        {
            int maxCol = nRow;

            for (int i = nRow + 1; i < nCols_; ++i)
                if (std::abs (rows[nRow][cols[maxCol]]) < std::abs (rows[nRow][cols[i]]))
                    maxCol = i;

            return maxCol;
        }

        //-----------------------------------------------------------------------------------------------------

        int fakeSwapWithBiggest (T **fakeRows, int *fakeCols, const int index, const bool param) const noexcept
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

        T countDet (T **rows, const int *cols, const int sign) const noexcept
        {
            T det = rows[0][cols[0]];
            for (int i = 1; i < nCols_; ++i)
                det *= rows[i][cols[i]];
            return (sign == 1) ? det : -det;
        }

        //-----------------------------------------------------------------------------------------------------

        int *createFakeCols () const
        {
            int *fakeCols = new int[nCols_];  // for fake swap cols
            for (int i = 0; i < nCols_; ++i)
                fakeCols[i] = i;

            return fakeCols;
        }

        //-----------------------------------------------------------------------------------------------------

        T **createFakeRows () const
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

            const T &operator[] (const int col) const
            {
                if (col >= proxynCols_ || col < 0)
                    throw std::length_error{"you tried to get data from nonexistent column"};

                return row_[col];
            }

            //-----------------------------------------------------------------------------------------------------

            T &operator[] (const int col)
            {
                if (col >= proxynCols_ || col < 0)
                    throw std::length_error{"you tried to get data from nonexistent column"};

                return row_[col];
            }
        };

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
            double det = support.det ();

            return round (det);
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

    public:
        Matrix (const int nRows = 0, const int nCols = 0)  // ctor
        try : nRows_ (nRows),
              nCols_ (nCols),
              arr_ (new T[nRows * nCols]) {
        }
        catch (...) {
            nullify ();
            throw;
        }

        //-----------------------------------------------------------------------------------------------------

        Matrix (const int nRows, const int nCols, T val)  // ctor
        try : nRows_ (nRows),
              nCols_ (nCols),
              arr_ (new T[nRows * nCols]) {
            std::fill_n (arr_, nRows_ * nCols_, val);
        }
        catch (...) {
            nullify ();
            throw;
        }

        //-----------------------------------------------------------------------------------------------------

        Matrix (const Matrix<T> &other)
        try : nRows_ (other.nRows_),
              nCols_ (other.nCols_),
              arr_ (new T[other.nRows_ * other.nCols_])  // copy ctor
        {
            T *data = other.arr_;
            for (int i = 0, size = nRows_ * nCols_; i < size; ++i)
                arr_[i] = data[i];
        }
        catch (...) {
            nullify ();
            throw;
        }

        //-----------------------------------------------------------------------------------------------------

        template <typename otherT>
        Matrix (const Matrix<otherT> &other)
        try : nRows_ (other.getNRows ()),
              nCols_ (other.getNCols ()),
              arr_ (new T[nRows_ * nCols_])  // copy ctor from other type
        {
            for (int i = 0; i < nRows_; ++i)
                for (int j = 0; j < nCols_; ++j)
                    arr_[i * nCols_ + j] = other[i][j];
        }
        catch (...) {
            nullify ();
            throw;
        }

        //-----------------------------------------------------------------------------------------------------

        Matrix (Matrix<T> &&other) noexcept
            : nRows_ (other.nRows_),
              nCols_ (other.nCols_),
              arr_ (other.arr_)  // move ctor
        {
            other.nullify ();
        }

        //-----------------------------------------------------------------------------------------------------

        Matrix &operator= (const Matrix<T> &other)  // assignment operator
        {
            if (this == &other)
                return *this;

            delete[] arr_;

            try {
                arr_ = new T[other.nRows_ * other.nCols_];
            }
            catch (...) {
                nullify ();
                throw;
            }
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

        int getNRows () const noexcept
        {
            return nRows_;
        }

        //-----------------------------------------------------------------------------------------------------

        int getNCols () const noexcept
        {
            return nCols_;
        }

        //-----------------------------------------------------------------------------------------------------

        static Matrix<int> randomIntMatrix (const int size, const int det)
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
                while (!(randCoef = rand () % 11 - 5))  // because I want randCoef != 0
                    ;

                for (int j = 0; j < size; ++j)
                    rndMtrx[i][j] += randCoef * rndMtrx[0][j];
            }

            for (int i = 0; i < size - 1; ++i) {
                while (!(randCoef = rand () % 11 - 5))  // because I want randCoef != 0
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
                throw std::length_error{"you tried to get data from nonexistent row"};

            return Proxy (arr_ + nCols_ * row, nCols_);
        }

        //-----------------------------------------------------------------------------------------------------

        const Proxy operator[] (const int row) const
        {
            if (row >= nRows_ || row < 0)
                throw std::length_error{"you tried to get data from nonexistent row"};
            
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
                throw std::logic_error{"matrix is not square! I don't know what are you want from me"};

            return det (std::is_integral<T> ());
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
        if (first.getNCols () != second.getNCols () || first.getNRows () != second.getNRows ())
            throw std::logic_error{"I can't add these matrix because their size isn't equal"};

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