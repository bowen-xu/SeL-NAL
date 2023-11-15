#include <pybind11/pybind11.h>
#include <set>
#include <algorithm>
#include <tuple>

#include "include/Group.h"
#include "include/Buffer.h"
#include "include/Node.h"

using namespace GROUP;
// using BOOLEAN_FUNCTIONS::Or;
namespace UMF = UNCERTAINTY_MAPPING_FUNCTIONS;
using LINK::pLink;
using NODE::Node;
using NODE::pNode;

using std::tuple;
using std::vector;

// SG_GLOBAL::Buffer SG_GLOBAL::event_buffer = SG_GLOBAL::Buffer();

Group::Group(size_t n_columns, size_t n_nodes)
{
    distribution_pv = std::normal_distribution<double>(0.3, 0.01);
    for (auto i = 0; i < n_columns; i++)
    {
        auto column = COLUMN::Column::create(n_nodes);
        this->columns.push_back(column);
        // column->group.set(this);
    }
    this->n_columns = n_columns;
    this->n_nodes = n_nodes;
}

void Group::build_links(vector<pNode> &&ante_nodes, vector<pNode> &&post_nodes)
{
    Group::build_links(ante_nodes, post_nodes);
}

void Group::build_links(vector<pNode> &&ante_nodes, vector<pNode> &post_nodes)
{
    Group::build_links(ante_nodes, post_nodes);
}

void Group::build_links(vector<pNode> &ante_nodes, vector<pNode> &&post_nodes)
{
    Group::build_links(ante_nodes, post_nodes);
}

template <typename _Container>
void Group::build_links(_Container &ante_nodes, _Container &post_nodes)
{
    for (auto &ante_node : ante_nodes)
    {
        for (auto &post_node : post_nodes)
        {
            if (!ante_node->post_links->contains(post_node) && !post_node->ante_links->contains(ante_node))
            {
                NODE::Node::connect(ante_node, post_node);
                // std::cout << "build link" << std::endl;
            }
        }
    }
}

std::set<pNode> &Group::activate(size_t i_column)
{
    auto &column = this->columns[i_column];
    auto &nodes = column->nodes;
    this->ts += 1;
    this->buffer2 = this->buffer;
    this->buffer.clear();

    /* 更新node的状态 */
    for (auto &node : nodes)
    {
        node->update(this->ts);
    }
    /* 激活结点。首先判断是否存在预激活的结点，如果存在，则激活那些结点；否则激活所有结点。 */
    bool anticipated = std::any_of(
        nodes.begin(), nodes.end(), [](const pNode &node)
        { return node->p[0]; });
    if (anticipated)
    {
        for (auto &node : nodes)
            if (node->p[0])
                node->a[1] = true;
    }
    else
    {
        for (auto &node : nodes)
            node->a[1] = true;
    }

    /* 预测结点。 */
    for (auto &ante_node : nodes)
    {
        for (auto &[_, post_links] : ante_node->post_links->links_col)
        {
            if (post_links.empty())
                continue;
            auto &post_link = *std::max_element(
                post_links.begin(), post_links.end(), [](const pLink &a, const pLink &b)
                { return a->pv() < b->pv(); });
            auto &post_node = post_link->post_node;
            if (ante_node->a[1] * post_link->pv() > this->thresh)
            {
                post_node->update(this->ts);
                post_node->p[1] = true;
                post_link->ts_activate = this->ts;
                post_node->activty = post_link->pv();
                // post_node->ts_update = this->ts;
                this->buffer.insert(post_node);
            }
        }
    }
    this->learn(i_column);
    this->last_activaetd_column = this->columns[i_column];
    return this->buffer;
}

