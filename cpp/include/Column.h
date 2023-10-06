#ifndef _COLUMN_H
#define _COLUMN_H
#include "Link.h"
#include "Node.h"

#include <pybind11/pybind11.h>
#include <vector>
#include <memory>
#include <string>
#include <fmt/core.h>
#include "helpers.hpp"
#include <algorithm>
#include "utils/hash.h"

namespace GROUP
{
    using std::shared_ptr;
    class Group;
    typedef shared_ptr<Group> pGroup;

}

namespace COLUMN
{
    namespace py = pybind11;

    // using BUDGET::Budget;
    // using BUDGET::pBudget;
    // using SENTENCE::pStamp;
    // using SENTENCE::Stamp;
    // using TERM::pTerm;
    // using TERM::Term;
    // using TRUTH::pTruth;
    // using TRUTH::Truth;

    using std::shared_ptr;
    using std::vector;

    using GROUP::Group;
    using LINK::Link;
    using LINK::pLink;
    using NODE::Node;
    using NODE::pNode;

    using UTILS::Hash;
    using UTILS::hash;

    class Column;
    typedef shared_ptr<Column> pColumn;

    class PYBIND11_EXPORT Column
    {
    public:
        std::weak_ptr<Group> group;

        vector<pNode> nodes;

        int64_t mark=0;

        Column(size_t n);

        static pColumn create(size_t n);

        std::vector<double> get_activities(double amplifier = 1.0, double expense = 0.5);

        vector<std::pair<double, pNode>> select_nodes_by_activity(double expense = 0.5, double amplifier = 1.0);

        std::vector<double> get_ambiguities();

        std::vector<pNode> select_nodes_by_ambiguity();

        std::string __repr__()
        {
            return fmt::format("<Column ({})>", this->nodes.size());
        }

        size_t __hash__() const;
    };

    void pybind_column(py::module &m);

} // namespace COLUMN

#endif // _COLUMN_H