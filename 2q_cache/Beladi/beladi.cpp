#include <iostream>
#include <unordered_map>

//=====================================================================================================

void read_data(int *data, size_t count);

bool beladi_lookup(std::unordered_map <int, int> &cache, size_t sz_cache, int *data, size_t count_data, size_t index);

//=====================================================================================================

int main()
{
	size_t sz, count;
	size_t hit_counter = 0;
	std::cin >> sz >> count;

	std::unordered_map <int, int> cache;
	int *data = new int [count];

	read_data(data, count);

	for(size_t i = 0; i < count; ++i)
	{
		hit_counter += beladi_lookup(cache, sz, data, count, i);
	}

	std::cout << "hits: " << hit_counter << std::endl;

	return 0;
}

//=====================================================================================================

void read_data(int *data, size_t count)
{
	for(size_t i = 0; i < count; ++i)
	{
		std::cin >> data[i];
	}
}

//-----------------------------------------------------------------------------------------------------

bool beladi_lookup(std::unordered_map <int, int> &cache, size_t sz_cache, int *data, size_t count_data, size_t index)
{
	auto cache_find = cache.find(data[index]);
	if(cache_find == cache.end()){
		if(cache.size() < sz_cache){
			cache.insert({data[index], data[index]});
			return false;
		}

		int common_last_in = -1;
		
		for(auto cache_elem : cache)
		{
			int elem = cache_elem.second;
			int last_in_elem = -1;

			for(size_t k = index; k < count_data; ++k)
			{
				if(data[k] == elem){
					last_in_elem = k;
					break;
				}
			}
			if(last_in_elem == -1){
				common_last_in = -1;
				cache.erase(cache.find(elem));
				break;
			}
		
			if(last_in_elem > common_last_in){
				common_last_in = last_in_elem; 
			}
		}

		if(common_last_in != -1){
			cache.erase(cache.find(data[common_last_in]));
		}

		cache.insert({data[index], data[index]});
		return false;
	}
	return true;
}
