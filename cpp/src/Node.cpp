#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <vector>
#include <memory>
#include <string>
#include "include/Node.h"
#include "include/Link.h"
#include "include/Node.inl"
#include "include/Link.inl"
// #include "Interpreter/include/Interpreter.hpp"
// #include "NAL/Functions/include/TruthFunctions.h"
// #include "Narsese/include/Narsese.h"
#include <fmt/core.h>
#include <iostream>
#include "include/Group.h"

using namespace NODE;

namespace py = pybind11;

// using INTERPRETER::Interpreter;
// using BUDGET::Budget;
// using TRUTH::Truth;
// using TRUTH_FUNCTIONS::truth_comparison;
// using TRUTH_FUNCTIONS::truth_induction;
// using TRUTH_FUNCTIONS::truth_revision;

std::string Node::__str__(void *interpreter = nullptr) const
{
    // if (interpreter == nullptr)
    // {
    //     interpreter = this->term._interpreter;
    // }
    std::string word;
    // if (interpreter != nullptr)
    // {
    //     auto &_interpreter = *(Interpreter *)interpreter;
    //     word = _interpreter.interpret(*(this->term));
    // }
    // else
    // {
    word = fmt::format("at {:x}", (size_t)this);
    // }
    return word;
}

std::string Node::__repr__(void *interpreter = nullptr) const
{
    auto word = this->__str__(interpreter);
    // std::string word = "";
    return fmt::format("<Node: {}>", word);
}

pNode Node::create(std::string word, size_t n_ltb, size_t n_stb)
{
    return pNode(new Node(word, n_ltb, n_stb));
}

pNode Node::create(size_t n_ltb, size_t n_stb)
{
    return pNode(new Node(n_ltb, n_stb));
}

void NODE::pybind_node(py::module &m)
{
    using classType = py::class_<NODE::Node, NODE::pNode>;
    const char *className = "Node";
    PyObject *pyObj_;
    if (!py::hasattr(m, className))
        pyObj_ = classType(m, className).ptr();
    else
        pyObj_ = m.attr(className).ptr();
    auto pyClass_ = py::reinterpret_borrow<classType>(pyObj_);
    // MyClass* myClassPtr = py::cast<MyClass*>(pyObj);
    pyClass_
        .def(py::init(py::overload_cast<std::string, size_t, size_t>(&NODE::Node::create)), py::arg("word"), py::arg("n_ltb") = 100, py::arg("n_stb") = 100)
        .def(py::init(py::overload_cast<size_t, size_t>(&NODE::Node::create)), py::arg("n_ltb") = 100, py::arg("n_stb") = 100)
        .def("__repr__", &NODE::Node::__repr__, py::arg("interpreter") = nullptr)
        // .def("__repr__", &NODE::Node::__repr__, py::arg("interpreter") = (void *)&INTERPRETER::interpreter)
        .def_property_readonly("ambiguity", py::overload_cast<>(&Node::ambiguity))
        .def_static("connect", &Node::connect)
        .def(
            "connect_to",
            [](pNode &node1, pNode &node2, double pv)
            {
                auto link = Node::connect(node1, node2, pv);
                return link;
            },
            // py::arg("node1"),
            py::arg("node2"),
            py::arg("pv") = 0.3)
        // .def_property_readonly(
        //     "ante_links",
        //     [](Node &self)
        //     {
        //         std::vector<pLink> values;
        //         std::transform(self.ante_links->links.begin(), self.ante_links->links.end(), std::back_inserter(values), [](const auto &pair)
        //                        { return pair.second; });
        //         return values;
        //     })
        .def_readonly("ante_links", &Node::ante_links)
        // .def_property_readonly(
        //     "post_links",
        //     [](Node &self)
        //     {
        //         std::vector<pLink> values;
        //         std::transform(self.post_links->links.begin(), self.post_links->links.end(), std::back_inserter(values), [](const auto &pair)
        //                        { return pair.second; });
        //         return values;
        //     })
        .def_readonly("post_links", &Node::post_links)
        .def_readwrite("a", &Node::a)
        .def_readwrite("p", &Node::p)
        .def_property_readonly(
            "column", [](Node &self)
            { return self.column.lock(); })
        .def("update", &Node::update)
        .def_property_readonly("activity", [](const Node& self){return self.a[1]*self.activty;})
        .def_property_readonly("activity_pred", [](const Node& self){return self.p[1]*self.activty;})
        .def("__hash__", &Node::__hash__)
        .def("__contains__", [](Node &self, pLink link)
             { return (self.contains_antelink(link) || self.contains_postlink(link)); })
        .def("contains_antelink", py::overload_cast<const pLink &>(&Node::contains_antelink, py::const_))
        .def("contains_antelink", py::overload_cast<const pNode &>(&Node::contains_antelink, py::const_))
        .def("contains_postlink", py::overload_cast<const pLink &>(&Node::contains_postlink, py::const_))
        .def("contains_postlink", py::overload_cast<const pNode &>(&Node::contains_postlink, py::const_))
        .def("remove_antelink", py::overload_cast<const pLink &>(&Node::remove_antelink))
        .def("remove_antelink", py::overload_cast<const pNode &>(&Node::remove_antelink))
        .def("remove_postlink", py::overload_cast<const pLink &>(&Node::remove_postlink))
        .def("remove_postlink", py::overload_cast<const pNode &>(&Node::remove_postlink))
        // .def("remove_antelink")
        .def("remove_link", [](Node &self, pLink &link)
             { return self.remove_postlink(link) || self.remove_antelink(link); })
        .def("remove_link", [](Node &self, pNode &node)
             { return self.remove_postlink(node) || self.remove_antelink(node); });
}
