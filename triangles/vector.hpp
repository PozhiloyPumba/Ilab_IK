#pragma once

#include <iostream>
#include <algorithm>
#include <cmath>

// TODO: cross and dot 

namespace vector{
	template <typename T>
	class vector_t{
		T coord_[3];

		public:

		vector_t(T x = 0, T y = 0, T z = 0){
			coord_[0] = x;
			coord_[1] = y;
			coord_[2] = z;
		}
		vector_t(const vector_t<T> &other){
			for(size_t i = 0; i < 3; ++i)
				set_coord(other.get_coord(i), i);
		}

		T get_coord(int axis) const{
			return coord_[axis];
		}

		void set_coord(T x, int axis){
			coord_[axis] = x;
		}

		T max_abs_coord(){
			return std::max({std::abs(coord_[0]), std::abs(coord_[1]), std::abs(coord_[2])});
		}

		vector_t<T> operator- ();
		vector_t<T> &operator= (const vector_t<T> &vec);
		vector_t<T> operator+ (const vector_t<T> &vec);
		vector_t<T> operator- (const vector_t<T> &vec);
		vector_t<T> operator/ (const float coef);
		vector_t<T> &operator/= (const float coef);
		vector_t<T> &operator-= (const vector_t<T> &vec);
		vector_t<T> &operator+= (const vector_t<T> &vec);
		vector_t<T> &operator*= (const float coef);
	};
	
	template <typename T>
	vector_t<T> vector_t<T>::operator- (){
		return vector_t(-coord_[0], -coord_[1], -coord_[2]);
	}

	template <typename T>
	vector_t<T> vector_t<T>::operator+ (const vector_t<T> &vec){
		return vector_t(coord_[0] + vec.get_coord(0), coord_[1] + vec.get_coord(1), coord_[2] + vec.get_coord(2));
	}
	template <typename T>
	vector_t<T> vector_t<T>::operator- (const vector_t<T> &vec){
		return vector_t(coord_[0] - vec.get_coord(0), coord_[1] - vec.get_coord(1), coord_[2] - vec.get_coord(2));
	}
	template <typename T>
	vector_t<T> vector_t<T>::operator/ (const float coef){
		return vector_t(coord_[0] / coef, coord_[1] / coef, coord_[2] / coef);
	}
	template <typename T>
	vector_t<T> &vector_t<T>::operator/= (const float coef){
		for(size_t i = 0; i < 3; ++i)
			coord_[i] /= coef;
		return *this;
	}

	template <typename T>
	vector_t<T> &vector_t<T>::operator= (const vector_t<T> &vec){
		for(size_t i = 0; i < 3; ++i)
			coord_[i] = vec.get_coord(i);
		return *this;
	}

	template <typename T>
	vector_t<T> &vector_t<T>::operator-= (const vector_t<T> &vec){
		for(size_t i = 0; i < 3; ++i)
			coord_[i] -= vec.get_coord(i);
		return *this;
	}
	template <typename T>
	vector_t<T> &vector_t<T>::operator+= (const vector_t<T> &vec){
		for(size_t i = 0; i < 3; ++i)
			coord_[i] += vec.get_coord(i);
		return *this;
	}
	template <typename T>
	vector_t<T> &vector_t<T>::operator*= (const float coef){
		for(size_t i = 0; i < 3; ++i)
			coord_[i] *= coef;
		return *this;
	}

	template <typename T>
	vector_t<T> operator* (const vector_t<T> &vec, const float coef){
		return vector_t<T>(vec.get_coord(0) * coef, vec.get_coord(1) * coef, vec.get_coord(2) * coef);
	}

	template <typename T>
	vector_t<T> operator* (const float coef, const vector_t<T> &vec){
		return vector_t<T>(vec.get_coord(0) * coef, vec.get_coord(1) * coef, vec.get_coord(2) * coef);
	}
	
	template <typename T>
	std::ostream &operator<< (std::ostream &left, const vector_t<T> &right){
		left << "(" << right.get_coord(0) << "; " << right.get_coord(1) << "; " << right.get_coord(2) << ")";
		
		return left; 
	}

	template <typename T>
	std::istream &operator>> (std::istream &left, vector_t<T> &right){
		T x;

		for(size_t i = 0; i < 3; ++i)
		{
			left >> x;
			right.set_coord(x, i);
		}
	
		return left; 
	}
}