
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
	
	cache_t (size_t sz) : // TODO: sz < 5 is dangerous
		sz_ (sz),
		in_ (sz - (sz / 5) * 4),
		hot_ (sz / 5),
		out_ (3 * (sz / 5)){
	}

//-----------------------------------------------------------------------------------------------------

	~cache_t <T, KeyT> (){
	}

//-----------------------------------------------------------------------------------------------------

	bool lookup_update(KeyT key){
		auto hit = hash_.find(key);

		if(hit == hash_.end()){
			if(in_.full()){
				if(out_.full()){
					hash_.erase(hash_.find((out_.box_.back()).first));
					out_.box_.pop_back();
				}
				out_.box_.push_front({in_.box_.back().first, OUT});
				auto hit1 = hash_.find(in_.box_.back().first);
				hash_.erase(hit1);
				
				in_.box_.pop_back();
				hash_.insert({out_.box_.begin()->first, out_.box_.begin()});
			}
			in_.box_.push_front({key, IN});
			hash_.insert({key, in_.box_.begin()});

			return false;
		}
		auto elit = hit->second;

		switch(elit->second){ // name of box
			case IN:
				break;
			case OUT:
				if(hot_.full())
				{
					hash_.erase(hash_.find(hot_.box_.back().first));
					hot_.box_.pop_back();
				}

				hot_.box_.push_front({elit->first, HOT});
				hash_.erase(hit);
				out_.box_.erase(elit);
				hash_.insert({hot_.box_.begin()->first, hot_.box_.begin()});
				break;
			case HOT:
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
