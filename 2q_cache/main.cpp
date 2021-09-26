#include "cache.hpp"
#include <cassert>

//=====================================================================================================

int main()
{
	size_t sz, count;
	size_t hit_counter = 0;
	std::cin >> sz >> count;
	assert(std::cin.good() == true);

	cache_t <int> c (sz);

	for(size_t i = 0; i < count; ++i)
	{
		int data;
		std::cin >> data;
		assert(std::cin.good() == true);

		hit_counter += c.lookup_update(data);
	}

	std::cout << "hits = " << hit_counter << std::endl;
	return 0;
}
