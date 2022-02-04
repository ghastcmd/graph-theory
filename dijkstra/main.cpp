#include <iostream>
#include <vector>
#include <limits>
#include <utility>
#include <deque>
#include <algorithm>

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
    const char *ret_str[size] {0};
    auto count = get_numbers_from_str(to_parse, ret_str, size);
    if (count > size)
    {
        std::cout << "Too many numbers\n";
    }
    assign<size>(ret_str, args...);
    return count;
}

class graph
{
public:
    graph(size_t vertices_len, std::vector<std::pair<int, int>> in_edges, std::vector<int> weights)
        : V(vertices_len+1)
    {
        for (size_t i = 1; i <= vertices_len; i++)
        {
            V[i] = i;
        }

        for (size_t i = 1; i < in_edges.size(); i++)
        {
            const auto& vv = in_edges[i-1];
            const auto& weight = weights[i-1];
            edges[vv.first].emplace_back(std::pair<int, int>{vv.second, weight});
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