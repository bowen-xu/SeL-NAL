#ifndef _BODHI_BUFFER_H
#define _BODHI_BUFFER_H
#include <queue>
#include <algorithm>
#include <functional>
#include <any>
#include <optional>
#include "./Node.h"

namespace BUFFER
{
    using NODE::pNode;
    // static size_t timestamp = 0;
    // static size_t &ts = timestamp;

    // static size_t D_tw = 200; // size of the time window

    struct Event
    {
        pNode node;
        int t;

        Event(pNode node, int t)
            : node(node), t(t) {}

        bool operator<(const Event &other) const
        {
            return t > other.t;
        }
    };

    class Buffer
    {
    public:
        std::priority_queue<Event> _pq;

        Buffer() {}

        void add(signed int t, pNode node)
        {
            _pq.push(Event(node, t));
        }

        std::optional<size_t> get_t_min()
        {
            if (_pq.empty())
            {
                return std::nullopt;
            }
            return _pq.top().t;
        }

        pNode pop()
        {
            if (!_pq.empty())
            {
                auto event = _pq.top();
                _pq.pop();
                return event.node;
            }
            else
                return nullptr;
        }
    };

    struct Events
    {
        std::vector<pNode> nodes;
        int t;

        Events(std::vector<pNode> &nodes, int t)
            : nodes(nodes), t(t) {}

        Events(std::initializer_list<pNode> &nodes, int t)
            : nodes(nodes), t(t) {}

        bool operator<(const Events &other) const
        {
            return t > other.t;
        }
    };
    typedef std::shared_ptr<Events> pEvents;

    class BufferH // Buffer for building hypotheses
    {
    private:
        std::vector<pNode> empty_nodes{};

    public:
        struct _Comp
        {
            bool operator()(pEvents &lhs, pEvents &rhs) { return *lhs < *rhs; }
        };
        std::priority_queue<pEvents, std::vector<pEvents>, _Comp> _pq;

        BufferH() {}

        void add(signed int t, std::vector<pNode> &nodes)
        {
            _pq.push(pEvents(new Events(nodes, t)));
        }

        void add(signed int t, std::initializer_list<pNode> &&nodes)
        {
            _pq.push(pEvents(new Events(nodes, t)));
        }

        void add(signed int t, pEvents &events)
        {
            _pq.push(events);
        }

        std::optional<size_t> get_t_min()
        {
            if (_pq.empty())
            {
                return std::nullopt;
            }
            return _pq.top()->t;
        }

        pEvents pop()
        {
            if (!_pq.empty())
            {
                auto event = _pq.top();
                _pq.pop();
                return event;
            }
            else
                return nullptr;
        }

        auto to_vector()
        {
            auto pq = this->_pq;
            std::vector<pEvents> vec;
            while (!pq.empty())
            {
                vec.push_back(pq.top());
                pq.pop();
            }
            return vec;
        }
    };

    // static Buffer event_buffer;
}

#endif // _BODHI_BUFFER_H