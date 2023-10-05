#ifndef _LINK_INL
#define _LINK_INL

#include "./Link.h"
#include <algorithm>
// #include "functions/include/ExtendedBooleanFunctions.hpp"
#include "NAL/Functions/include/TruthFunctions.h"
#include "NAL/Functions/include/UncertaintyMappingFunctions.h"
// #include "NAL/Functions/include/ExtendedBooleanFunction.h"
#include "./Global.h"
#include "Narsese/include/Narsese.h"

namespace LINK
{
    // using BODHI::FUNCTIONS::And;
    // using BOOLEAN_FUNCTIONS::And;
    // using TRUTH_FUNCTIONS::truth_deduction;
    // using TRUTH_FUNCTIONS::truth_induction;
    // using TRUTH_FUNCTIONS::truth_revision;
    namespace UMF = UNCERTAINTY_MAPPING_FUNCTIONS;

    inline Link::Link(pNode node1, pNode node2)
    {
        this->ante_node = node1;
        this->post_node = node2;
    }

    inline double Link::ambiguity() const
    {
        auto a = 1 - this->pv();
        return a;
    }

    inline double Link::pv() const
    {
        return this->truth->e();
    }

    inline void Link::revise(double w_p, double w_m)
    {
        auto truth = UMF::truth_from_w(w_p, w_p+w_m, 1);
        this->revise(truth);
    }

    inline void Link::revise(Truth& truth)
    {
        TRUTH_FUNCTIONS::truth_revision(*this->truth, truth);
    }

    inline pLink Link::create(pNode node1, pNode node2)
    {
        return pLink(new Link(node1, node2));
    }

    inline size_t Link::__hash__()
    {
        using UTILS::Hash;
        using UTILS::hash;
        return Hash{}({hash(*this->ante_node), hash(*this->post_node)});
    }

} // namespace LINK

#endif // _LINK_INL