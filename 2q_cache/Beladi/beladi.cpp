#include <iostream>
#include <unordered_map>
#include "beladi.hpp"

//=====================================================================================================

template <typename T>
T slow_get_page(T key);

void read_data(int *data, size_t count);

//=====================================================================================================

int main()
{
	size_t sz, count;
	std::cin >> sz >> count;

	int *data = new int [count];

	read_data(data, count);

	beladi_cache_t <int> cache{sz, data, count};

	std::cout << "hits: " << cache.count_beladi_hits(slow_get_page <int>) << std::endl;

	delete [] data;

	return 0;
}

//=====================================================================================================

template <typename T>
T slow_get_page(T key)
{
	return key;
}

//-----------------------------------------------------------------------------------------------------

void read_data(int *data, size_t count)
{
	for(size_t i = 0; i < count; ++i)
		std::cin >> data[i];
}