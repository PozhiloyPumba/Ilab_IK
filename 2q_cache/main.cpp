#include <iostream>
#include <unordered_map>
#include <list>
#include "cache.hpp"

//=====================================================================================================

int main()
{
	int sz, count;
	int hit_counter = 0;
	std::cin >> sz >> count;
	cache_t <int, int> c (sz);

	for(int i = 0; i < count; i++)
	{
		int key;
		std::cin >> key;
		hit_counter += c.lookup_update(key);
	}

	std::cout << hit_counter << std::endl;
	return 0;
}