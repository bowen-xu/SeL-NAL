#include <pybind11/pybind11.h>
#include "SequentialGroup/include/Column.h"
#include "SequentialGroup/include/Column.inl"
#include "SequentialGroup/include/Group.h"
// #include "NAL/Functions/include/TruthFunctions.h"
#include "helpers.hpp"
#include <numeric>

using namespace COLUMN;
using HELPERS::softmax;
// using TRUTH_FUNCTIONS::truth_induction;
// using TRUTH_FUNCTIONS::truth_revision;

std::vector<double> COLUMN::Column::get_activities(double amplifier, double expense)
{
    /** 1. get expectations */
    std::vector<double> values;
    auto group = this->group.lock();
    auto ts = (group != nullptr) ? group->ts : 0;
    std::transform(this->nodes.begin(), this->nodes.end(), std::back_inserter(values), [&amplifier, ts](const pNode &node)
                   { node->update(ts); return (double)(node->a[1]); });
    return values;
}

void COLUMN::pybind_column(py::module &m)
{
    using classType = py::class_<COLUMN::Column, COLUMN::pColumn>;
    const char *className = "Column";

    PyObject *pyObj_;
    if (!py::hasattr(m, className))
        pyObj_ = classType(m, className).ptr();
    else
        pyObj_ = m.attr(className).ptr();
    auto pyClass_ = py::reinterpret_borrow<classType>(pyObj_);

    pyClass_
        .def(py::init<size_t>())
        .def_readonly("nodes", &Column::nodes)
        .def_readwrite("mark", &Column::mark)
        .def("get_activities", &Column::get_activities, py::arg("amplifier") = 1.0, py::arg("expense") = 0.5)
        .def("get_ambiguities", &Column::get_ambiguities)
        .def("select_nodes_by_activity", &Column::select_nodes_by_activity, py::arg("expense") = 0.5, py::arg("amplifier") = 1.0)
        .def("select_nodes_by_ambiguity", &Column::select_nodes_by_ambiguity)
        .def("__getitem__", [](Column &self, double i)
             { return self.nodes[i]; })
        .def("__repr__", &Column::__repr__)
        .def("__iter__", [](Column &self)
             { return py::make_iterator(self.nodes); })
        .def("__len__", [](Column &self)
             { return self.nodes.size(); });
    // .def_readwrite("truth_predimp", &Link::truth_predimp)
    // .def_readwrite("truth_retrimp", &Link::truth_retrimp)
    // .def_readwrite("truth_predeqv", &Link::truth_predeqv)
    // .def("__hash__", &Link::__hash__)
}