#include <iostream>
#include <vector>
#include <limits>
#include <utility>
#include <deque>
#include <algorithm>
#include <fstream>
#include <string>
#include <array>

template <typename _ty_size>
_ty_size get_numbers_from_str(const char *str, const char **ret_ptr, _ty_size max)
{
    auto not_space = [](char ch) {
        return ch != ' ' && ch != '\n' && ch != '\r';
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

        space_before = c == ' ';
    }

    return count;
}

template <size_t size>
inline constexpr void _assign(const char *n_str[size], size_t index, int& value)
{
    value = atoi(n_str[index]);
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
    
    for (int i = count; i < size && (ret_str[i] = ""); i++);

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
    void from_file_stream(_stream& in_stream)
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

    const size_t size() const
    {
        return V.size();
    }
public:
    std::vector<int> V;
    std::vector<std::vector<std::pair<int, int>>> edges;
};

template <typename _ty>
class pqueue
{
public:
    pqueue(std::vector<_ty> values, std::vector<_ty> weights)
    {
        if (values.size() != weights.size())
        {
            std::cout << "Invalid sizes for values and weights vector\n";
        }

        m_values.resize(values.size());

        for (size_t i = 0; i < m_values.size(); i++)
        {
            m_values[i].first = weights[i];
            m_values[i].second = values[i];
        }

        std::sort(m_values.begin(), m_values.end());
    }

    void set_priority(size_t index, int new_value)
    {
        for (auto& vv: m_values)
        {
            if (vv.second == index)
            {
                vv.first = new_value;
                break;
            }
        }

        std::sort(m_values.begin(), m_values.end());
    }

    _ty front()
    {
        return m_values.front().second;
    }

    _ty pop_front()
    {
        _ty element = front();
        m_values.pop_front();
        return element;
    }

    bool empty()
    {
        return m_values.empty();
    }
private:
    std::deque<std::pair<int, int>> m_values;
};

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

// int main()
// {
//     const char *str = "";

//     int a = atoi(str);

//     printf("%i\n", a);
// }

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
}