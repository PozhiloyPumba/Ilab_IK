#pragma once

#include <iostream>
#include <unordered_map>
#include <list>

//=====================================================================================================

template <typename T, typename KeyT = int>
struct data_t{
	T data_;
	KeyT key;
};

//-----------------------------------------------------------------------------------------------------

template <typename T, typename KeyT = int>
class beladi_cache_t{
	const size_t cache_sz_;
	KeyT *data_keys_;
	size_t data_keys_sz_;

	std::unordered_map< KeyT, data_t < T, KeyT> > cache_;

	template <typename F>
	bool beladi_lookup(size_t index, F slow_get_page)
	{
		auto cache_find = cache_.find(data_keys_[index]);
		if(cache_find == cache_.end()){
			if(cache_.size() < cache_sz_){
				cache_.insert({data_keys_[index], {slow_get_page(data_keys_[index]), data_keys_[index]}});
				return false;
			}

			int common_last_in = -1;
			
			for(auto cache_elem : cache_)
			{
				auto elem = cache_elem.second;
				int last_in_elem = -1;

				for(size_t k = index; k < data_keys_sz_; ++k)
				{
					if(data_keys_[k] == elem.key){
						last_in_elem = k;
						break;
					}
				}
				if(last_in_elem == -1){
					common_last_in = -1;
					cache_.erase(cache_.find(elem.key));
					break;
				}
			
				if(last_in_elem > common_last_in){
					common_last_in = last_in_elem; 
				}
			}

			if(common_last_in != -1){
				cache_.erase(cache_.find(data_keys_[common_last_in]));
			}

			cache_.insert({data_keys_[index], {slow_get_page(data_keys_[index]), data_keys_[index]}});
			return false;
		}
		return true;
	}

//-----------------------------------------------------------------------------------------------------

	public:

	beladi_cache_t (size_t cache_sz, KeyT *data, size_t data_sz) : 
		cache_sz_ (cache_sz),
		data_keys_ (data),
		data_keys_sz_ (data_sz){
	}

	template <typename F>
	size_t count_beladi_hits(F slow_get_page)
	{
		size_t hit_counter = 0;
		for(size_t i = 0; i < data_keys_sz_; ++i)
		{
			hit_counter += beladi_lookup(i, slow_get_page);
		}

		return hit_counter;
	}
};