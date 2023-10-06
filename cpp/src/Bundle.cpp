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
#include "include/Bundle.h"

using namespace BUNDLE;
namespace py = pybind11;

void BUNDLE::pybind_bundle(py::module &m)
{
    using classType = py::class_<BUNDLE::Bundle, BUNDLE::pBundle>;
    const char *className = "Bundle";

    PyObject *pyObj_;
    if (!py::hasattr(m, className))
        pyObj_ = classType(m, className).ptr();
    else
        pyObj_ = m.attr(className).ptr();
    auto pyClass_ = py::reinterpret_borrow<classType>(pyObj_);

    pyClass_
        .def(py::init(
            [](const pNode &node, size_t n_ltb, size_t n_stb)
            { return pBundle(new Bundle(node.get(), n_ltb, n_stb)); }))
        .def("add", &Bundle::insert)
        .def("remove", &Bundle::remove)
        .def("update", &Bundle::update)
        .def_property_readonly("maxlink", &Bundle::topmost)
        .def_property_readonly("submaxlink", &Bundle::subtop)
        .def_readonly("links", &Bundle::links)
        .def_property_readonly("links_ltb", [](const Bundle &self)
                               { return self.links_lstb.ltb; })
        .def_property_readonly("links_stb", [](const Bundle &self)
                               { return self.links_lstb.stb; })
        .def("__len__", [](Bundle &self)
             { return self.links.size(); })
        .def("__iter__", [](Bundle &self)
             { return py::make_iterator(self.links); });
}
