#ifndef MATRIX_HPP_
#define MATRIX_HPP_

#include <chrono>
#include <complex>
#include <cstring>
#include <iostream>
#include <memory>
#include <random>
#include <algorithm>
#include <iterator>

namespace matrix {
    const double EPSILON = 10E-15;
    const int maxCoefConst = 5;

    template <typename T = double>
    class Matrix final {
    private:
        size_t nRows_, nCols_;
        T *arr_;

        //-----------------------------------------------------------------------------------------------------

        void nullify () noexcept
        {
            arr_ = nullptr;
            nCols_ = 0;
            nRows_ = 0;
        }

        //-----------------------------------------------------------------------------------------------------

        int fakeSwapWithBiggest (std::vector<T *> &fakeRows, std::vector<int> &fakeCols, const size_t index, const bool param) const noexcept
        {
            int sign = 1;
            size_t withMax = index;

            if (param) {
                int realCol = fakeCols[index];
                for (size_t i = index + 1; i < nRows_; ++i)
                    if (std::abs (fakeRows[withMax][realCol]) < std::abs (fakeRows[i][realCol]))
                        withMax = i;
            }
            else {
                T *realRow = fakeRows[index];
                for (size_t i = index + 1; i < nCols_; ++i)
                    if (std::abs (realRow[fakeCols[withMax]]) < std::abs (realRow[fakeCols[i]]))
                        withMax = i;
            }

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

        struct Proxy final {
            T *row_;
            size_t proxynCols_;

            Proxy (T *row, size_t nCols)
                : row_ (row),
                  proxynCols_ (nCols)
            {
            }

            //-----------------------------------------------------------------------------------------------------

            const T &operator[] (const size_t col) const
            {
                if (col >= proxynCols_)
                    throw std::length_error{"you tried to get data from nonexistent column"};

                return row_[col];
            }

            //-----------------------------------------------------------------------------------------------------

            T &operator[] (const size_t col)
            {
                if (col >= proxynCols_)
                    throw std::length_error{"you tried to get data from nonexistent column"};

                return row_[col];
            }
        };

        //-----------------------------------------------------------------------------------------------------

        T det (std::false_type) const
        {
            Matrix<T> support (*this);
            T det = support.fakeGauss ();

            return det;
        }

        //-----------------------------------------------------------------------------------------------------

        T det (std::true_type) const
        {
            Matrix<double> support (*this);
            double det = support.det ();

            return round (det);
        }

        //-----------------------------------------------------------------------------------------------------

        T fakeGauss ()  // only for square matrix
        {
            int sign = 1;
            std::vector<T *> fakeRows;  // for fake swap rows
            std::vector<int> fakeCols;  // for fake swap cols

            for (size_t i = 0; i < nRows_; ++i) {
                fakeRows.push_back (arr_ + nCols_ * i);
                fakeCols.push_back (i);
            }

            for (size_t i = 0; i < nRows_; ++i) {
                sign *= fakeSwapWithBiggest (fakeRows, fakeCols, i, true);
                sign *= fakeSwapWithBiggest (fakeRows, fakeCols, i, false);

                if (std::abs (fakeRows[i][fakeCols[i]]) <= EPSILON)
                    return T{};

                T max = fakeRows[i][fakeCols[i]];

                for (size_t j = i + 1; j < nRows_; ++j) {
                    T del = fakeRows[j][fakeCols[i]] / max;

                    for (size_t k = i + 1; k < nCols_; ++k)  // we can not nullify the column that we will not pay attention to and thn k = i + 1 (not i)
                        fakeRows[j][fakeCols[k]] -= del * fakeRows[i][fakeCols[k]];
                }
            }

            T determinant = fakeRows[0][fakeCols[0]];
            for (size_t i = 1; i < nCols_; ++i)
                determinant *= fakeRows[i][fakeCols[i]];

            return (sign == 1) ? determinant : -determinant;
        }

        //-----------------------------------------------------------------------------------------------------

        Matrix &mul (const Matrix<T> &other)  // multiply selector
        {
            if (nCols_ != other.nRows_)
                throw std::logic_error{"The number of columns of the first matrix does not match the number of rows of the second!"};

            Matrix<T> temporary_m (nRows_, other.nCols_);
            Matrix<T> B (other);

            B.transpose ();
            T *otherArr = B.arr_;
            for (int i = 0; i < nRows_; ++i) {
                for (int j = 0; j < other.nCols_; ++j) {
                    T temporary_sum = T{};
                    for (int k = 0; k < other.nRows_; ++k)
                        temporary_sum += arr_[i * nCols_ + k] * otherArr[j * B.nCols_ + k];
                    temporary_m[i][j] = temporary_sum;
                }
            }

            std::swap (temporary_m, *this);
            return *this;
        }

        //-----------------------------------------------------------------------------------------------------

        template <typename distrType>
        static Matrix<T> rndMatr (const size_t size, const int det)
        {
            const int maxCoef = maxCoefConst;  // I don't want big coef
            unsigned seed = std::chrono::system_clock::now ().time_since_epoch ().count ();
            std::mt19937 generator (seed);

            int absDet = std::abs (det);
            distrType randUpperTriangle (-absDet, absDet);

            Matrix<T> rndMtrx (size, size);

            for (size_t i = 0; i < size - 1; ++i) {
                rndMtrx[i][i] = 1;

                for (size_t j = i + 1; j < size; ++j)
                    rndMtrx[i][j] = randUpperTriangle (generator);
            }

            rndMtrx[size - 1][size - 1] = det;

            distrType coefGen (-maxCoef, maxCoef);  // I don't want big coef
            int randCoef;

            for (size_t i = 1; i < size; ++i) {
                randCoef = coefGen (generator);

                for (size_t j = 0; j < size; ++j)
                    rndMtrx[i][j] += randCoef * rndMtrx[0][j];
            }

            for (size_t i = 0; i < size - 1; ++i) {
                randCoef = coefGen (generator);

                for (size_t j = 0; j < size; ++j)
                    rndMtrx[j][i] += randCoef * rndMtrx[j][size - 1];
            }

            return rndMtrx;
        }

    public:
        Matrix (const size_t nRows = 0, const size_t nCols = 0)  // ctor
            : nRows_ (nRows),
              nCols_ (nCols),
              arr_ (new T[nRows * nCols])
        {
        }

        //-----------------------------------------------------------------------------------------------------

        Matrix (const size_t nRows, const size_t nCols, T val)  // ctor
            : nRows_ (nRows),
              nCols_ (nCols),
              arr_ (new T[nRows * nCols])
        {
            try {
                std::fill_n (arr_, nRows_ * nCols_, val);
            }
            catch (const std::exception &e) {
                delete[] arr_;
                throw;
            }
        }

        //-----------------------------------------------------------------------------------------------------

        Matrix (const Matrix<T> &other)
            : nRows_ (other.nRows_),
              nCols_ (other.nCols_),
              arr_ (new T[nRows_ * nCols_])  // copy ctor
        {
            try {
                std::copy (other.arr_, other.arr_ + nRows_ * nCols_, arr_);
            }
            catch (const std::exception &e) {
                delete[] arr_;
                throw;
            }
        }

        //-----------------------------------------------------------------------------------------------------

        template <typename otherT>
        Matrix (const Matrix<otherT> &other)
            : nRows_ (other.getNRows ()),
              nCols_ (other.getNCols ()),
              arr_ (new T[nRows_ * nCols_])  // copy ctor from other type
        {
            try {
                for (size_t i = 0; i < nRows_; ++i)
                    for (size_t j = 0; j < nCols_; ++j)
                        arr_[i * nCols_ + j] = other[i][j];
            }
            catch (const std::exception &e) {
                delete[] arr_;
                throw;
            }
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
            catch (const std::exception &e) {
                nullify ();
                throw;
            }
            nRows_ = other.nRows_;
            nCols_ = other.nCols_;

            try {
                std::copy (other.arr_, other.arr_ + nRows_ * nCols_, arr_);
            }
            catch (const std::exception &e) {
                delete[] arr_;
                nullify ();
                throw;
            }

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

        size_t getNRows () const noexcept
        {
            return nRows_;
        }

        //-----------------------------------------------------------------------------------------------------

        size_t getNCols () const noexcept
        {
            return nCols_;
        }

        //-----------------------------------------------------------------------------------------------------

        T det () const
        {
            if (nRows_ != nCols_)
                throw std::logic_error{"matrix is not square! I don't know what are you want from me"};

            return det (std::is_integral<T> ());
        }

        //-----------------------------------------------------------------------------------------------------

        static Matrix<T> randomMatrix (const size_t size, const int det)
        {
            static_assert (std::is_fundamental<T> ());

            if constexpr (std::is_integral<T> ())
                return rndMatr<std::uniform_int_distribution<T>> (size, det);
            else
                return rndMatr<std::uniform_real_distribution<T>> (size, det);
        }

        //=====================================================================================================

        Proxy operator[] (const size_t row)
        {
            if (row >= nRows_)
                throw std::length_error{"you tried to get data from nonexistent row"};

            return Proxy (arr_ + nCols_ * row, nCols_);
        }

        //-----------------------------------------------------------------------------------------------------

        const Proxy operator[] (const size_t row) const
        {
            if (row >= nRows_)
                throw std::length_error{"you tried to get data from nonexistent row"};

            return Proxy (arr_ + nCols_ * row, nCols_);
        }

        //=====================================================================================================

        void dump (std::ostream &out) const
        {
            for (size_t i = 0; i < nRows_; ++i) {
                for (size_t j = 0; j < nCols_; ++j)
                    out << arr_[nCols_ * i + j] << " ";

                if (i + 1 != nRows_)
                    out << std::endl;
            }
        }

        //-----------------------------------------------------------------------------------------------------

        void input (std::istream &in)
        {
            for (size_t i = 0, size = nRows_ * nCols_; i < size; ++i)
                in >> arr_[i];
        }

        //-----------------------------------------------------------------------------------------------------

        bool equal (const Matrix<T> &other) const
        {
            if (nRows_ != other.nRows_ || nCols_ != other.nCols_)
                return false;
            
            return std::equal(arr_, arr_ + nRows_ * nCols_, other.arr_);
        }

        //-----------------------------------------------------------------------------------------------------

        Matrix &transpose ()
        {
            Matrix<T> trans (nCols_, nRows_);
            T *transArr = trans.arr_;

            for (size_t i = 0; i < nRows_; ++i)
                for (size_t j = 0; j < nCols_; ++j)
                    transArr[j * nRows_ + i] = arr_[i * nCols_ + j];

            std::swap (trans, *this);

            return *this;
        }

        //-----------------------------------------------------------------------------------------------------

        Matrix &operator*= (const Matrix<T> &other)
        {
            return mul (other);
        }

        //-----------------------------------------------------------------------------------------------------

        Matrix &operator+= (const Matrix<T> &other)
        {
            if (getNCols () != other.getNCols () || getNRows () != other.getNRows ())
                throw std::logic_error{"I can't add these matrix because their size isn't equal"};

            T *otherArr = other.arr_;
            for (int i = 0; i < nRows_; ++i)
                for (int j = 0; j < nCols_; ++j)
                    arr_[i * nCols_ + j] += otherArr[i * nCols_ + j];

            return (*this);
        }

        //-----------------------------------------------------------------------------------------------------

        template <typename delType>
        Matrix &operator/= (delType del)
        {
            if (del == delType{})
                throw std::logic_error{"Floating point exception\n"};

            for (int i = 0; i < nRows_; ++i)
                for (int j = 0; j < nCols_; ++j)
                    arr_[i * nCols_ + j] /= del;

            return (*this);
        }
    };

    //=====================================================================================================

    template <typename T = double>
    std::ostream &operator<< (std::ostream &out, const Matrix<T> &matrix)
    {
        matrix.dump (out);
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
    Matrix<T> operator+ (const Matrix<T> &first, const Matrix<T> &second)
    {
        Matrix<T> result (first);

        return result += second;
    }

    //-----------------------------------------------------------------------------------------------------

    template <typename T = double>
    bool operator== (const Matrix<T> &first, const Matrix<T> &second)
    {
        return first.equal (second);
    }

    //-----------------------------------------------------------------------------------------------------

    template <typename T = double>
    Matrix<T> operator~ (const Matrix<T> &matrix)
    {
        Matrix<T> other (matrix);

        return other.transpose ();
    }

    //-----------------------------------------------------------------------------------------------------

    template <typename T = double>
    Matrix<T> operator* (const Matrix<T> &first, const Matrix<T> &second)
    {
        Matrix<T> copy (first);
        return copy *= second;
    }

}  // namespace matrix

#endif