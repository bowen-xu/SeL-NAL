#ifndef _LSTB_H
#define _LSTB_H

#include <vector>
#include <unordered_set>
// #include "Narsese/include/Narsese.h"

namespace LSTB
{
    // using BUDGET::Budget;
    // using BUDGET::pBudget;

    using std::unordered_set;
    using std::vector;

    /*
    Long-Short-Term Buffer
    综合长期和短期的空间资源决定记忆的内容


    Budget budget_s
    */
    template <class pUnit>
    class LSTBuffer
    {
    public:
        unordered_set<pUnit> ltb; // long term buffer
        unordered_set<pUnit> stb; // short term buffer
        size_t n_ltb;
        size_t n_stb;

        LSTBuffer(size_t n_ltb, size_t n_stb) : n_ltb(n_ltb), n_stb(n_stb) {}

        pUnit insert_to_ltb(pUnit unit);

        pUnit insert_to_stb(pUnit unit);

        std::pair<pUnit, pUnit> insert(pUnit unit);

        void remove_from_ltb(pUnit unit);

        void remove_from_stb(pUnit unit);

        void erase(pUnit unit);

        bool contain(pUnit unit);
    };
} // namespace LSTB

#endif // _LSTB_H