#pragma once

#include <iostream>
#include <cmath>
#include <list>
#include "triangles.hpp"

namespace tree{
	template <typename T>
	class octotree_t{
		private:
			using ListIt = typename std::list < triangle::triangle_t <T> > ::iterator;

			struct node_t{
				vector::vector_t <T> right_border_, left_border_;

				node_t (vector::vector_t <T> left = 0, vector::vector_t <T> right = 0) :
					right_border_ (right),
					left_border_ (left){
				}
				node_t *child_[8] {};
				std::list < triangle::triangle_t <T> > list_of_triangles_; 
			};
			
			node_t root_;

			int what_chapter(vector::vector_t <T> &left_border, vector::vector_t <T> &right_border, const triangle::triangle_t <T> &tr)
			{
				int chapter[3] = {}; // (z,y,x)

				for(size_t i = 0; i < 3; ++i)
				{
					vector::vector_t <T> tmp = tr.get_vertex(i);

					for(size_t j = 0; j < 3; ++j) // 1 is positive
					{
						T mid_j = ((left_border + right_border) / 2).get_coord(j);

						if(tmp.get_coord(j) > mid_j){
							chapter[i] |= 1 << j;
						}
						else if(tmp.get_coord(j) == mid_j)
							return -1;
					}

					if(i){
						if(chapter[i] != chapter[i - 1])
							return -1;
					}
				}
				return chapter[0];
			}

			void sift_tree(node_t &cur_root)
			{
				if(cur_root.list_of_triangles_.size() <= 2 || (cur_root.right_border_ - cur_root.left_border_).length() < 0.001)
					return;

				ListIt it = cur_root.list_of_triangles_.begin();
				while(it != cur_root.list_of_triangles_.end())
				{
					int chapter = what_chapter(cur_root.left_border_, cur_root.right_border_, *it);

					if(chapter < 0){
						++it;
						continue;
					}

					if(!cur_root.child_[chapter]){
						cur_root.child_[chapter] = new node_t {};
						
						vector::vector_t <T> right = cur_root.right_border_;
						vector::vector_t <T> left = cur_root.left_border_;

						vector::vector_t <T> mid = (right + left) / 2;

						T tmp_mid, tmp_right, tmp_left;
						for(size_t j = 0; j < 3; ++j)
						{
							tmp_mid = mid.get_coord(j);
							tmp_right = right.get_coord(j);
							tmp_left = left.get_coord(j);

							cur_root.child_[chapter]->left_border_.set_coord(tmp_left * (1 - ((chapter >> j) & 1)) + tmp_mid * ((chapter >> j) & 1), j);
							cur_root.child_[chapter]->right_border_.set_coord(tmp_mid * (1 - ((chapter >> j) & 1)) + tmp_right * ((chapter >> j) & 1), j);
						}
					}
					ListIt tmp = it;

					if(it != cur_root.list_of_triangles_.end())
						tmp = next(it);
					
					cur_root.child_[chapter]->list_of_triangles_.splice(cur_root.child_[chapter]->list_of_triangles_.begin(), cur_root.list_of_triangles_, it);

					it = tmp;
				}

				for(size_t i = 0; i < 8; ++i)
				{
					if(!cur_root.child_[i])
						continue;

					sift_tree(*(cur_root.child_[i]));
				}
			}

			void delete_node(node_t &cur_root)
			{
				for(size_t i = 0; i < 8; ++i)
				{
					if(!cur_root.child_[i])
						continue;

					delete_node(*(cur_root.child_[i]));
				}
				delete &cur_root;
			}

		public:

			octotree_t() {
			}

			~octotree_t() {
				for(size_t i = 0; i < 8; ++i)
				{
					if(!root_.child_[i])
						continue;

					delete_node(*(root_.child_[i]));
				}
			}

			void fill_tree(size_t count_triangles)
			{
				T max_in_triangle, max_in_tree = 0;
				triangle::triangle_t <T> tmp;
				for(size_t i = 0; i < count_triangles; ++i)
				{
					std::cin >> tmp;

					root_.list_of_triangles_.push_front(tmp);

					max_in_triangle = tmp.max_abs_coord();
					if(max_in_triangle > max_in_tree)
						max_in_tree = max_in_triangle;
				}
				root_.right_border_ = {max_in_tree, max_in_tree, max_in_tree};
				root_.left_border_ = - root_.right_border_;
 
				sift_tree(root_);
			}

			void dump_tree(node_t &cur_root)
			{
				for(size_t i = 0; i < 8; ++i)
				{
					if(!cur_root.child_[i])
						continue;

					dump_tree(*(cur_root.child_[i]));
				}
				std::cout << "left = " << cur_root.left_border_ << "; right = " << cur_root.right_border_ << std::endl;

				for(auto v : cur_root.list_of_triangles_)
					std::cout << v << std::endl;

				std::cout << std::endl;
			}
	};
}