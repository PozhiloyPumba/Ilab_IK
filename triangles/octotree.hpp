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
				T right_border = 0, left_border = 0;
				node_t *child[8] {};
				std::list < triangle::triangle_t <T> > list_of_triangles; 
			};
			
			node_t root;

			int what_chapter(const T left_border, const T right_border, const triangle::triangle_t <T> &tr)
			{
				T mid = (left_border + right_border) / 2;
				int chapter[3] = {}; // (x,y,z)

				for(size_t i = 0; i < 3; ++i)
				{
					vector::vector_t <T> tmp = tr.get_vertex(i);

					for(size_t j = 0; j < 3; ++j) // 1 is right
					{
						if(tmp.get_coord(j) > mid){
							chapter[i] |= 1 << (2 - j);
						}
						else if(tmp.get_coord(j) == mid)
							return -1;
					}

					if(i){
						if(chapter[i] != chapter[i - 1])
							return -1;
					}
				}
				return chapter[0];
			}

			void sift_tree(node_t &root)
			{
				if(root.list_of_triangles.size() <= 2)
					return;

				ListIt it = root.list_of_triangles.begin();
				while(it != root.list_of_triangles.end())
				{
					int chapter = what_chapter(root.left_border, root.right_border, *it);

					if(chapter < 0){
						++it;
						continue;
					}

					if(!root.child[chapter])
						root.child[chapter] = new node_t;

					ListIt tmp = it;

					if(it != root.list_of_triangles.end())
						tmp = next(it);
					
					root.child[chapter]->list_of_triangles.splice(root.child[chapter]->list_of_triangles.begin(), root.list_of_triangles, it);

					it = tmp;
				}

				for(size_t i = 0; i < 8; ++i)
				{
					if(!root.child[i])
						continue;

					sift_tree(*(root.child[i]));
				}
			}

			void delete_node(node_t &root)
			{
				for(size_t i = 0; i < 8; ++i)
				{
					if(!root.child[i])
						continue;

					delete_node(*(root.child[i]));
				}
				delete &root;
			}

		public:

			octotree_t() {
			}

			~octotree_t() {
				for(size_t i = 0; i < 8; ++i)
				{
					if(!root.child[i])
						continue;

					delete_node(*(root.child[i]));
				}
			}

			void fill_tree(size_t count_triangles)
			{
				T max_in_triangle;
				triangle::triangle_t <T> tmp;
				for(size_t i = 0; i < count_triangles; ++i)
				{
					std::cin >> tmp;

					root.list_of_triangles.push_front(tmp);

					max_in_triangle = tmp.max_abs_coord();
					if(max_in_triangle > root.right_border)
						root.right_border = max_in_triangle;
				}
				root.left_border = - root.right_border;
 
				sift_tree(root);

				// for(auto v : root.list_of_triangles)
				// 	std::cout << what_chapter(root.left_border, root.right_border, v) << std::endl;
			}
	};
}