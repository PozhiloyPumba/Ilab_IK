#include <iostream>
#include <cmath>
#include "vector.hpp"
#include "triangles.hpp"
#include "octotree.hpp"

int main()
{
	size_t count_triangles;
	std::cin >> count_triangles;

	tree::octotree_t <float> tree;

	tree.fill_tree(count_triangles);

	return 0;
}