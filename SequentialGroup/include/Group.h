#ifndef _GROUP_H
#define _GROUP_H

#include <pybind11/pybind11.h>
#include "Column.h"
#include "Column.inl"
#include "./Buffer.h"
// #include "Narsese/include/Truth.h"
#include <vector>
#include <string>
#include <memory>
#include <set>
#include <fmt/core.h>
#include <random>

namespace GROUP
{
    namespace py = pybind11;
    // using BUDGET::Budget;
    // using BUDGET::pBudget;
    // using TRUTH::Truth;
    // using TRUTH::pTruth;
    // using TERM::Term;
    // using TERM::pTerm;
    // using SENTENCE::Stamp;
    // using SENTENCE::pStamp;
    using NODE::pNode;
    using std::shared_ptr;
    using std::vector;
    // using NODE::Node;
    // using LINK::pLink;
    // using LINK::Link;
    // using COLUMN::Column;
    using COLUMN::pColumn;

    static std::random_device rd;
    static std::mt19937 gen(rd()); // 使用Mersenne Twister 19937引擎

    

    class Group;
    typedef shared_ptr<Group> pGroup;
    class PYBIND11_EXPORT Group
    {
    public:
        size_t n_columns = 0;
        size_t n_nodes = 1;
        vector<pColumn> columns;

        double mean_pv = 0.3;
        double std_pv = 0.01;

        std::normal_distribution<double> distribution_pv;

        std::set<pNode> buffer{};
        std::set<pNode> buffer2{};

        std::list<pNode> buffer_h; /* Buffer for building hypotheses */

        pColumn last_activaetd_column = nullptr;

        size_t ts = 0;

        double thresh = 0.51;

        double p_plus = 0.06;
        double p_minus = 0.033;
        double p_minus2 = 0.008;
        

        Group(size_t n_columns, size_t n_nodes);

        static pGroup create(size_t n_columns, size_t n_nodes)
        {
            auto group = pGroup(new Group(n_columns, n_nodes));
            for (auto column : group->columns)
            {
                column->group = group;
                for (auto node : column->nodes)
                {
                    node->group = group;
                    node->ts_update = group->ts;
                }
            }

            return group;
        }

        static void build_links(vector<pNode> &&ante_nodes, vector<pNode> &&post_nodes);
        static void build_links(vector<pNode> &&ante_nodes, vector<pNode> &post_nodes);
        static void build_links(vector<pNode> &ante_nodes, vector<pNode> &&post_nodes);

        template <typename _Container>
        static void build_links(_Container &ante_nodes, _Container &post_nodes);

        /*
         * To activate a certian column
         */
        std::set<pNode> &activate(size_t i_column); /* To activate a certian node */
        void learn(size_t i_column);

        std::string __repr__()
        {
            return fmt::format("<Group ({} x {})>", this->n_columns, this->n_nodes);
        }
    };

    void pybind_group(py::module &m) __attribute__((used));

} // namespace GROUP
#endif // _GROUP_H
