#include <iostream>
#include <vector>
#include <limits>

#include "../impl.hpp"

void dijkstra(graph g, int start_vertex)
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
}

int main(int argc, char **argv)
{
    std::ifstream file ("input_wo.dat");
    // std::istream ff = file;
    // std::fstream file = std::cin;

    // if (!file)
    // {
    //     std::cout << "File opened incorrectly\n";
    // }

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

    return 0;
}