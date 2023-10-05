#ifndef _BUNDLE_H
#define _BUNDLE_H

#include <unordered_map>
#include <unordered_set>
#include <set>
#include "utils/hash.h"
#include "./LSTB.h"
#include <memory>
#include <tuple>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <queue>
#include "Narsese/include/Narsese.h"

namespace COLUMN
{
    using std::shared_ptr;
    class Column;
    typedef shared_ptr<Column> pColumn;
}

namespace NODE
{
    using std::shared_ptr;
    class Node;
    typedef shared_ptr<Node> pNode;
}

namespace LINK
{
    using std::shared_ptr;
    class Link;
    typedef shared_ptr<Link> pLink;
}

namespace BUNDLE
{
    using COLUMN::pColumn;
    using LINK::pLink;
    using NODE::Node;
    using NODE::pNode;

    using UTILS::Hash;
    using UTILS::hash;

    using std::map;
    using std::priority_queue;
    using std::set;
    using std::shared_ptr;
    using std::unordered_map;
    using std::unordered_set;
    using std::vector;

    using LSTB::LSTBuffer;
    namespace py = pybind11;

    class Hash_pT
    {
    public:
        template <typename _pT>
        size_t operator()(const _pT &key) const;
    };

    class _ComparePv
    {
    public:
        bool operator()(const pLink &lhs, const pLink &rhs) const
        {
            return lhs->pv() > rhs->pv();
        }
    };

    class PYBIND11_EXPORT Bundle
    {
    public:
        Node *node;
        unordered_map<pNode, pLink, Hash_pT> links;
        map<pLink, pNode, _ComparePv> links_sorted;
        unordered_map<pColumn, unordered_set<pLink>, Hash_pT> links_col;
        LSTBuffer<pLink> links_lstb;

        Bundle(Node *node, size_t n_ltb, size_t n_stb) : node(node), links_lstb(LSTBuffer<pLink>(n_ltb, n_stb)) {}

        void set_capacity(size_t n_ltb, size_t n_stb);
        
        pLink topmost();
        pLink subtop();

        bool remove(const pLink &link);
        std::pair<pLink, pLink> insert(const pLink &link, const pNode &node);
        void update(pLink &link, TRUTH::Truth& truth);
        bool contains(const pNode &node) const;
        bool contains(const pLink &link) const;
        bool size() const;

        pLink &operator[](const pNode &node);
    };

    typedef std::shared_ptr<Bundle> pBundle;
    void pybind_bundle(py::module &m);
} // namespace BUNDLE

#endif // _BUNDLE_H