#include <iostream>
#include <vector>
#include <limits>
#include <string>
#include <sstream>

#include "../include/graph.hpp"
#include "../include/pqueue.hpp"

#include "../include/argparse.hpp"

void dijkstra(graph& g, std::stringstream& out_stream, int start_vertex, int out_vert, bool setted_solution)
{
    (void)setted_solution;
    
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
        out_stream << dist[out_vert] << '\n';
    }
    else
    {
        for (size_t i = 1, max = dist.size(); i < max; i++)
        {
            const auto& val = dist[i];
            out_stream << i << ':' << val << ' ';
        }
        out_stream << '\n';
    }
}

void usage(int argc, char **argv)
{
    std::string help (R"(
-h           : mostra o help (este texto)
-f <arquivo> : indica o "arquivo" que contém o grafo de entrada
-o <arquivo> : redireciona a saida para o "arquivo"
-i <vertice> : vértice inicial
-l <vertice> : vértice final
)");

    std::cout << "O uso do programa é:\n" << argv[0] << help;
}

int main(int argc, char **argv)
{
    program_parameters params;

    params.get_paramers(argc, argv, usage);

    graph G;

    if (params.setted_file_input)
    {
        std::ifstream in_file (params.file_input_path);
        if (!in_file)
        {
            std::cout << "File " << params.file_input_path <<  " does not exist";
            return -1;
        }
        G.from_file_stream(in_file);
    }
    else
    {
        G.from_file_stream(std::cin);
    }

    std::stringstream out_stream;

    dijkstra(G, out_stream, params.in_vertex, params.out_vertex, params.setted_solution);

    if (params.setted_file_output)
    {
        std::ofstream out_file (params.file_output_path);
        if (!out_file)
        {
            std::cout << "File " << params.file_output_path <<  " does not exist";
            return -1;
        }
        out_file << out_stream.rdbuf();
    }
    else
    {
        std::cout << out_stream.rdbuf();
    }

    return 0;
}