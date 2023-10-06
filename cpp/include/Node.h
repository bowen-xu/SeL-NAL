#ifndef _BODHI_NODE_H
#define _BODHI_NODE_H

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <vector>
#include <memory>
#include <string>
#include <fmt/core.h>
#include "Narsese/include/Narsese.h"
// #include "NAL/Functions/include/TemporalFunctions.h"
#include "utils/hash.h"
#include <unordered_map>
#include <unordered_set>
#include "./Global.h"
#include "./LSTB.h"
#include <tuple>
#include <cmath>

namespace BUNDLE
{
    using std::shared_ptr;
    class Bundle;
    typedef shared_ptr<Bundle> pBundle;

    class Hash_pT;
}
namespace LINK
{
    using std::shared_ptr;
    class Link;
    typedef shared_ptr<Link> pLink;
}

namespace GROUP
{
    using std::shared_ptr;
    class Group;
    typedef shared_ptr<Group> pGroup;
}

namespace COLUMN
{
    using std::shared_ptr;
    class Column;
    typedef shared_ptr<Column> pColumn;
}

namespace NODE
{
    // using BUDGET::Budget;
    // using BUDGET::pBudget;
    // using SENTENCE::pStamp;
    // using SENTENCE::Stamp;
    // using std::string;
    using TERM::pTerm;
    using TERM::Term;
    // using TRUTH::pTruth;
    // using TRUTH::Truth;

    using BUNDLE::Bundle;
    using BUNDLE::pBundle;
    using COLUMN::Column;
    using COLUMN::pColumn;
    using LINK::pLink;

    using UTILS::Hash;
    using UTILS::hash;

    using std::shared_ptr;
    using std::unordered_map;
    using std::unordered_set;
    using std::vector;

    using GROUP::Group;
    using LSTB::LSTBuffer;

    using BUNDLE::Hash_pT;

    namespace py = pybind11;

    class Node;
    typedef shared_ptr<Node> pNode;

    // class Hash_pT
    // {
    // public:
    //     template <typename _pT>
    //     size_t operator()(const _pT &key) const;
    // };

    class PYBIND11_EXPORT Node
    {
    public:
        std::weak_ptr<Group> group;
        std::weak_ptr<Column> column;

        pTerm term;

        int64_t ts_update = -1;
        vector<bool> a;
        vector<bool> r; // retrospection or tentative activation
        vector<bool> p;

        double activty = 0.0;

        pBundle ante_links;
        pBundle post_links;

        void roll_state();
        void reset_state();

        void update(int64_t ts_now);

        double ambiguity();
        double ambiguity(const unordered_map<pNode, pLink, Hash_pT> &links, double amplifier);
        double utility();

        static pNode create(std::string word, size_t n_ltb = 100, size_t n_stb = 100);
        static pNode create(size_t n_ltb = 100, size_t n_stb = 100);

        Node(size_t n_ltb = 100, size_t n_stb = 100);
        Node(std::string word, size_t n_ltb = 100, size_t n_stb = 100);

        bool contains_antelink(const pLink &link) const;
        bool contains_antelink(const pNode &node) const;
        bool contains_postlink(const pLink &link) const;
        bool contains_postlink(const pNode &node) const;
        bool remove_antelink(const pLink &link);
        bool remove_antelink(const pNode &node);
        bool remove_postlink(const pLink &link);
        bool remove_postlink(const pNode &node);
        std::pair<pLink, pLink> insert_antelink(const pLink &link);
        std::pair<pLink, pLink> insert_postlink(const pLink &link);

        static pLink connect(pNode node1, pNode node2, double pv = 0.3);

        size_t __hash__() const
        {
            return Hash{}((size_t)this);
        }

        std::string __repr__(void *interpreter) const;
        std::string __str__(void *interpreter) const;
    };

    void pybind_node(py::module &m);

} // namespace NODE

#endif // _BODHI_NODE_H