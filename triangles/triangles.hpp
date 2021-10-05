#pragma once

#include <iostream>
#include <algorithm>
#include "vector.hpp"

namespace triangle{
	template <typename T>
	class triangle_t{
		vector::vector_t<T> vertex_[3];

		public:

		triangle_t(vector::vector_t<T> x = 0, vector::vector_t<T> y = 0, vector::vector_t<T> z = 0){
			vertex_[0] = x;
			vertex_[1] = y;
			vertex_[2] = z;
		}

		vector::vector_t<T> get_vertex(int vertex_number) const{
			return vertex_[vertex_number];
		}

		void set_vertex(vector::vector_t<T> &x, int vertex_number){
			vertex_[vertex_number] = x;
		}

		T max_abs_coord(){
			return std::max({vertex_[0].max_abs_coord(), vertex_[1].max_abs_coord(), vertex_[2].max_abs_coord()});
		}
	};

	template <typename T>
	std::ostream &operator<< (std::ostream &left, const triangle_t<T> &right){
		left << "{ " << right.get_vertex(0) << " ;\n  " << right.get_vertex(1) << " ;\n  " << right.get_vertex(2) << " }\n";
		
		return left; 
	}

	template <typename T>
	std::istream &operator>> (std::istream &left, triangle_t<T> &right){
		vector::vector_t<T> x, y, z;
		left >> x >> y >> z;

		right.set_vertex(x, 0);
		right.set_vertex(y, 1);
		right.set_vertex(z, 2);
	
		return left; 
	}
	
}