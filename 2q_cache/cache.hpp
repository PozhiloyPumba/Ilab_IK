#include <iostream>
#include <unordered_map>
#include <list>
#include <cstring>

enum name_of_boxes{
	IN = 0,
	OUT = 1,
	HOT = 2,
};

//-----------------------------------------------------------------------------------------------------

template <typename T>
struct box_t{
	size_t sz_;

	std::list< std::pair <T, name_of_boxes> > box_;

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

template <typename T>
struct cache_t{
	size_t sz_;

	using ListIt = typename std::list< std::pair <T, name_of_boxes> >::iterator;
	std::unordered_map<T, ListIt> hash_;
	
	box_t <T> in_;
	box_t <T> out_;
	box_t <T> hot_;

//-----------------------------------------------------------------------------------------------------
	
	cache_t (size_t sz) : //if sz < 5 is LRU
		sz_ (sz),
		in_ (sz - (sz / 5) * 4),
		hot_ (sz / 5),
		out_ (3 * (sz / 5)){
	}

//-----------------------------------------------------------------------------------------------------

	~cache_t (){
	}

//-----------------------------------------------------------------------------------------------------

	bool lookup_update(T key){
		auto hit = hash_.find(key);

		if(hit == hash_.end()){	// if elem is not found in lists, we 
			if(in_.full()){
				auto elit = hash_.find((in_.box_.back()).first)->second;
				elit->second = OUT;
				out_.box_.splice(out_.box_.begin(), in_.box_, elit);
				
				if(out_.full()){
					hash_.erase(hash_.find((out_.box_.back()).first));
					out_.box_.pop_back();
				}
			}
			in_.box_.push_front({key, IN});
			hash_.insert({key, in_.box_.begin()});

			return false;
		}
		auto elit = hit->second;

		switch(elit->second){ // name of box
			case IN:	// if elem locates in "in_" then we do nothing
				break;

			case OUT:	// if elem locates in "out_" then we splice it in "hot_" and if "hot_" is full we displace last elem of "hot_"	
				if(hot_.full()) {
					hash_.erase(hash_.find(hot_.box_.back().first));
					hot_.box_.pop_back();
				}
				elit->second = HOT;
				hot_.box_.splice(hot_.box_.begin(), out_.box_, elit);
				break;

			case HOT:	// if elem locates in "hot_" we move elem into begin of "hot_"
				hot_.box_.splice(hot_.box_.begin(), hot_.box_, elit);
				break;

			default: 
				std::cout << "Error of name box " << elit->second << elit->first << std::endl;
		}
		return true;
	}
};
