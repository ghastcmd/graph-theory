#pragma once

#include <string>
#include <iostream>
#include <vector>
#include <utility>
#include <algorithm>
#include <functional>

void usage_error(int argc, char **argv)
{
    std::cout << "Uso inapropriado do programa.\nDigite " << argv[0] << " -h para saber mais.\n";
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
    error,
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
        else
        {
            result.push_back({types_of_args::error, ""});
            return;
        }
    }
}

struct program_parameters
{
    void get_paramers(int argc, char **argv, std::function<void(int, char**)> usage);

    std::string file_input_path;
    std::string file_output_path;
    bool setted_file_input = false;
    bool setted_file_output = false;
    bool setted_solution = false;
    int in_vertex = 1;
    int out_vertex = 0;
};

void program_parameters::get_paramers(int argc, char **argv, std::function<void(int, char**)> usage)
{
    std::vector<std::pair<types_of_args, std::string>> parsed_args;
    parse_args(parsed_args, argc, argv);

    std::sort(parsed_args.begin(), parsed_args.end());

    for (const auto& value: parsed_args)
    {
        switch (value.first)
        {
            case types_of_args::error:
                usage_error(argc, argv);
                exit(-1);
            break;
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