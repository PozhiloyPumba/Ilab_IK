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

	box_t(size_t sz = 0){
		sz_ = sz;
	}
	
	bool full() const{
		if(box_.size() < sz_)
			return false;
		return true;
	}
};

//-----------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------

template <typename T>
class cache_t{
	const size_t sz_;

	using ListIt = typename std::list< std::pair <T, name_of_boxes> >::iterator;
	std::unordered_map<T, ListIt> hash_;
	
	const float out_part = 0.6;
	const float hot_part = 0.2;

	box_t <T> in_ {size_t (sz_ - size_t (sz_ * out_part) - size_t (sz_ * hot_part))};
	box_t <T> out_ {size_t(sz_ * out_part)};
	box_t <T> hot_ {size_t(sz_ * hot_part)};

	void no_in_cache(T key)
	{
		if(in_.full()){
			if(out_.full()){
				if(out_.sz_ == 0){
					delete_last(in_.box_);
				}
				delete_last(out_.box_);					
			}
			auto elit = hash_.find((in_.box_.back()).first)->second;
			elit->second = OUT;
			out_.box_.splice(out_.box_.begin(), in_.box_, elit);
		}
		in_.box_.push_front({key, IN});
		hash_.insert({key, in_.box_.begin()});
	}

//-----------------------------------------------------------------------------------------------------

	void in_cache(auto elit)
	{
		switch(elit->second){ // name of box
			case IN:	// if elem locates in "in_" then we do nothing
				break;

			case OUT:	// if elem locates in "out_" then we splice it in "hot_" and if "hot_" is full we displace last elem of "hot_"	
				if(hot_.full()) {
					delete_last(hot_.box_);
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
	}

//-----------------------------------------------------------------------------------------------------
	
	void delete_last(std::list< std::pair <T, name_of_boxes> > &victim)
	{
		hash_.erase(hash_.find(victim.back().first));
		victim.pop_back();
	}

//-----------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------

	public:

		cache_t (size_t sz) : sz_ (sz){
		}

//-----------------------------------------------------------------------------------------------------

		bool lookup_update(T key)
		{
			auto hit = hash_.find(key);

			if(hit == hash_.end()){	// if elem is not found in lists, we insert it in "in_" and splice back of "in_" to out and back of "out_" send to hell
				no_in_cache(key);
				return false;
			}

			in_cache(hit->second);
			
			return true;
		}
};
