#include "../cache.hpp"
#include <cassert>

//=====================================================================================================

void RunSingleTest (const std::pair<std::string, size_t> &test);

//=====================================================================================================

int main () {
	const std::pair<std::string, size_t> tests [] = {
							{"tests/test1.txt", 183},
							{"tests/test2.txt", 459},
							{"tests/test3.txt", 23},
							{"tests/test4.txt", 369},
							{"tests/test5.txt", 2285},
							{"tests/test6.txt", 10},
							{"tests/test7.txt", 32},
							{"tests/test8.txt", 7},
							{"tests/test9.txt", 10},       	// size of test10.txt it is almost 4 Gb and I don't want push it in git
							{"tests/test11.txt", 1998071}
	};

	for (size_t i = 0; i < 10; i++)
		RunSingleTest (tests [i]);

	return 0;
}

//=====================================================================================================

void RunSingleTest (const std::pair<std::string, size_t> &test)
{
	assert(freopen (test.first.c_str(), "r", stdin));

	size_t cache_size, count_input;

	std::cin >> cache_size >> count_input;
	assert(std::cin.good() == true);

	cache_t <int> c (cache_size);

	size_t hit_counter = 0;

	for(size_t i = 0; i < count_input; ++i)
	{
		int key;
		std::cin >> key;
		assert(std::cin.good() == true);

		hit_counter += c.lookup_update(key);
	}

	fclose(stdin);

	if (hit_counter != test.second) {
        std::cout << "(" << test.first << ")" << "This test is so BAD, MAAAAAAAAN! GO DEBUG AND REPEAT THIS AGAIN!" << std::endl;
        std::cout << "	- Expected : " << test.second << std::endl;
        std::cout << "	- Got : " << hit_counter << std::endl;
    } else {
        std::cout << "(" << test.first << ")" << " YEAH you are GODDAMN RIGHT in this test!" << std::endl;
    }

    std::cin.clear ();
}