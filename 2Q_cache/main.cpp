#include "cache.hpp"
#include <cassert>

template <typename T>
T slow_get_page(T key){
	return key;
}

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

		hit_counter += c.lookup_update(data, slow_get_page <int>);
	}

	std::cout << "hits = " << hit_counter << std::endl;
	return 0;
}
