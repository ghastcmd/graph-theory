#include <iostream>
#include <vector>
#include <limits>
#include <string>
#include <sstream>

#include "../include/graph.hpp"
#include "../include/pqueue.hpp"

void dijkstra(graph g, std::stringstream& out_stream, int start_vertex, int out_vert, bool setted_solution)
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
-s           : mostra a solução (em ordem crescente)
-i <vertice> : vértice inicial (dependendo do algoritmo)
-l <vertice> : vértice final (dependendo do algoritmo)
    )");

    std::cout << "O uso do programa é:\n" << argv[0] << help;
}

void usage_error(int argc, char **argv)
{
    std::cout << "Uso inapropriado do programa.\nDigite" << argv[0] << "-h para saber mais.\n";
}

bool str_opt_ncmp(const char *in_str, const char *cmp_str, size_t n)
{
    for (size_t i = 0; i < n; i++)
    {
        if (in_str[i] != cmp_str[i])
        {
            return false;
        }
    }
    return true;
}

enum class types_of_args
{
    help,
    input,
    output,
    solution,
    in_vertex,
    out_vertex
};

void parse_args(std::vector<std::pair<types_of_args, std::string>>& result, int argc, char **argv)
{
    auto check_error = [&](int index) {
        if (index > argc)
        {
            usage_error(argc, argv);
            std::exit(-1);
        }
    };

    for (size_t i = 1; i < argc; i++)
    {
        if (str_opt_ncmp(argv[i], "-h", 3))
        {
            result.push_back({types_of_args::help, ""});
        }
        else if (str_opt_ncmp(argv[i], "-f", 3))
        {
            check_error(i + 1);
            result.push_back({types_of_args::input, argv[++i]});
        }
        else if (str_opt_ncmp(argv[i], "-o", 3))
        {
            check_error(i + 1);
            result.push_back({types_of_args::output, argv[++i]});
        }
        else if (str_opt_ncmp(argv[i], "-s", 3))
        {
            result.push_back({types_of_args::solution, ""});
        }
        else if (str_opt_ncmp(argv[i], "-i", 3))
        {
            check_error(i + 1);
            result.push_back({types_of_args::in_vertex, argv[++i]});
        }
        else if (str_opt_ncmp(argv[i], "-l", 3))
        {
            check_error(i + 1);
            result.push_back({types_of_args::out_vertex, argv[++i]});
        }
    }
}

struct program_parameters
{
    void get_paramers(int argc, char **argv);

    std::string file_input_path;
    std::string file_output_path;
    bool setted_file_input = false;
    bool setted_file_output = false;
    bool setted_solution = false;
    int in_vertex = 1;
    int out_vertex = 0;
};

void program_parameters::get_paramers(int argc, char **argv)
{
    std::vector<std::pair<types_of_args, std::string>> parsed_args;
    parse_args(parsed_args, argc, argv);

    std::sort(parsed_args.begin(), parsed_args.end());

    for (const auto& value: parsed_args)
    {
        switch (value.first)
        {
            case types_of_args::help:
                usage(argc, argv);
                exit(0);
            break;
            case types_of_args::input: {
                file_input_path = std::move(value.second);
                setted_file_input = true;
            } break;
            case types_of_args::output: {
                file_output_path = std::move(value.second);
                setted_file_output = true;
            } break;
            case types_of_args::solution: {
                setted_solution = true;
            } break;
            case types_of_args::in_vertex: {
                in_vertex = std::stoi(value.second);
            } break;
            case types_of_args::out_vertex: {
                out_vertex = std::stoi(value.second);
            } break;
        }
    }
}

int main(int argc, char **argv)
{
    program_parameters params;

    params.get_paramers(argc, argv);

    graph G;

    if (params.setted_file_input)
    {
        std::ifstream in_file (params.file_input_path);
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
        out_file << out_stream.rdbuf();
    }
    else
    {
        std::cout << out_stream.rdbuf();
    }

    return 0;
}