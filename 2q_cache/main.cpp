#include "cache.hpp"
#include <cassert>

//=====================================================================================================

int main()
{
	int sz, count;
	int hit_counter = 0;
	std::cin >> sz >> count;
	assert(std::cin.good());

	cache_t <int> c (sz);

	for(int i = 0; i < count; i++)
	{
		int key;
		std::cin >> key;
		assert(std::cin.good());

		hit_counter += c.lookup_update(key);
	}

	std::cout << hit_counter << std::endl;
	return 0;
}
