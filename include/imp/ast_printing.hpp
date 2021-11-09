#pragma once

#include <fstream>
#include <imp/syntax_types.hpp>
#include <imp/util/node_id.hpp>
#include <iostream>
#include <string>

namespace imp {
using std::ostream;
using std::string;

// By default, there is no styling
inline std::string_view style_node(std::vector<command> const&) {
    return R"(label="[list of commands]")";
}
template <class Expr>
std::string_view style_node(assignment<Expr> const&) {
    return R"(fontname="Hack italic, monospace italic"; label="assignment")";
}
inline auto style_node(bool_const const& node) {
    return fmt::format(R"(fontcolor="#9ECE6A"; label="{}")", node.value);
}
inline auto style_node(constant const& node) {
    return fmt::format(R"(fontcolor="#9ECE6A"; label="{}")", node.value);
}
inline std::string_view style_node(while_loop<bool_expr, command> const&) {
    return R"(fontcolor="#BB9AF7"; label="while loop")";
}
inline std::string_view style_node(if_command<bool_expr, command> const&) {
    return R"(fontcolor="#BB9AF7"; label="conditional")";
}
inline std::string_view style_node(skip_command const&) {
    return R"(fontcolor="#BB9AF7"; label="skip")";
}
inline auto style_node(variable const& node) {
    return fmt::format(R"(fontcolor="#7AA2F7"; label="{}")", node.get_name());
}
template <class Expr>
auto style_node(unary_expr<Expr> const& node) {
    return fmt::format(
        R"(fontname="Hack bold, monospace bold"; label="{}")",
        node.get_op());
}
template <class Expr>
auto style_node(binary_expr<Expr> const& node) {
    return fmt::format(
        R"(fontname="Hack bold, monospace bold"; label="{}")",
        node.get_op());
}
template <class... T>
auto style_node(rva::variant<T...> const& node) {
    return rva::visit([](auto const& item) { return style_node(item); }, node);
}

void declare_nodes(ostream& os, auto const& node) {
    // Declare this node
    os << fmt::format("    {} [{}];\n", get_id(node), style_node(node));
    // Declare the child nodes
    for_each(node, [&](auto const& child) { declare_nodes(os, child); });
}

void print_edges(ostream& os, auto const& node) {
    // For each child, print an edge from this node to the child
    for_each(node, [&](auto const& child) {
        os << fmt::format("    {} -> {};\n", get_id(node), get_id(child));
        print_edges(os, child);
    });
}

inline void print_graph(ostream& os, arith_expr const& ast) {
    os << R"(digraph {
    graph [
        bgcolor="#24283B"
        pad="0.5"
        dpi=300]
    node [
        fontsize=12
        fontcolor="#ffffff"
        color="#E0AF68"
        shape=underline
        fontname="Hack, monospace"]
    edge [
        color="#E0AF68"
        arrowsize=0.5]
)";
    declare_nodes(os, ast);
    print_edges(os, ast);
    os << "}\n";
}
inline void print_graph(ostream& os, bool_expr const& ast) {
    os << R"(digraph {
    graph [
        bgcolor="#24283B"
        pad="0.5"
        dpi=300]
    node [
        fontsize=12
        fontcolor="#ffffff"
        color="#E0AF68"
        shape=underline
        fontname="Hack, monospace"]
    edge [
        color="#E0AF68"
        arrowsize=0.5]
)";
    declare_nodes(os, ast);
    print_edges(os, ast);
    os << "}\n";
}
inline void print_graph(ostream& os, command const& ast) {
    os << R"(digraph {
    graph [
        bgcolor="#24283B"
        pad="0.5"
        dpi=300]
    node [
        fontsize=12
        fontcolor="#ffffff"
        color="#E0AF68"
        shape=underline
        fontname="Hack, monospace"]
    edge [
        color="#E0AF68"
        arrowsize=0.5]
)";
    declare_nodes(os, ast);
    print_edges(os, ast);
    os << "}\n";
}
// ast_to_dotfile
inline void ast_to_dotfile(string fname, command const& ast) {
    // Open the file
    std::ofstream dotfile(fname);

    // Print the graph to the dotfile
    print_graph(dotfile, ast);

    // dotfile closed automatically when it falls out of scope
}
} // namespace imp
