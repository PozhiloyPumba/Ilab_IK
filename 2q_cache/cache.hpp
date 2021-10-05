#pragma once

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

template <typename T, typename KeyT = int>
struct list_data_t{
	T data_;
	name_of_boxes place;
	KeyT key;
};

//-----------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------

template <typename T, typename KeyT = int>
struct box_t{
	size_t sz_;

	std::list< list_data_t <T, KeyT> > box_;

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

template <typename T, typename KeyT = int>
class cache_t{
	const size_t sz_;

	using ListIt = typename std::list< list_data_t <T, KeyT> >::iterator;
	std::unordered_map<KeyT, ListIt> hash_;
	
	const int out_frac = 3;
	const int hot_frac = 1;
	const int count_fracs = 5;

	box_t <T> in_ {sz_ - sz_ / count_fracs * out_frac - sz_ / count_fracs * hot_frac};
	box_t <T> out_ {sz_ / count_fracs * out_frac};
	box_t <T> hot_ {sz_ / count_fracs * hot_frac};

	template <typename F>
	void no_in_cache(KeyT key, F slow_get_page)
	{
		if(in_.full()){
			if(out_.full()){
				if(out_.sz_ == 0){
					delete_last(in_.box_);
					in_.box_.push_front({slow_get_page(key), IN, key});
					hash_.insert({key, in_.box_.begin()});

					return;
				}
				delete_last(out_.box_);					
			}
			auto elit = hash_.find((in_.box_.back()).key)->second;
			elit->place = OUT;
			out_.box_.splice(out_.box_.begin(), in_.box_, elit);
		}
		in_.box_.push_front({slow_get_page(key), IN, key});
		hash_.insert({key, in_.box_.begin()});
	}

//-----------------------------------------------------------------------------------------------------

	void in_cache(ListIt elit)
	{
		switch(elit->place){ // name of box
			case IN:	// if elem locates in "in_" then we do nothing
				break;

			case OUT:	// if elem locates in "out_" then we splice it in "hot_" and if "hot_" is full we displace last elem of "hot_"	
				if(hot_.full()) {
					delete_last(hot_.box_);
				}
				elit->place = HOT;
				hot_.box_.splice(hot_.box_.begin(), out_.box_, elit);
				break;

			case HOT:	// if elem locates in "hot_" we move elem into begin of "hot_"
				hot_.box_.splice(hot_.box_.begin(), hot_.box_, elit);
				break;

			default: 
				std::cout << "Error of name box " << elit->place << std::endl;
		}
	}

//-----------------------------------------------------------------------------------------------------
	
	void delete_last(std::list< list_data_t <T, KeyT> > &victim)
	{
		hash_.erase(hash_.find(victim.back().key));
		victim.pop_back();
	}

//-----------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------

	public:

		cache_t (size_t sz) : sz_ (sz){
		}

//-----------------------------------------------------------------------------------------------------
		template <typename F>
		bool lookup_update(KeyT key, F slow_get_page)
		{
			auto hit = hash_.find(key);

			if(hit == hash_.end()){	// if elem is not found in lists, we insert it in "in_" and splice back of "in_" to out and back of "out_" send to hell
				no_in_cache(key, slow_get_page);
				return false;
			}

			in_cache(hit->second);
			
			return true;
		}
};