void Group::learn(size_t i_column)
{
    /*
        须确保学习过程紧跟在激活之后、相同时间戳内
    */
    if (this->last_activaetd_column == nullptr)
        return;
    auto &column = this->columns[i_column];
    auto &nodes = column->nodes;
    /* 选择当前column中激活的用于学习的结点 */
    vector<pNode> nodes_selected;
    std::copy_if(
        nodes.begin(), nodes.end(), std::back_inserter(nodes_selected), [this](const pNode &node)
        { node->update(this->ts); return  node->p[0] && node->a[1]; });

    pNode node_selected = nullptr;
    pLink link_selected = nullptr;

    auto _get_value = [this](pLink link, pNode node, BUNDLE::pBundle bundle)
    {
        node->update(this->ts);
        double ambiguity = 0.0;
        /*
        这里我本来想尝试，在修改时避开那些使用度很高的结点。但是没有成功。

        // pLink link_max = nullptr;
        // if (bundle->links_sorted.size() > 0)
        // {
        //     for (auto [ante_link, ante_node] : bundle->links_sorted)
        //     {
        //         if (ante_node->column.lock() != this->last_activaetd_column)
        //         {
        //             link_max = ante_link;
        //             break;
        //         }
        //     }
        // }
        // if (link_max != nullptr)
        //     ambiguity = link_max->pv;

        // auto &&link_max1 = bundle->topmost();
        // auto &&link_max2 = bundle->subtop();
        // // if (link == link_max1)
        // {
        //     if (link_max2 != nullptr)
        //         ambiguity = 1.0 - link_max2->pv;
        //     else
        //         ambiguity = 0.0;
        //     // ambiguity = 0.0;
        // }
        // else
        // {
        //     if (link_max1 != nullptr)
        //     {
        //         ambiguity = 1 - link_max1->pv;
        //         // ambiguity = link_max1->pv > this->thresh ? std::pow(link_max1->pv, 1.0/3) : 1 - std::pow(link_max1->pv, 3);
        //         // ambiguity = link_max1->pv > this->thresh ? 0.1 : 0.9;
        //     }
        //     else
        //         ambiguity = 0.0;
        // }
        */
        return link->pv() * (node->a[0] + 1e-3) * (1.0 - ambiguity);
    };

    auto _get_value2 = [this](pLink link, pNode node)
    {
        node->update(this->ts);

        return link->pv() * (node->a[0] + 1e-3);
    };

    auto _learn = [this](pLink link, pNode node)
    {
        node->update(this->ts);

        auto delta = this->p_plus * node->a[0] - this->p_minus;
        auto w_p = delta > 0.0 ? delta : 0.0;
        auto w_m = delta < 0 ? this->p_minus : 0;
        auto truth = UMF::truth_from_w(w_p, w_p + w_m, 1);
        return truth;
    };

    auto _learn2 = [this](pLink link, double p_minus2)
    {
        auto w_p = 0.0;
        auto w_m = std::max(p_minus2, this->p_minus2);
        auto truth = UMF::truth_from_w(w_p, w_p + w_m, 1);
        return truth;
    };

    /*
    在上一次激活和这一次激活的结点中，建立hypothetical links

    思路：
        给定一组nodes，寻找使用度最高和最低的结点，
        在两组nodes之间建立全连接。
    */
    auto nodes1 = this->buffer_h;
    std::list<pNode> nodes2;
    std::copy_if(
        nodes.begin(), nodes.end(), std::back_inserter(nodes2),
        [](const pNode &node)
        {
            return node->a[1];
        });
    if (nodes1.size() > 0 && nodes2.size() > 0)
    {
        /*
        nodes1是上一个时刻激活的结点
        */
        auto [it1, it2] = std::minmax_element(
            nodes1.begin(), nodes1.end(),
            [](const pNode &a, const pNode &b)
            {
                return a->utility() < b->utility();
            });
        auto node1_min = *it1;
        auto node1_max = *it2;

        auto [it3, it4] = std::minmax_element(
            nodes2.begin(), nodes2.end(),
            [](const pNode &a, const pNode &b)
            {
                return a->utility() < b->utility();
            });
        auto node2_min = *it3;
        auto node2_max = *it4;
        if (!node1_min->post_links->contains(node2_min))
        {
            Node::connect(node1_min, node2_min, this->distribution_pv(gen));
        }
        if (!node1_max->post_links->contains(node2_min))
        {
            Node::connect(node1_max, node2_min, this->distribution_pv(gen));
        }
        if (!node1_min->post_links->contains(node2_max))
        {
            Node::connect(node1_min, node2_max, this->distribution_pv(gen));
        }
        if (!node1_max->post_links->contains(node2_max))
        {
            Node::connect(node1_max, node2_max, this->distribution_pv(gen));
        }
    }
    this->buffer_h = nodes2;

    if (nodes_selected.size() == 0)
    {
        /* 如果没有任何预测后激活的结点，则从现有的激活结点中，选取一个value最大的结点进行学习。 */
        double value_max = 0.0;
        for (pNode &post_node : nodes)
            for (auto &[ante_node, ante_link] : post_node->ante_links->links)
            {
                double value = _get_value(ante_link, ante_node, post_node->ante_links);
                if (value > value_max)
                {
                    value_max = value;
                    node_selected = post_node;
                    link_selected = ante_link;
                }
            }

        if (node_selected != nullptr && link_selected != nullptr)
        {
            link_selected->ante_node->update(this->ts);
            if (link_selected->ante_node->a[0])
            {
                auto &&truth = _learn(link_selected, link_selected->ante_node);
                node_selected->ante_links->update(link_selected, truth);
            }
        }

        if (node_selected != nullptr)
            if (this->buffer2.count(node_selected) > 0)
                this->buffer2.erase(node_selected);
    }
    else
    {
        /* 否则，有一些结点被激活，且此前被预测。
         * 这时候按照ante_node所属的column，对所有的ante_links进行分组
         * 每个组内挑选一个ante_link进行学习
         * Note: 目前的设计中，每个组内只有一个ante_link进行学习。是否考虑，如果有一条进行“正学习”，则其余的进行“负学习”？
         */

        for (auto &post_node : nodes_selected)
        {
            if (this->buffer2.count(post_node) > 0)
                this->buffer2.erase(post_node);
            for (auto &[ante_column, ante_links] : post_node->ante_links->links_col)
            {
                if (ante_column != this->last_activaetd_column)
                    continue;
                if (ante_links.empty())
                    continue;
                link_selected = *std::max_element(
                    ante_links.begin(), ante_links.end(), [_get_value2](pLink ante_link1, pLink ante_link2)
                    { return _get_value2(ante_link1, ante_link1->ante_node) < _get_value2(ante_link2, ante_link2->ante_node); });
                node_selected = link_selected->ante_node;

                link_selected->ante_node->update(this->ts);
                if (link_selected->ante_node->a[0])
                {
                    auto pv = _learn(link_selected, link_selected->ante_node);
                    post_node->ante_links->update(link_selected, pv);
                }
            }
        }
    }
    
    for (auto &post_node : this->buffer2)
    {
        // auto n = post_node->ante_links->links.size();
        // auto p_m = this->p_minus/(std::sqrt(n)*2);
        auto& links = post_node->ante_links->links;
        auto n = std::count_if(links.begin(), links.end(), [this](const auto& pair){return pair.second->pv() > this->thresh;}) + 40;
        auto p_m = this->p_minus/n;
        for (auto &[_, ante_link] : post_node->ante_links->links)
        {
            if (ante_link->ts_activate < this->ts - 1)
                continue;
            auto pv = _learn2(ante_link, p_m);
            post_node->ante_links->update(ante_link, pv);
        }
    }
}

