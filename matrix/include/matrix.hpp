#ifndef __MATRIX_HPP__
#define __MATRIX_HPP__

#include <complex>
#include <ctime>
#include <iostream>

namespace matrix {
    const double EPSILON = 10E-15;

    template <typename T = float>
    class Matrix final {
    private:
        int nRows_, nCols_;
        T *arr_;

        //-----------------------------------------------------------------------------------------------------

        int maxSubColElem (T **rows, int nCol)
        {
            T *max = rows[nCol];
            int maxCol = nCol;

            for (int i = nCol + 1; i < nRows_; ++i) {
                if (std::abs (max[nCol]) < std::abs (rows[i][nCol])) {
                    max = rows[i];
                    maxCol = i;
                }
            }

            return maxCol;
        }

    public:
        Matrix (const int nRows, const int nCols, T val = T{})  // ctor
            : nRows_ (nRows), nCols_ (nCols), arr_ (new T[nRows * nCols])
        {
            std::fill_n (arr_, nRows_ * nCols_, val);
        }

        //-----------------------------------------------------------------------------------------------------

        Matrix (const Matrix<T> &other)
            : nRows_ (other.nRows_), nCols_ (other.nCols_), arr_ (new T[other.nRows_ * other.nCols_])  // copy ctor
        {
            T *data = other.arr_;
            for (int i = 0, size = nRows_ * nCols_; i < size; ++i)
                arr_[i] = data[i];
        }

        //-----------------------------------------------------------------------------------------------------

        template <typename otherT>
        Matrix (const Matrix<otherT> &other)
            : nRows_ (other.getNRows ()), nCols_ (other.getNCols ()), arr_ (new T[nRows_ * nCols_])
        {
            for (int i = 0; i < nRows_; ++i)
                for (int j = 0; j < nCols_; ++j)
                    arr_[i * nCols_ + j] = other[i][j];
        }

        //-----------------------------------------------------------------------------------------------------

        Matrix (Matrix<T> &&other) noexcept
            : nRows_ (other.nRows_), nCols_ (other.nCols_), arr_ (other.arr_)  // move ctor
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

        Matrix &operator= (Matrix<T> &&other)  // move operator
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

        static Matrix randomIntMatrix (int size, int det)
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

        struct Proxy {
            T *row_;
            int nCols_;

            Proxy (T *row, int nCols = -1)
                : row_ (row),
                  nCols_ (nCols)
            {
            }

            //-----------------------------------------------------------------------------------------------------

            const T &operator[] (int col) const  // in case of an error, it will return a reference to the first element
            {
                if (nCols_ == -1 || col >= nCols_) {
                    std::cout << "there is no such element in the matrix" << std::endl;
                    return row_[0];
                }

                return row_[col];
            }

            //-----------------------------------------------------------------------------------------------------

            T &operator[] (int col)  // in case of an error, it will return a reference to the first element
            {
                if (nCols_ == -1 || col >= nCols_) {
                    std::cout << "there is no such element in the matrix" << std::endl;
                    return row_[0];
                }

                return row_[col];
            }
        };

        //-----------------------------------------------------------------------------------------------------

        Proxy operator[] (const int row)  // in case of an error, it will return a reference to the first element
        {
            if (row >= nRows_)
                return arr_;

            return Proxy (arr_ + nCols_ * row, nCols_);
        }

        //-----------------------------------------------------------------------------------------------------

        const Proxy operator[] (const int row) const  // in case of an error, it will return a reference to the first element
        {
            if (row >= nRows_)
                return arr_;

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

        T det () const;

        //-----------------------------------------------------------------------------------------------------

        T gauss ()
        {
            int sign = 1;
            T **rows = new T *[nRows_];  // for fake swap rows
            for (int i = 0; i < nRows_; ++i)
                rows[i] = arr_ + nCols_ * i;

            for (int i = 0; i < nRows_; ++i) {
                int rowWithMax = maxSubColElem (rows, i);

                if (rowWithMax != i) {
                    std::swap (rows[i], rows[rowWithMax]);
                    sign *= -1;
                }

                if (std::abs (rows[i][i]) <= EPSILON)
                    return T{0};

                T max = rows[i][i];

                for (int j = i + 1; j < nRows_; ++j) {
                    T del = rows[j][i] / max;

                    for (int k = i; k < nCols_; ++k)
                        rows[j][k] -= del * rows[i][k];
                }
            }

            T det = (sign == 1) ? rows[0][0] : -rows[0][0];
            for (int i = 1; i < nCols_; ++i)
                det *= rows[i][i];

            delete[] rows;

            return det;
        }
    };

    //-----------------------------------------------------------------------------------------------------

    template <>
    int Matrix<int>::det () const
    {
        if (nRows_ != nCols_) {
            std::cout << "matrix is not square! I don't know what are you want from me" << std::endl;
            return 0;
        }

        Matrix<double> support = *this;
        int det = support.gauss ();

        return det;
    }

    //-----------------------------------------------------------------------------------------------------

    template <typename T>
    T Matrix<T>::det () const
    {
        if (nRows_ != nCols_) {
            std::cout << "matrix is not square! I don't know what are you want from me" << std::endl;
            return T{};
        }

        Matrix<T> support = *this;
        T det = support.gauss ();

        return det;
    }

    //=====================================================================================================

    template <typename T = float>
    std::ostream &operator<< (std::ostream &out, Matrix<T> &matrix)
    {
        matrix.dump ();
        return out;
    }

    //-----------------------------------------------------------------------------------------------------

    template <typename T = float>
    std::istream &operator>> (std::istream &in, Matrix<T> &matrix)
    {
        matrix.input (in);
        return in;
    }
}  // namespace matrix

#endif