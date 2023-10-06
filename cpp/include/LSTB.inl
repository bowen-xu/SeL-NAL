#ifndef _LSTB_INL
#define _LSTB_INL

#include "./LSTB.h"

namespace LSTB
{

    template <class pUnit>
    inline pUnit LSTBuffer<pUnit>::insert_to_ltb(pUnit unit)
    {

        if (n_ltb <= 0 || ltb.size() < n_ltb)
        {
            ltb.insert(unit);
            return nullptr;
        }

        double pv = unit->pv();
        auto unit_min = std::min_element(
            ltb.begin(), ltb.end(),
            [](const pUnit &lhs, const pUnit &rhs)
            { return lhs->pv() < rhs->pv(); });
        if (unit_min != ltb.end() && (*unit_min)->pv() < pv)
        {
            auto ret = *unit_min;
            ltb.erase(unit_min);
            ltb.insert(unit);
            // std::cout << "ltb: " << (*unit_min) << ", " << unit << std::endl;
            return ret;
            // return *unit_min;
        }
        else
        {
            // std::cout << "ltb: " << unit << std::endl;
            return unit;
        }
    }

    template <class pUnit>
    inline pUnit LSTBuffer<pUnit>::insert_to_stb(pUnit unit)
    {
        if (n_stb <= 0 || stb.size() < n_stb)
        {
            stb.insert(unit);
            return nullptr;
        }

        double pv = unit->pv();
        auto unit_min = std::min_element(
            stb.begin(), stb.end(),
            [](const pUnit &lhs, const pUnit &rhs)
            { return lhs->pv() < rhs->pv(); });

        // std::cout << (*unit_min)->budget_s->priority << std::endl;
        // std::cout << unit->budget_s->priority << std::endl;
        if (unit_min != stb.end() && (*unit_min)->pv() < pv)
        {
            // std::cout << "stb: " << (*unit_min) << ", " << unit << std::endl;
            auto ret = *unit_min;
            stb.erase(unit_min);
            stb.insert(unit);
            return ret;
        }
        else
        {
            // std::cout << "stb: " << unit << std::endl;
            return unit;
        }
    }

    template <class pUnit>
    inline std::pair<pUnit, pUnit> LSTBuffer<pUnit>::insert(pUnit unit)
    {
        auto of_ltb = this->insert_to_ltb(unit);
        auto of_stb = this->insert_to_stb(unit);
        if (of_ltb == of_stb)
            return std::pair(of_ltb, of_stb);
        else
        {
            if (of_stb != nullptr && ltb.count(of_stb) != 0)
                of_stb = nullptr;
            if (of_ltb != nullptr && stb.count(of_ltb) != 0)
                of_ltb = nullptr;
            return std::pair(of_ltb, of_stb);
        }
    }

    template <class pUnit>
    inline void LSTBuffer<pUnit>::remove_from_ltb(pUnit unit)
    {
        ltb.erase(unit);
    }

    template <class pUnit>
    inline void LSTBuffer<pUnit>::remove_from_stb(pUnit unit)
    {
        stb.erase(unit);
    }

    template <class pUnit>
    inline void LSTBuffer<pUnit>::erase(pUnit unit)
    {
        this->remove_from_ltb(unit);
        this->remove_from_stb(unit);
    }

    template <class pUnit>
    inline bool LSTBuffer<pUnit>::contain(pUnit unit)
    {
        return ltb.find(unit) != ltb.end() || stb.find(unit) != ltb.end();
    }

} // namespace LSTB

#endif // _LSTB_INL