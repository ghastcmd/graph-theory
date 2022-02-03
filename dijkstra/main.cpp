#include <iostream>
#include <vector>
#include <limits>
#include <utility>
#include <queue>

#include <deque>
#include <algorithm>
#include <sstream>
#include <string>

class graph
{
public:
    graph(size_t vertices_size, std::vector<std::pair<int, int>> in_edges, std::vector<int> weights)
    {
        V.resize(vertices_size);
        for (size_t i = 0; i < vertices_size; i++)
        {
            V[i] = i;
        }

        for (size_t i = 0; i < in_edges.size(); i++)
        {
            const auto& vv = in_edges[i];
            const auto& weight = weights[i];
            edges[vv.first].emplace_back(std::pair<int, int>{vv.second, weight});
        }
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

constexpr int INF = std::numeric_limits<int>::max();


void dijkstra(graph g, int start_vertex)
{
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
        auto u = queue.pop_front(); // acessa o primeiro item da fila
        
        for (auto v: g.edges[u])
        {
            auto ndist = dist[u] + v.second;
            if (dist[v.first] > ndist)
            {
                dist[v.first] = ndist;
                prev[v.first] = u;
                queue.set_priority(v.first, dist[v.first]);
            }
        }
    }
}

void print_deque(std::deque<std::pair<int, int>> queue)
{
    std::stringstream first_line;
    std::stringstream second_line;
    while (!queue.empty())
    {
        auto vv = queue.front();
        queue.pop_front();
        first_line << vv.first << ' ';
        second_line << vv.second << ' ';
    }
    std::cout << first_line.str() << '\n' << second_line.str() << '\n'; 
}


template <typename AR = std::pair<int, int>>
int compar(AR first_pair, AR second_pair)
{
    return first_pair.second <= second_pair.second;
}

bool is_space(char ch)
{
    return ch == ' ';
}

size_t get_numbers_from_str(const char *str, const char **ret_ptr, size_t max)
{
    auto not_space = [](char ch) {
        return ch != ' ' && ch != '\n' && ch != '\r';
    };

    size_t count = 0;
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

template <size_t size, typename _ty, typename ..._args>
inline constexpr void
_assign(const char *n_str[size], size_t index, _ty& value, _args&... args)
{
    return (_assign<size>(n_str, index, value), _assign<size>(n_str, index + 1, args...));
}

template <size_t size, typename ... _args>
inline constexpr
void assign(const char *n_str[size], _args& ... args)
{
    return (_assign<size>(n_str, 0, args...));
}

template <size_t size, typename ..._args>
int parse_ints(const char *to_parse, _args&... args)
{
    const char *ret_str[size] {0};
    auto count = get_numbers_from_str(to_parse, ret_str, size);
    if (count > size)
    {
        std::cout << "Too many numbers\n";
    }
    puts("before the recursive call");
    assign<size>(ret_str, args...);
    return count;
}

int main(int argc, char **argv)
{
    char input_line[32];
    
    const char *ret_str[3];
    std::cin.getline(input_line, 32);

    int num_vertex, len_edges;
    parse_ints<2>(input_line, num_vertex, len_edges);

    std::vector<std::pair<int, int>> edges (len_edges);
    std::vector<int> weights (len_edges);

    for (size_t i = 0; i < len_edges; i++)
    {
        auto& weight = weights[i];
        auto& vertice = edges[i];

        std::cin.getline(input_line, 32);
        
        const char *ret_str[3];
        auto count = parse_ints<3>(input_line, vertice.first, vertice.second, weight);

        puts("this the shits");

        if (count != 3)
        {
            weight = 1;
        }
    }

    for (auto& val: edges)
    {
        printf("%i %i\n", val.first, val.second);
    }

    for (auto& val: weights)
    {
        std::cout << val << '\n';
    }

    // graph G((size_t)num_vertex, edges, weights);

    // dijkstra(G, 1);

    return 0;
}