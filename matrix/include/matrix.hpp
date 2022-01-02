#ifndef __MATRIX_HPP__
#define __MATRIX_HPP__

#include <iostream>

namespace matrix {

    template <typename T = float>
    class Matrix {
    private:
        T *arr_;

        int nRows_, nCols_;

    public:
        Matrix (const int nRows, const int nCols, T val = T{})  // ctor
            : arr_ (new T[nRows * nCols]), nRows_ (nRows), nCols_ (nCols)
        {
            std::fill_n (arr_, nRows_ * nCols_, val);
        }

        //-----------------------------------------------------------------------------------------------------

        Matrix (const Matrix<T> &other)
            : arr_ (new T[other.nRows_ * other.nCols_]), nRows_ (other.nRows_), nCols_ (other.nCols_)  // copy ctor
        {
            T *data = other.arr_;
            for (int i = 0, size = nRows_ * nCols_; i < size; ++i)
                arr_[i] = data[i];
        }

        //-----------------------------------------------------------------------------------------------------

        Matrix (Matrix<T> &&other) noexcept
            : arr_ (other.arr_), nRows_ (other.nRows_), nCols_ (other.nCols_)  // move ctor
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
    };

    //=====================================================================================================

    template <typename T = float>
    std::ostream &operator<< (std::ostream &out, Matrix<T> &matrix)
    {
        matrix.dump ();
        return out;
    }

    template <typename T = float>
    std::istream &operator>> (std::istream &in, Matrix<T> &matrix)
    {
        matrix.input (in);
        return in;
    }
}  // namespace matrix

#endif