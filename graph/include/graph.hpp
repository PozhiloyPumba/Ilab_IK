#ifndef GRAPH_HPP__
#define GRAPH_HPP__

#include <vector>
#include <array>

namespace Graph {
    template <typename EDType = int, typename VDType = int>
    class KnutGraph {
        std::vector<std::array<int, 3> top;
        std::vector<EDType> edgeData;
        std::vector<VDType> vertexData;
    
    public:
        
        template <It, std::__void_t < decltype (++std::declval<It>()), decltype (std::declval<It>() != std::declval<It>()), decltype (*std::declval<It>())>>
        KnutGraph (It begin, It end) {}

        KnutGraph (const std::initializer_list<std::pair <int, int>> &edges) : KnutGraph (edges.begin(), edges.end ())
    };
}

#endif