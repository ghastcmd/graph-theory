#include <iostream>
#include <vector>
#include <limits>
#include <string>
#include <sstream>

#include "../include/graph.hpp"
#include "../include/pqueue.hpp"

#include "../include/argparse.hpp"

using vec_pair = std::vector<std::pair<int, int>>;

void get_intersection(vec_pair& inter, vec_pair& vec1, pqueue<int>& queue)
{
    for (const auto& v1: vec1)
    {
        for (const auto& q: queue)
        {
            if (v1.first == q.second)
            {
                inter.push_back(v1);
            }
        }
    }
}

void bellman_ford(graph g, std::stringstream& out_stream, int start_vertex, int out_vert, bool setted_solution)
{
    (void)setted_solution;
    
    constexpr int INF = std::numeric_limits<int>::max() - 100;

    using matrix = typename std::vector<std::vector<int>>;

    const auto vert_size = g.V.size();

    matrix weights (vert_size, std::vector<int>(vert_size, INF));
    matrix dist (vert_size-1, std::vector<int>(vert_size, INF));

    for (size_t i = 1, max_i = weights.size(); i < max_i; i++)
    {
        for (const auto vv: g.edges[i])
        {
            weights[i][vv.first] = vv.second;
        }
    }

    dist[0][start_vertex] = 0;

    for (size_t l = 1, max_l = vert_size; l < max_l - 1; l++)
    {
        for (size_t k = 1; k < max_l; k++)
        {
            int min_ww = INF;
            for (size_t i = 1; i < max_l; i++)
            {
                auto val = INF;
                if (dist[l-1][i] != INF && weights[i][k] != INF)
                {
                    val = dist[l-1][i] + weights[i][k];
                }
                min_ww = std::min(min_ww, val);
            }

            dist[l][k] = std::min(dist[l-1][k], min_ww);
        }
    }

    if (out_vert == 0)
    {
        for (size_t i = 1, max = dist[vert_size-2].size(); i < max; i++)
        {
            std::cout << i << ':' << dist[vert_size-2][i] << ' ';
        }
        std::cout << '\n';
    }
    else
    {
        std::cout << dist[vert_size-2][out_vert] << '\n';
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

    bellman_ford(G, out_stream, params.in_vertex, params.out_vertex, params.setted_solution);

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