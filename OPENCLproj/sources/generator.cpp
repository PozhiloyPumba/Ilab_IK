#include <chrono>
#include <iostream>
#include <map>
#include <random>
#include <vector>

int main ()
{
    unsigned seed = std::chrono::system_clock::now ().time_since_epoch ().count ();
    std::mt19937 generator (seed);

    int border;
    std::cin >> border;
    std::uniform_int_distribution<int> distr{-border, border};

    int elemNum;
    std::cin >> elemNum;
    std::cout << elemNum << std::endl;

    for (int i = 0; i < elemNum; ++i)
        std::cout << distr (generator) << " ";

    std::cout << std::endl;
}