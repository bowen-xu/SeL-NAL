#include "include/Link.h"
#include "include/Node.h"
#include "include/Link.inl"
#include "include/Node.inl"
// #include "Interpreter/include/Interpreter.hpp"
#include <fmt/core.h>

using namespace LINK;
using NODE::Node;

std::string Link::__repr__(void *interpreter = nullptr)
{
    return fmt::format("<Link: {} ---> {}>", this->ante_node->__str__(interpreter), this->post_node->__str__(interpreter));
}

void LINK::pybind_link(py::module &m)
{
    using classType = py::class_<LINK::Link, LINK::pLink>;
    const char *className = "Link";

    PyObject *pyObj_;
    if (!py::hasattr(m, className))
        pyObj_ = classType(m, className).ptr();
    else
        pyObj_ = m.attr(className).ptr();
    auto pyClass_ = py::reinterpret_borrow<classType>(pyObj_);

    pyClass_
        // .def("__repr__", &Link::__repr__, py::arg("interpreter") = (void *)&INTERPRETER::interpreter)
        .def("__repr__", &Link::__repr__, py::arg("interpreter") = nullptr)
        .def_readonly("ante_node", &Link::ante_node)
        .def_readonly("post_node", &Link::post_node)
        .def_readwrite("ts_activate", &Link::ts_activate)
        .def_property_readonly("pv", &Link::pv)
        .def("__hash__", &Link::__hash__);
}
