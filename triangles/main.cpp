#include <iostream>
#include <cmath>
#include "vector.hpp"
#include "triangles.hpp"

int main()
{
	size_t count_triangles;
	float max_point = 0;
	float max_in_triangle = 0;
	std::cin >> count_triangles;

	triangle::triangle_t<float> *arr_of_triangles = new triangle::triangle_t<float> [count_triangles];

	for(size_t i = 0; i < count_triangles; ++i)
	{
		std::cin >> arr_of_triangles[i];

		max_in_triangle = arr_of_triangles[i].max_abs_coord();

		if(max_in_triangle > max_point)
			max_point = max_in_triangle;
	}

	delete [] arr_of_triangles;

	return 0;
}