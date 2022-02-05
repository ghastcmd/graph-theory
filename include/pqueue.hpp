#pragma once

#include <vector>
#include <deque>
#include <algorithm>
#include <utility>

template <typename _ty>
class pqueue
{
public:
    pqueue(std::vector<_ty> values, std::vector<_ty> weights);

    void set_priority(size_t index, int new_value);

    _ty front() const;

    _ty pop_front();

    bool empty() const;
private:
    std::deque<std::pair<int, int>> m_values;
};

template <typename _ty>
pqueue<_ty>::pqueue(std::vector<_ty> values, std::vector<_ty> weights)
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

template <typename _ty>
void pqueue<_ty>::set_priority(size_t index, int new_value)
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

template <typename _ty>
_ty pqueue<_ty>::front() const
{
    return m_values.front().second;
}

template <typename _ty>
_ty pqueue<_ty>::pop_front()
{
    _ty element = front();
    m_values.pop_front();
    return element;
}

template <typename _ty>
bool pqueue<_ty>::empty() const
{
    return m_values.empty();
}