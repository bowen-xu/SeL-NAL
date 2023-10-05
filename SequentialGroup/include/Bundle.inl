#ifndef _BUNDLE_INL
#define _BUNDLE_INL

#include "Bundle.h"
#include "Node.inl"
#include "Link.inl"
#include "Column.inl"
#include <algorithm>
#include "Narsese/include/Narsese.h"

namespace BUNDLE
{
    template <typename _pT>
    inline size_t Hash_pT::operator()(const _pT &key) const
    {
        return hash(*key);
    }

    inline void Bundle::set_capacity(size_t n_ltb, size_t n_stb)
    {
        this->links_lstb.n_ltb = n_ltb;
        this->links_lstb.n_stb = n_stb;
    }

    inline pLink Bundle::topmost()
    {
        if (this->links_sorted.size() < 1)
            return nullptr;
        auto &[link, _] = *this->links_sorted.begin();
        return link;
    }

    inline pLink Bundle::subtop()
    {
        if (this->links_sorted.size() < 2)
            return nullptr;
        auto &[link, _] = *(++this->links_sorted.begin());
        return link;
    }

    inline bool Bundle::remove(const pLink &link)
    {
        // if (link->post_node.get() != this->node || this->links.count(link->ante_node) == 0)
        //     return false;

        auto it_link = this->links_sorted.find(link);
        if (it_link == links_sorted.end())
            return false;
        const auto &[_, node] = *it_link;

        this->links.erase(node);
        this->links_sorted.erase(link);

        auto column = link->ante_node->column.lock();
        if (column != nullptr)
        {
            this->links_col[column].erase(link);
            if (this->links_col.size() == 0)
                this->links_col.erase(column);
        }
        this->links_lstb.erase(link);
        return true;
    }

    inline std::pair<pLink, pLink> Bundle::insert(const pLink &link, const pNode &node)
    {
        auto &&links_of = this->links_lstb.insert(link);
        /* link插入到lstb中，如果插入失败，说明记忆已经占满了，则无法再插入 */
        if (link == links_of.first && link == links_of.second)
            return links_of;
        /* 如果插入成功，但发生了溢出，则需要把溢出的部分删除 */
        this->links[node] = link;
        this->links_sorted[link] = node;
        auto column = node->column.lock();
        if (column != nullptr)
        {
            if (this->links_col.find(column) == this->links_col.end())
                this->links_col[column] = unordered_set<pLink>();
            this->links_col[column].insert(link);
        }
        if (links_of.first != nullptr && links_of.first == links_of.second)
        {
            auto &link_of = links_of.first;
            this->remove(link_of);
        }
        return links_of;
    }

    inline void Bundle::update(pLink &link, TRUTH::Truth& truth)
    {
        /* 如果link不存在，则直接返回 */
        auto it_link = this->links_sorted.find(link);
        if (it_link == this->links_sorted.end())
            return;
        /* 更新排序 */
        auto [_link, _node] = *it_link;
        this->links_sorted.erase(it_link);
        link->revise(truth);
        this->links_sorted[link] = _node;
    }

    inline bool Bundle::contains(const pNode &node) const
    {
        /* O(n) = 1 */
        return this->links.count(node) > 0;
    }

    inline bool Bundle::contains(const pLink &link) const
    {
        /* O(n) = log(n) */
        return this->links_sorted.count(link) > 0;
    }

    inline bool Bundle::size() const
    {
        return this->links.size();
    }

    inline pLink &Bundle::operator[](const pNode &node)
    {
        return this->links[node];
    }

} // namespace BUNDLE

#endif // _BUNDLE_INL