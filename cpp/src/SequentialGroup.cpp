
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <functional>
#include <iostream>
#include <string>
#include "SequentialGroup/include/Group.h"
#include "SequentialGroup/include/Bundle.h"
#include <chrono>
// #include "cNARS/Narsese/include/Narsese.h"
// #include "cNARS/Narsese/Parser/NarseseParser.h"
// #include "cNARS/Interpreter/include/Interpreter.hpp"


using GROUP::Group;
using NODE::Node;
using NODE::pNode;
using BUNDLE::Bundle;
using LINK::Link;
using LINK::pLink;
using COLUMN::Column;
using COLUMN::pColumn;

void test()
{
    int n_nodes = 16;
    int n_columns = 26;
    double sparse_rate = 1.0;
    double mu = 0.3;
    double sigma = 0.01;

    GROUP::Group g(n_columns, n_nodes);
    auto start_time = std::chrono::high_resolution_clock::now();
    // Initialize random number generator
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    for (int j = 0; j < n_columns; ++j)
    {
        for (int i = 0; i < n_nodes; ++i)
        {
            std::vector<std::vector<bool>> sparse_matrix(n_nodes, std::vector<bool>(n_columns));

            for (int i_node = 0; i_node < n_nodes; ++i_node)
            {
                for (int i_column = 0; i_column < n_columns; ++i_column)
                {
                    double random_value = static_cast<double>(std::rand()) / RAND_MAX;
                    sparse_matrix[i_node][i_column] = (random_value < sparse_rate);
                }
            }

            sparse_matrix[i][j] = false;

            for (int i_node = 0; i_node < n_nodes; ++i_node)
            {
                for (int i_column = 0; i_column < n_columns; ++i_column)
                {
                    if (sparse_matrix[i_node][i_column])
                    {
                        double pv = mu + sigma * std::sqrt(-2.0 * std::log(static_cast<double>(std::rand()) / RAND_MAX)) * std::cos(2.0 * M_PI * static_cast<double>(std::rand()) / RAND_MAX);
                        NODE::Node::connect(g.columns[i_column]->nodes[i_node], g.columns[j]->nodes[i], pv);
                    }
                }
            }
        }
    }

    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time)/1000.0;
    std::cout << "Execution time: " << duration.count() << " seconds" << std::endl;

    for (auto i = 0; i < 10000; i++)
    {
        auto idx = i%n_columns;
        g.activate(idx);
    }
    /*
    Release模式下， 10^4次循环，3.4s。大约3000it/s，
    相比于最初的python代码30it/s，加速了100倍。
    */
}

PYBIND11_MODULE(SequentialGroup, m)
{
    namespace py = pybind11;

    m.doc() = R"pbdoc(
        SequentialGroup module
    )pbdoc";
    py::module_::import("pyBodhi.narsese");
    volatile auto narsese = py::module_::import("pyBodhi.narsese");
    m.attr("narsese") = py::module_::import("pyBodhi.narsese");
    m.def("test", &test);
    auto pyNode = py::class_<NODE::Node, NODE::pNode>(m, "Node");
    auto pyLink = py::class_<LINK::Link, LINK::pLink>(m, "Link");
    auto pyColumn = py::class_<COLUMN::Column, COLUMN::pColumn>(m, "Column");
    auto pyGroup = py::class_<GROUP::Group, GROUP::pGroup>(m, "Group");
    auto pyBundle = py::class_<BUNDLE::Bundle, BUNDLE::pBundle>(m, "Bundle");
    NODE::pybind_node(m);
    LINK::pybind_link(m);
    COLUMN::pybind_column(m);
    GROUP::pybind_group(m);
    BUNDLE::pybind_bundle(m);

    /* Narsese */

    // COPULA::pybind_copula(m);
    // CONNECTOR::pybind_connnector(m);

    // NARSESEPARSER::pybind_parse(m);
    // TERM::pybind_term(m);
    // STATEMENT::pybind_statement(m);
    // COMPOUND::pybind_compound(m);

    // TASK::pybind_task(m);
    // SENTENCE::pybind_sentence(m);
    // TRUTH::pybind_truth(m);
    // BUDGET::pybind_budget(m);

    // py::class_<NARSESEPARSER::NarseseParser, std::shared_ptr<NARSESEPARSER::NarseseParser>>(m, "NarseseParser");
    // m.attr("parser") = NARSESEPARSER::parser;
}