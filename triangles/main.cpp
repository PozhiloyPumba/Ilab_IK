#include <iostream>
#include "octotree.hpp"

int main()
{
	size_t countTriangles;
	std::cin >> countTriangles;

	tree::octotree_t <double> tree;

	tree.fill_tree(countTriangles);

	return 0;
}