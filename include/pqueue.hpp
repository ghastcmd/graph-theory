#pragma once

#include <vector>
#include <deque>
#include <algorithm>
#include <utility>

template <typename _ty>
class pqueue
{
public:
    using iterator = typename std::deque<std::pair<int, int>>::iterator;

    pqueue(std::vector<_ty>& values, std::vector<_ty>& weights);

    void set_priority(size_t index, int new_value);

    _ty front() const;

    _ty pop_front();

    iterator begin();

    iterator end();

    bool empty() const;
private:

    std::deque<std::pair<int, int>> m_values;
};

template <typename _ty>
pqueue<_ty>::pqueue(std::vector<_ty>& values, std::vector<_ty>& weights)
{
    if (values.size() != weights.size())
    {
        std::cout << "Invalid sizes for values and weights vector\n";
    }

    m_values.resize(values.size() - 1);

    for (size_t i = 1, max = values.size(); i < max; i++)
    {
        m_values[i-1].first = weights[i];
        m_values[i-1].second = values[i];
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
typename pqueue<_ty>::iterator pqueue<_ty>::begin()
{
    return m_values.begin();
}

template <typename _ty>
typename pqueue<_ty>::iterator pqueue<_ty>::end()
{
    return m_values.end();
}

template <typename _ty>
bool pqueue<_ty>::empty() const
{
    return m_values.empty();
}