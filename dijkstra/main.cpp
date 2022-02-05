#include <iostream>
#include <vector>
#include <limits>

#include "../include/graph.hpp"
#include "../include/pqueue.hpp"

void dijkstra(graph g, int start_vertex, int out_vert = 0)
{
    constexpr int INF = std::numeric_limits<int>::max();

    std::vector<int> dist(g.V.size());
    std::vector<bool> prev(g.V.size());

    for (auto v: g.V)
    {
        dist[v] = INF;
        prev[v] = -1;
    }

    dist[start_vertex] = 0;

    // declarar queue com os vértices e pesos com dist
    pqueue<int> queue(g.V, dist);

    while (!queue.empty())
    {
        const auto u = queue.pop_front(); // acessa o primeiro item da fila
        
        for (auto v: g.edges[u])
        {
            const auto ndist = dist[u] + v.second;
            const auto vert = v.first;
            if (dist[vert] > ndist)
            {
                dist[vert] = ndist;
                prev[vert] = u;
                queue.set_priority(vert, dist[vert]);
            }
        }
    }

    if (out_vert != 0)
    {
        std::cout << dist[out_vert] << '\n';
    }
    else
    {
        for (size_t i = 1, max = dist.size(); i < max; i++)
        {
            const auto& val = dist[i];
            std::cout << i << ':' << val << ' ';
        }
        std::cout << '\n';
    }
}

int main(int argc, char **argv)
{
    


    std::ifstream file ("input_w.dat");
    
    bool flag = true;

    graph G;

    if (flag)
    {
        G.from_file_stream(file);
    }
    else
    {
        G.from_file_stream(std::cin);
    }

    // puts("this main");

    // graph G(file);

    for (int i = 0, max_size = G.edges.size(); i < max_size; i++)
    {
        for (int j = 0, nother_max = G.edges[i].size(); j < nother_max; j++)
        {
            auto& val = G.edges[i][j];
            printf("%i %i %i\n", i, val.first, val.second);
        }
    }

    dijkstra(G, 1);

    return 0;
}