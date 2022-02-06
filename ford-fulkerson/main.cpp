#include <iostream>
#include <vector>
#include <limits>
#include <string>
#include <sstream>

#include "../include/graph.hpp"
#include "../include/pqueue.hpp"

#include "../include/argparse.hpp"

#include <queue>

using matrix = typename std::vector<std::vector<int>>;

bool find_paths_bfs(matrix& res_graph, int start_vertex, int out_vertex, std::vector<int>& parent)
{
    const auto max_vert_size = res_graph[0].size();
    std::vector<bool> visited(max_vert_size, false);
    std::queue<int> queue;

    queue.push(start_vertex);
    visited[start_vertex] = true;

    while (!queue.empty())
    {
        const auto vert = queue.front();
        queue.pop();

        for (size_t v = 1; v < max_vert_size; v++)
        {
            if (!visited[v] && res_graph[vert][v] > 0)
            {
                if (v == out_vertex)
                {
                    parent[v] = vert;
                    return true;
                }

                queue.push(v);
                parent[v] = vert;
                visited[v] = true;
            }
        }
    }

    return false;
}

void ford_fulkerson(graph& g, std::stringstream& out_stream, int start_vertex, int out_vertex, bool setted_solution)
{
    (void)setted_solution;
    
    constexpr int INF = std::numeric_limits<int>::max();

    const auto vert_size = g.V.size();
    matrix residual_flux (vert_size, std::vector<int>(vert_size, 0));
   
    for (size_t i = 1; i < vert_size; i++)
    {
        for (const auto& value: g.edges[i])
        {
            residual_flux[i][value.first] = value.second;
        }
    }

    std::vector<int> parents (vert_size);

    int max_flow = 0;

    while (find_paths_bfs(residual_flux, start_vertex, out_vertex, parents))
    {
        int u, v;

        // Essa parte calcula o gargalo do caminho
        auto path_flow = INF;
        for (v = out_vertex; v != start_vertex; v = parents[v]) {
            u = parents[v];
            path_flow = std::min(path_flow, residual_flux[u][v]);
        }
 
        // Essa parte atualiza os fluxos da parte residual
        for (v = out_vertex; v != start_vertex; v = parents[v]) {
            u = parents[v];
            residual_flux[u][v] -= path_flow;
            residual_flux[v][u] += path_flow;
        }

        max_flow += path_flow;
    }

    if (!setted_solution)
    {
        out_stream << max_flow << '\n';
    }
    else
    {
        for (size_t i = 1; i < vert_size; i++)
        {
            for (size_t j = 1; j < vert_size; j++)
            {
                const auto val = residual_flux[i][j];
                if (val != 0)
                {
                    out_stream << '(' << i << ',' << j << ") " << val << '\n';
                }
            }
        }   
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

    if (params.out_vertex == 0 || params.in_vertex == 0)
    {
        std::cout << "Need to type input and output vertex.\nType -h for usage.";
        return -1;
    }

    ford_fulkerson(G, out_stream, params.in_vertex, params.out_vertex, params.setted_solution);

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