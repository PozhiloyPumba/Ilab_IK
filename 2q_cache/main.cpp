#include <iostream>
// #include <algorithm>
#include <unordered_map>
#include <list>

//=====================================================================================================

enum name_of_boxes{
	IN = 0,
	OUT = 1,
	HOT = 2,
};

//-----------------------------------------------------------------------------------------------------

template <typename T, typename KeyT = int>
struct box_t{
	size_t sz_;

	std::list< std::pair <T, name_of_boxes> > box_;
	// using ListIt = typename std::list< std::pair <T, int> >::iterator;

	box_t(size_t sz){
		sz_ = sz;
	}
	bool full() const{
		if(box_.size() < sz_)
			return false;
		return true;
	};
};

//-----------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------

template <typename T, typename KeyT = int>
struct cache_t{
	size_t sz_;

	using ListIt = typename std::list< std::pair <T, name_of_boxes> >::iterator;
	std::unordered_map<KeyT, ListIt> hash_;
	
	box_t <T, KeyT> in_;
	box_t <T, KeyT> out_;
	box_t <T, KeyT> hot_;

//-----------------------------------------------------------------------------------------------------
	
	cache_t (size_t sz) :
		sz_ (sz),
		in_ (sz / 5),
		hot_ (sz / 5),
		out_ (sz - 2 * (sz / 5)){
	}

//-----------------------------------------------------------------------------------------------------

	~cache_t <T, KeyT> (){
	}

//-----------------------------------------------------------------------------------------------------

	bool lookup_update(KeyT key){
		auto hit = hash_.find(key);

		if(hit == hash_.end()){
			// std::cout << "No in hash!" << std::endl;

			if(in_.full()){
				// std::cout << "Full in" << std::endl;

				if(out_.full()){
					// std::cout << "Full out" << std::endl;
					hash_.erase(hash_.find((out_.box_.back()).first));
					// std::cout << "hash erase out" << std::endl;
					out_.box_.pop_back();
					// std::cout << "pop back" << std::endl;
				}
				out_.box_.push_front({in_.box_.back().first, OUT});
				// std::cout << "push front" << std::endl;
				auto hit1 = hash_.find(in_.box_.back().first);
				// std::cout << "hash find" << (*((*hit1).second)).first << std::endl;
				hash_.erase(hit1);
				// std::cout << "hash erase in" << std::endl;
				
				in_.box_.pop_back();
				// out_.box_.begin()->second = OUT;
				hash_.insert({out_.box_.begin()->first, out_.box_.begin()});
			}
			in_.box_.push_front({key, IN});
			hash_.insert({key, in_.box_.begin()});
			std::cout << "out size = " << out_.box_.size() << "		in size = " << in_.box_.size() << "		hot size = " << hot_.box_.size() << std::endl;

			return false;
		}
		// std::cout << "kdhfkhs" << std::endl;

		auto elit = hit->second;

		switch(elit->second){ // name of box
			case IN:
				std::cout << "IN!" << elit->first << std::endl;
				break;
			case OUT:
				std::cout << "OUT!" << std::endl;
				if(hot_.full())
				{
					hash_.erase(hash_.find(hot_.box_.back().first));
					hot_.box_.pop_back();
				}
				std::cout << "elit " << elit->first << std::endl;

				hot_.box_.push_front({elit->first, HOT});
				hash_.erase(hit);
				out_.box_.erase(elit);
				hash_.insert({hot_.box_.begin()->first, hot_.box_.begin()});
				break;
			case HOT:
				std::cout << "HOT!" << std::endl;
				hash_.erase(hit);
				hot_.box_.push_front({elit->first, HOT});
				hash_.insert({hot_.box_.begin()->first, hot_.box_.begin()});
				hot_.box_.erase(elit);
				break;
			default: 
				std::cout << "Error of name box " << elit->second << elit->first << std::endl;
		}
		return true;
	}
};

//=====================================================================================================

int main()
{
	int sz, count;
	int hit_counter = 0;
	std::cin >> sz >> count;
	cache_t <int, int> c (sz);
	// std::cout << sizeof(name_of_boxes) << std::endl;

	for(int i = 0; i < count; i++)
	{
		int key;
		std::cin >> key;
		hit_counter += c.lookup_update(key);
		std::cout << i << "   :   " << key << std::endl;
	}

	std::cout << hit_counter << std::endl;
	return 0;
}