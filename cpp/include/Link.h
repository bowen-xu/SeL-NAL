#ifndef _LINK_H
#define _LINK_H

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <fmt/core.h>
#include <vector>
#include <memory>
#include <string>
// #include "./Node.h"
#include "Narsese/include/Narsese.h"

namespace NODE
{
    using std::shared_ptr;
    class Node;
    typedef shared_ptr<Node> pNode;
}

namespace GROUP
{
    using std::shared_ptr;
    class Group;
    typedef shared_ptr<Group> pGroup;
}

namespace LINK
{
    namespace py = pybind11;

    // using BUDGET::Budget;
    // using BUDGET::pBudget;
    // using SENTENCE::pStamp;
    // using SENTENCE::Stamp;
    // using TERM::pTerm;
    // using TERM::Term;
    using TRUTH::pTruth;
    using TRUTH::Truth;

    using NODE::pNode;
    using std::shared_ptr;
    using std::vector;

    using GROUP::Group;

    class Link;
    typedef shared_ptr<Link> pLink;

    class PYBIND11_EXPORT Link
    {
    public:
        std::weak_ptr<Group> group;

        int64_t ts_activate = -1;
        pTruth truth = Truth::create(1.0, 0.1, 1.0); // truth for predictive-equivalence

        pNode ante_node;
        pNode post_node;


        Link(pNode node1, pNode node2);

        size_t __hash__();

        double ambiguity() const;

        double pv() const; // Perfomance Value

        void revise(double w_p, double w_m);
        void revise(Truth& truth);

        static pLink create(pNode node1, pNode node2);

        std::string __repr__(void *interpreter);
    };

    void pybind_link(py::module &m);

} // namespace LINK

#endif // _LINK_H