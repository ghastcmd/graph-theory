#pragma once

#include <fstream>
#include <istream>
#include <vector>

template <typename _ty_size>
_ty_size get_numbers_from_str(const char *str, const char **ret_ptr, _ty_size max)
{
    auto not_space = [](char ch) {
        return ch != ' ' && ch != '\n' && ch != '\r';
    };

    auto is_space = [](char ch) {
        return ch == ' ' || ch == '\n' || ch == '\r';
    };

    _ty_size count = 0;
    if (not_space(*str))
    {
        ret_ptr[count] = str;
        count += 1;
        if (count == max) return count;
    }

    char c;
    bool space_before = false;
    while ((c = *str++))
    {
        if (not_space(c) && space_before)
        {
            ret_ptr[count] = str-1;
            count += 1;
            if (count == max) return count;
        }

        space_before = is_space(c);
    }

    // setting the rest of ret_ptr to empty string
    for (int i = count; i < max && (ret_ptr[i] = ""); i++);

    return count;
}

template <size_t size>
inline constexpr void _assign(const char *n_str[size], size_t index, int& value)
{
    value = std::atoi(n_str[index]);
}

template <size_t size, typename _ty, typename... _args>
inline constexpr void
_assign(const char *n_str[size], size_t index, _ty& value, _args&... args)
{
    return (_assign<size>(n_str, index, value), _assign<size>(n_str, index + 1, args...));
}

template <size_t size, typename... _args>
inline constexpr
void assign(const char *n_str[size], _args&... args)
{
    return (_assign<size>(n_str, 0, args...));
}

template <size_t size, typename... _args>
int parse_ints(const char *to_parse, _args&... args)
{
    const char *ret_str[size];
    auto count = get_numbers_from_str(to_parse, ret_str, size);

    assign<size>(ret_str, args...);
    return count;
}

template <typename _ty>
struct is_stream {
    static constexpr bool value = false;
};

template <>
struct is_stream<std::istream> {
    static constexpr bool value = true;
};

template <>
struct is_stream<std::ifstream> {
    static constexpr bool value = true;
};

template <>
struct is_stream<std::fstream> {
    static constexpr bool value = true;
};

class graph
{
public:
    graph() {}

    template <typename _stream>
    void from_file_stream(_stream& in_stream);

    const size_t size() const;
    
public:
    std::vector<int> V;
    std::vector<std::vector<std::pair<int, int>>> edges;
};

template <typename _stream>
void graph::from_file_stream(_stream& in_stream)
{
    static_assert(is_stream<_stream>::value, "Invalid stream type");

    constexpr size_t ibuffer_len = 64;

    char input_line[ibuffer_len];
    in_stream.getline(input_line, ibuffer_len);

    int num_vertex, num_edges;
    parse_ints<2>(input_line, num_vertex, num_edges);

    V.resize(num_vertex + 1);
    edges.resize(num_edges + 1);

    for (size_t i = 1; i <= num_vertex; i++)
    {
        V[i] = i;
    }

    for (size_t i = 0; i < num_edges; i++)
    {
        int edge1, edge2, weight;

        in_stream.getline(input_line, ibuffer_len);
        auto count = parse_ints<3>(input_line, edge1, edge2, weight);

        if (count != 3)
        {
            weight = 1;
        }

        edges[edge1].push_back(std::pair<int, int>{edge2, weight});
    }
}

const size_t graph::size() const
{
    return V.size();
}
