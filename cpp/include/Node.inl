#ifndef _NODE_INL
#define _NODE_INL

#include "./Node.h"
#include "./Link.h"
#include <pybind11/pybind11.h>
#include <algorithm>
#include "helpers.hpp"
#include "./Global.h"
#include "LSTB.h"
#include "LSTB.inl"
#include "Column.h"
#include "Column.inl"
#include "Bundle.h"
#include "Bundle.inl"
#include "NAL/Functions/include/ExtendedBooleanFunction.h"

namespace NODE
{
    inline Node::Node(size_t n_ltb, size_t n_stb) : ante_links(pBundle(new Bundle(this, n_ltb, n_stb))), post_links(pBundle(new Bundle(this, n_ltb, n_stb)))
    {
        // this->term = Term::create();

        a = std::vector<bool>{false, false};
        p = std::vector<bool>{false, false};
        r = std::vector<bool>{false, false};

        this->ante_links->set_capacity(n_ltb, n_stb);
        this->post_links->set_capacity(n_ltb, n_stb);
    }

    inline Node::Node(std::string word, size_t n_ltb, size_t n_stb) : ante_links(pBundle(new Bundle(this, n_ltb, n_stb))), post_links(pBundle(new Bundle(this, n_ltb, n_stb)))
    {
        // this->term = Term::create(word.c_str());

        this->ante_links->set_capacity(n_ltb, n_stb);
        this->post_links->set_capacity(n_ltb, n_stb);
    }

    inline void Node::update(int64_t ts_now)
    {
        if (this->ts_update == ts_now - 1)
            this->roll_state();
        else if (this->ts_update < ts_now - 1)
            this->reset_state();
        this->ts_update = ts_now;
    }

    inline void Node::roll_state()
    {
        this->a[0] = this->a[1];
        this->a[1] = false;

        this->p[0] = this->p[1];
        this->p[1] = false;

        this->r[0] = this->r[1];
        this->r[1] = false;
    }

    inline void Node::reset_state()
    {
        std::fill(this->a.begin(), this->a.end(), false);
        std::fill(this->p.begin(), this->p.end(), false);
        std::fill(this->r.begin(), this->r.end(), false);
    }

    inline bool Node::contains_antelink(const pLink &link) const
    {
        return link->post_node.get() == this && this->ante_links->contains(link->ante_node);
    }

    inline bool Node::contains_antelink(const pNode &node) const
    {
        return this->ante_links->contains(node);
    }

    inline bool Node::contains_postlink(const pLink &link) const
    {
        return link->ante_node.get() == this && this->post_links->contains(link->post_node);
    }

    inline bool Node::contains_postlink(const pNode &node) const
    {
        return this->post_links->contains(node);
    }

    inline bool Node::remove_antelink(const pLink &link)
    {
        this->ante_links->remove(link);
        link->ante_node->remove_postlink(link);
        return true;
    }
    inline bool Node::remove_antelink(const pNode &node)
    {
        if (!this->ante_links->contains(node))
            return false;
        auto &link = (*this->ante_links)[node];
        this->remove_antelink(link);
        return true;
    }

    inline bool Node::remove_postlink(const pLink &link)
    {
        this->post_links->remove(link);
        link->post_node->remove_antelink(link);
        return true;
    }

    inline bool Node::remove_postlink(const pNode &node)
    {
        if (!this->post_links->contains(node))
            return false;
        auto &link = (*this->post_links)[node];
        this->remove_postlink(link);
        return true;
    }

    inline std::pair<pLink, pLink> Node::insert_antelink(const pLink &link)
    {
        auto &node = link->ante_node;
        return this->ante_links->insert(link, node);
    }

    inline std::pair<pLink, pLink> Node::insert_postlink(const pLink &link)
    {
        auto &node = link->post_node;
        return this->post_links->insert(link, node);
    }

    inline double Node::ambiguity(const unordered_map<pNode, pLink, Hash_pT> &links, double amplifier = 1.0)
    {
        using HELPERS::softmax;
        if (links.size() == 0)
        {
            return 1.0;
        }
        vector<double> dist(links.size());
        std::transform(
            links.begin(), links.end(), dist.begin(),
            [amplifier](const auto &pair)
            {
                const pLink &link = pair.second;
                return (1 - link->ambiguity()) * amplifier;
            });
        dist = softmax(dist);

        auto a = 1 - *std::max_element(dist.begin(), dist.end());
        return a;
    }

    inline double Node::ambiguity()
    {
        /* If there is no links around the node, the ambituity should be the maximal */
        if (this->ante_links->size() == 0 && this->post_links->size() == 0)
            return 1.0;

        /* For the anterior links, calculate the ambiguity */
        auto a1 = this->ambiguity(this->ante_links->links, 5);
        auto a2 = this->ambiguity(this->post_links->links, 5);

        /* merge the two values of ambiguity */
        auto a = std::min(a1, a2);

        return a;
    }

    inline double Node::utility()
    {
        using BOOLEAN_FUNCTIONS::Or;
        double u1 = 0, u2 = 0;
        auto it1 = this->ante_links->links_sorted.begin();
        if (it1 != this->ante_links->links_sorted.end())
        {
            auto [link, _] = *it1;
            u1 = link->pv();
        }
        auto it2 = this->post_links->links_sorted.begin();
        if (it2 != this->post_links->links_sorted.end())
        {
            auto [link, _] = *it2;
            u2 = link->pv();
        }
        auto u = Or(u1, u2);
        return u;
    }

    inline void insert_link(pNode &node1, pNode &node2, pLink &link)
    {
        /* add the link to node 1 */
        auto of_post_links = node1->insert_postlink(link);
        auto &[of_post_link1, of_post_link2] = of_post_links;
        /* add the link to node 2 */
        auto of_ante_links = node2->insert_antelink(link);
        auto &[of_ante_link1, of_ante_link2] = of_ante_links;
        // std::cout << link << ", " << of_post_link1 << ", " << of_post_link2 << ", " << of_ante_link1 << ", " << of_ante_link2 << std::endl;
        /* Check overflow */
        if (of_post_link1 != nullptr) // && of_post_link1 != link)
            // node1->remove_postlink(of_post_link1);
            of_post_link1->post_node->ante_links->remove(of_post_link1);
        if (of_post_link2 != nullptr && of_post_link1 != of_post_link2) // && of_post_link2 != link)
            // node1->remove_postlink(of_post_link2);
            of_post_link2->post_node->ante_links->remove(of_post_link2);

        if (of_ante_link1 != nullptr) // && of_ante_link1 != link)
            // node2->remove_antelink(of_ante_link1);
            of_ante_link1->ante_node->post_links->remove(of_ante_link1);
        if (of_ante_link2 != nullptr && of_ante_link1 != of_ante_link2) // && of_ante_link2 != link)
            // node2->remove_antelink(of_ante_link2);
            of_ante_link2->ante_node->post_links->remove(of_ante_link2);
    }

    inline pLink Node::connect(pNode node1, pNode node2, double w_p)
    {
        /* 如果已经存在，则避免重复添加 */
        if (node1->contains_postlink(node2) || node2->contains_antelink(node1))
            return nullptr;
        auto link = LINK::Link::create(node1, node2);
        link->truth->set_w(w_p, w_p);
        insert_link(node1, node2, link);
        return link;
    }

} // namespace NODE

#endif // _NODE_INL