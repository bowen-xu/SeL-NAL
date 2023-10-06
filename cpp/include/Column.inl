#ifndef _COLUMN_INL
#define _COLUMN_INL

#include "Column.h"

#include "Link.inl"
#include "Node.inl"
#include <numeric>

namespace COLUMN
{
    inline Column::Column(size_t n)
    {
        for (auto i = 0; i < n; i++)
        {
            nodes.push_back(Node::create(100, 100));
        }
    }

    inline pColumn Column::create(size_t n)
    {
        auto column = pColumn(new Column(n));
        for (auto &node : column->nodes)
            node->column = column;

        return column;
    }

    // inline vector<pNode> Column::select_nodes_by_activity(double expense, double amplifier)
    // {
    //     vector<pNode> winners;
    //     auto &&values = this->get_activities(amplifier);

    //     std::vector<size_t> indices(values.size());
    //     std::iota(indices.begin(), indices.end(), 0);
    //     std::sort(indices.begin(), indices.end(), [&values](size_t i1, size_t i2)
    //               { return values[i1] > values[i2]; });
    //     auto _expense = expense;
    //     for (auto i : indices)
    //     {
    //         winners.push_back(this->nodes[i]);
    //         _expense -= values[i];
    //         if (_expense <= 0)
    //             break;
    //     }
    //     return winners;
    // }

    inline vector<std::pair<double, pNode>> Column::select_nodes_by_activity(double expense, double amplifier)
    {
        vector<std::pair<double, pNode>> winners;
        auto &&values = this->get_activities(amplifier);

        std::vector<size_t> indices(values.size());
        std::iota(indices.begin(), indices.end(), 0);
        std::sort(
            indices.begin(), indices.end(),
            [&values](size_t i1, size_t i2)
            { return values[i1] > values[i2]; });
        auto _expense = expense;
        for (auto i : indices)
        {
            winners.push_back({values[i], this->nodes[i]});
            _expense -= values[i];
            if (_expense <= 0)
                break;
        }
        return winners;
    }

    inline std::vector<double> Column::get_ambiguities()
    {
        std::vector<double> values;
        std::transform(this->nodes.begin(), this->nodes.end(), std::back_inserter(values), [](const pNode &node)
                       { return node->ambiguity(); });
        return values;
    }

    inline std::vector<pNode> Column::select_nodes_by_ambiguity()
    {
        auto &&values = this->get_ambiguities();
        auto minmax_values = std::minmax_element(values.begin(), values.end());
        auto idx_min = std::distance(values.begin(), minmax_values.first);
        auto idx_max = std::distance(values.begin(), minmax_values.second);
        vector<pNode> winners{this->nodes[idx_min], this->nodes[idx_max]};
        return winners;
    }

    inline size_t Column::__hash__() const
    {
        return Hash{}((size_t)this);
    }
} // namespace COLUMN

#endif // _COLUMN_INL