void GROUP::pybind_group(py::module &m)
{
    using classType = py::class_<GROUP::Group, GROUP::pGroup>;
    const char *className = "Group";

    PyObject *pyObj_;
    if (!py::hasattr(m, className))
        pyObj_ = classType(m, className).ptr();
    else
        pyObj_ = m.attr(className).ptr();
    auto pyClass_ = py::reinterpret_borrow<classType>(pyObj_);

    pyClass_
        .def(py::init(&Group::create))
        .def("activate", &Group::activate)
        .def_static("build_links", [](vector<pNode> &nodes1, vector<pNode> &nodes2)
                    { Group::build_links(nodes1, nodes2); })
        .def_readonly("ts", &Group::ts)
        .def(
            "nop",
            [](Group &self)
            {
                self.ts += 1;
                self.buffer2 = self.buffer;
                self.buffer.clear();
            })
        .def_readwrite("thresh", &Group::thresh)
        .def_readonly("n_columns", &Group::n_columns)
        .def_readonly("n_nodes", &Group::n_nodes)
        .def_readonly("columns", &Group::columns)
        .def_readwrite("p_plus", &Group::p_plus)
        .def_readwrite("p_minus", &Group::p_minus)
        .def_readwrite("p_minus2", &Group::p_minus2)
        .def(py::pickle(
            [](const Group &g)
            {
                vector<vector<double>> pvs;
                vector<vector<int64_t>> ts_link;
                vector<vector<bool>> as;
                vector<vector<bool>> ps;
                vector<int64_t> ts_node;
                auto ts_group = g.ts;
                auto params = vector<double>{g.thresh, g.p_plus, g.p_minus, g.p_minus2};

                pvs.resize(g.n_columns * g.n_nodes);
                for (auto &pv : pvs)
                    pv.resize(g.n_columns * g.n_nodes, 0.0);

                ts_link.resize(g.n_columns * g.n_nodes);
                for (auto &ts : ts_link)
                    ts.resize(g.n_columns * g.n_nodes, 0);
                for (auto i = 0; i < g.n_columns * g.n_nodes; i++)
                    for (auto j = 0; j < g.n_columns * g.n_nodes; j++)
                    {
                        if (i == j)
                            continue;
                        auto &node1 = g.columns[(size_t)(i / g.n_nodes)]->nodes[(size_t)(i % g.n_nodes)];
                        auto &node2 = g.columns[(size_t)(j / g.n_nodes)]->nodes[(size_t)(j % g.n_nodes)];

                        auto &link = node1->post_links->links[node2];
                        pvs[i][j] = link->pv();
                        ts_link[i][j] = link->ts_activate;
                    }

                as.resize(g.n_columns * g.n_nodes);
                ps.resize(g.n_columns * g.n_nodes);
                ts_node.resize(g.n_columns * g.n_nodes);
                for (auto &a : as)
                    a.resize(2);
                for (auto &p : ps)
                    p.resize(2);
                for (auto i = 0; i < g.n_columns * g.n_nodes; i++)
                {
                    auto &node = g.columns[(size_t)(i / g.n_nodes)]->nodes[(size_t)(i % g.n_nodes)];
                    as[i] = node->a;
                    ps[i] = node->p;
                    ts_node[i] = node->ts_update;
                }

                return py::make_tuple(g.n_columns, g.n_nodes, pvs, ts_link, as, ps, ts_node, ts_group, params);
            },
            [](const py::tuple &s)
            {
                // if (s.size() != 7)
                //     throw std::runtime_error("Invalid state!");
                auto n_columns = s[0].cast<size_t>();
                auto n_nodes = s[1].cast<size_t>();
                auto pvs = s[2].cast<vector<vector<double>>>();
                auto ts_link = s[3].cast<vector<vector<int64_t>>>();
                auto as = s[4].cast<vector<vector<bool>>>();
                auto ps = s[5].cast<vector<vector<bool>>>();
                auto ts_node = s[6].cast<vector<int64_t>>();
                auto ts_group = s[7].cast<size_t>();
                auto params = s[8].cast<vector<double>>();
                auto thresh = params[0];
                auto p_plus = params[1];
                auto p_minus = params[2];
                auto p_minus2 = params[3];

                auto _g = pGroup(new Group(n_columns, n_nodes));
                auto &g = *_g;
                g.ts = ts_group;
                g.thresh = thresh;
                g.p_plus = p_plus;
                g.p_minus = p_minus;
                g.p_minus2 = p_minus2;

                for (auto i = 0; i < n_columns * n_nodes; i++)
                    for (auto j = 0; j < n_columns * n_nodes; j++)
                    {
                        if (i == j)
                            continue;
                        auto pv = pvs[i][j];
                        auto ts = ts_link[i][j];
                        auto &node1 = g.columns[(size_t)(i / g.n_nodes)]->nodes[(size_t)(i % g.n_nodes)];
                        auto &node2 = g.columns[(size_t)(j / g.n_nodes)]->nodes[(size_t)(j % g.n_nodes)];
                        auto &&link = Node::connect(node1, node2, pv);
                        link->ts_activate = ts;
                    }
                for (auto i = 0; i < n_columns * n_nodes; i++)
                {
                    auto &node = g.columns[(size_t)(i / g.n_nodes)]->nodes[(size_t)(i % g.n_nodes)];
                    node->a = as[i];
                    node->p = ps[i];
                    node->ts_update = ts_node[i];
                }
                std::cout << _g->ts << std::endl;
                return _g;
            }))
        .def("__repr__", &Group::__repr__)
        .def("__getitem__", ([](Group &self, std::tuple<size_t, size_t> idx)
                             {
            auto& [i_column, i_node] = idx;
            if (i_column >= self.n_columns) 
                return (pNode)nullptr;
            else if (i_node >= self.n_nodes) 
                return (pNode)nullptr;
            else
                return self.columns[i_column]->nodes[i_node]; }))
        .def("__getitem__", [](Group &self, size_t i_column)
             {
            if (i_column >= self.n_columns) 
                return pColumn(nullptr);
            else
                return self.columns[i_column]; })
        .def("__iter__", [](Group &self)
             { return py::make_iterator(self.columns); });

    py::class_<BUFFER::Buffer, std::shared_ptr<BUFFER::Buffer>>(m, "Buffer");

    // m.def("get_buffer", [](){return SG_GLOBAL::event_buffer;});
    // m.attr("event_buffer") = SG_GLOBAL::event_buffer;
}