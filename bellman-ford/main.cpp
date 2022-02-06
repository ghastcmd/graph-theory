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
    
    constexpr int INF = std::numeric_limits<int>::max();

    std::vector<int> cost(g.V.size());
    std::vector<int> prev(g.V.size());

    for (auto v: g.V)
    {
        cost[v] = INF;
        prev[v] = -1;
    }

    cost[start_vertex] = 0;

    // declarar queue com os vértices e pesos com dist
    pqueue<int> queue(g.V, cost);

    vec_pair intersection;

    while (!queue.empty())
    {
        const auto u = queue.pop_front(); // acessa o primeiro item da fila

        get_intersection(intersection, g.edges[u], queue);

        for (const auto& v: intersection)
        {
            const auto weight = v.second;
            const auto vert = v.first;
            if (cost[vert] > weight)
            {
                cost[vert] = weight;
                prev[vert] = u;
                queue.set_priority(vert, cost[vert]);
            }
        }
        intersection.clear();
    }

    if (setted_solution)
    {
        std::vector<std::pair<int, int>> solution;
        for (size_t i = 1, max_n = prev.size(); i < max_n; i++)
        {
            const auto val = prev[i];
            if (val != -1)
                solution.push_back({prev[i], i});
        }
        std::sort(solution.begin(), solution.end());
        for (const auto& val: solution)
        {
            out_stream << '(' << val.first << ',' << val.second << ") ";
        }
        out_stream << '\n';
    }
    else
    {
        size_t sum = 0;
        for (size_t i = 1, max = cost.size(); i < max; i++)
        {
            sum += cost[i];
        }
        out_stream << sum << '\n';
    }
}

void usage(int argc, char **argv)
{
    std::string help (R"(
-h           : mostra o help (este texto)
-f <arquivo> : indica o "arquivo" que contém o grafo de entrada
-o <arquivo> : redireciona a saida para o "arquivo"
-s           : mostra a solução (em ordem crescente)
-i <vertice> : vértice inicial
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