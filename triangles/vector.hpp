#pragma once

#include <iostream>
#include <algorithm>
#include <cmath>

// TODO: cross and dot 

namespace vector{
	template <typename T>
	class vector_t{

	private:
		T coord_[3];

	public:

		vector_t(T x = 0, T y = 0, T z = 0) : 
			coord_ {x, y, z}{
		}
		vector_t(const vector_t<T> &other){
			for(size_t i = 0; i < 3; ++i)
			{
				const T tmp = other.get_coord(i);
				set_coord(tmp, i);
			}
		}

		T get_coord(int axis) const
		{
			return coord_[axis];
		}

		void set_coord(T x, int axis)
		{
			coord_[axis] = x;
		}

		T max_abs_coord() const
		{
			return std::max({std::abs(coord_[0]), std::abs(coord_[1]), std::abs(coord_[2])});
		}

		float length() const
		{
			float sum = 0;
			for(size_t i = 0; i < 3; ++i)
				sum += coord_[i] * coord_[i];
			
			return sqrt(sum);
		}

		vector_t<T> cross(const vector_t<T> &vec) const
		{
			return vector_t<T> (coord_[1] * vec.coord_[2] - coord_[2] * vec.coord_[1], 
								coord_[2] * vec.coord_[0] - coord_[0] * vec.coord_[2], 
								coord_[0] * vec.coord_[1] - coord_[1] * vec.coord_[0]);
		}

		vector_t<T> dot(const vector_t<T> &vec) const
		{
			return 	coord_[0] * vec.coord_[0] + 
					coord_[1] * vec.coord_[1] + 
					coord_[2] * vec.coord_[2];
		}

		vector_t<T> &operator= (const vector_t<T> &vec);
		vector_t<T> operator- ();
		vector_t<T> operator+ (const vector_t<T> &vec);
		vector_t<T> operator- (const vector_t<T> &vec);
		vector_t<T> operator/ (const float coef);
		vector_t<T> &operator+= (const vector_t<T> &vec);
		vector_t<T> &operator-= (const vector_t<T> &vec);
		vector_t<T> &operator*= (const float coef);
		vector_t<T> &operator/= (const float coef);
		bool operator== (const vector_t<T> &vec);
		bool operator!= (const vector_t<T> &vec);
	};

	template <typename T>
	vector_t<T> &vector_t<T>::operator= (const vector_t<T> &vec){
		for(size_t i = 0; i < 3; ++i)
			coord_[i] = vec.get_coord(i);
		return *this;
	}
	
	template <typename T>
	vector_t<T> vector_t<T>::operator- (){
		return vector_t(-coord_[0],
						-coord_[1], 
						-coord_[2]);
	}

	template <typename T>
	vector_t<T> vector_t<T>::operator+ (const vector_t<T> &vec){
		return vector_t(coord_[0] + vec.get_coord(0), 
						coord_[1] + vec.get_coord(1), 
						coord_[2] + vec.get_coord(2));
	}
	template <typename T>
	vector_t<T> vector_t<T>::operator- (const vector_t<T> &vec){
		return vector_t(coord_[0] - vec.get_coord(0), 
						coord_[1] - vec.get_coord(1), 
						coord_[2] - vec.get_coord(2));
	}
	template <typename T>
	vector_t<T> vector_t<T>::operator/ (const float coef){
		return vector_t(coord_[0] / coef, 
						coord_[1] / coef, 
						coord_[2] / coef);
	}

	template <typename T>
	vector_t<T> &vector_t<T>::operator+= (const vector_t<T> &vec){
		for(size_t i = 0; i < 3; ++i)
			coord_[i] += vec.get_coord(i);
		return *this;
	}
	template <typename T>
	vector_t<T> &vector_t<T>::operator-= (const vector_t<T> &vec){
		for(size_t i = 0; i < 3; ++i)
			coord_[i] -= vec.get_coord(i);
		return *this;
	}
	template <typename T>
	vector_t<T> &vector_t<T>::operator*= (const float coef){
		for(size_t i = 0; i < 3; ++i)
			coord_[i] *= coef;
		return *this;
	}
	template <typename T>
	vector_t<T> &vector_t<T>::operator/= (const float coef){
		for(size_t i = 0; i < 3; ++i)
			coord_[i] /= coef;
		return *this;
	}

	template <typename T>
	bool vector_t<T>::operator== (const vector_t<T> &vec){
		for(size_t i = 0; i < 3; ++i)
			if(coord_[i] != vec.coord_[i])
				return false;
		return true;
	}
	template <typename T>
	bool vector_t<T>::operator!= (const vector_t<T> &vec){
		return !(*this == vec);
	}

	template <typename T>
	vector_t<T> operator* (const vector_t<T> &vec, const float coef){
		return vector_t<T>(	vec.get_coord(0) * coef,
							vec.get_coord(1) * coef, 
							vec.get_coord(2) * coef);
	}
	template <typename T>
	vector_t<T> operator* (const float coef, const vector_t<T> &vec){
		return vector_t<T>(	vec.get_coord(0) * coef, 
							vec.get_coord(1) * coef, 
							vec.get_coord(2) * coef);
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