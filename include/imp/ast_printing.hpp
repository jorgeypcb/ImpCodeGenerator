#pragma once

#include <fmt/format.h>

#include <fstream>
#include <imp/syntax_types.hpp>
#include <iostream>
#include <string>

namespace imp {
using std::ostream;
using std::string;

struct node_id {
    size_t type_size;
    size_t node_location;
    template <class T>
    node_id(T const& node) noexcept
      : type_size(sizeof(T))
      , node_location((size_t)&node) {}
};

ostream& operator<<(ostream& os, node_id n) {
    // The worst case size is 34 bytes,
    // however the actual size will probably be 16 bytes
    char buffer[40];
    auto result = fmt::format_to_n(
        buffer,
        sizeof(buffer),
        "s{:02x}x{:12x}",
        n.type_size,
        n.node_location);
    return os << std::string_view(buffer, result.size);
}

template <class T>
node_id get_id(const T& anything) {
    // This creates a unique identifier by combining the location of the
    // variable in memory with the size of the type. This ensures that types
    // that contain other types produce a unique identifier.
    return node_id(anything);
}
template <class... T>
node_id get_id(const rva::variant<T...>& anything) {
    auto visitor = [](auto const& x) { return get_id(x); };
    return rva::visit(visitor, anything);
}

// Forward declaration of declare_nodes and print_edges
template <class... T>
void declare_nodes(ostream& os, rva::variant<T...> const& node);
template <class... T>
void print_edges(ostream& os, rva::variant<T...> const& expr);

// declare_node implementations
void declare_nodes(ostream& os, constant const& node) {
    os << "    "; // Print space before line
    os << get_id(node) << " [label = \"" << node.value << "\"];\n";
}
void declare_nodes(ostream& os, variable const& node) {
    os << "    "; // Print space before line
    os << get_id(node) << " [label = \"" << node.get_name() << "\"];\n";
}
void declare_nodes(ostream& os, assignment<arith_expr> const& node) {
    os << "    "; // Print space before line
    os << get_id(node) << " [label = \"assignment\"];\n";
    declare_nodes(os, node.dest);
    declare_nodes(os, node.value);
}

void declare_nodes(ostream& os, binary_expr<arith_expr> const& node) {
    os << "    "; // Print space before line
    os << get_id(node) << " [label = \"" << node.get_op() << "\"];\n";
    declare_nodes(os, node.get_left());
    declare_nodes(os, node.get_right());
}

void declare_nodes(ostream& os, bool_const const& node) {
    os << "    "; // Print space before line
    os << get_id(node) << " [label = \"" << node.value << "\"];\n";
}

void declare_nodes(ostream& os, unary_expr<bool_expr> const& node) {
    os << "    "; // Print space before line
    os << get_id(node) << " [label = \"" << node.get_op() << "\"];\n";
    declare_nodes(os, node.get_input());
}

void print_edges(ostream& os, unary_expr<bool_expr> const& a) {
    os << "    "; // Print space before line
    os << get_id(a) << " -> " << get_id(a.get_input()) << "\n";
    print_edges(os, a.get_input());
}

void declare_nodes(ostream& os, binary_expr<bool_expr> const& node) {
    os << "    "; // Print space before line
    os << get_id(node) << " [label = \"" << node.get_op() << "\"];\n";
    declare_nodes(os, node.get_left());
    declare_nodes(os, node.get_right());
}

void print_edges(ostream& os, binary_expr<bool_expr> const& a) {
    os << "    "; // Print space before line
    os << get_id(a) << " -> " << get_id(a.get_left()) << "\n";
    os << "    "; // Print space before line
    os << get_id(a) << " -> " << get_id(a.get_right()) << "\n";
    print_edges(os, a.get_left());
    print_edges(os, a.get_right());
}

void declare_nodes(ostream& os, const imp::skip_command& node) {
    os << "    "; // Print space before line
    os << get_id(node) << " [label = \""
       << "skip"
       << "\"];\n";
}

// These types have no children, and so print_edges does nothing
void print_edges(ostream& os, bool_const const& a) {}
void print_edges(ostream& os, skip_command const& a) {}
void print_edges(ostream& os, variable const& a) {}
void print_edges(ostream& os, constant const& a) {}

void declare_nodes(ostream& os, if_command<bool_expr, command> const& node) {
    os << "    "; // Print space before line
    os << get_id(node) << " [label = \""
       << "if_condition"
       << "\"];\n";
    declare_nodes(os, node.get_condition());
    declare_nodes(os, node.when_false());
    declare_nodes(os, node.when_true());
}

void print_edges(ostream& os, if_command<bool_expr, command> const& a) {
    os << "    "; // Print space before line
    os << get_id(a) << " -> " << get_id(a.get_condition()) << "\n";
    os << "    "; // Print space before line
    os << get_id(a) << " -> " << get_id(a.when_false()) << "\n";
    os << "    "; // Print space before line
    os << get_id(a) << " -> " << get_id(a.when_true()) << "\n";
    print_edges(os, a.get_condition());
    print_edges(os, a.when_false());
    print_edges(os, a.when_true());
}

void declare_nodes(ostream& os, while_loop<bool_expr, command> const& node) {
    os << "    "; // Print space before line
    os << get_id(node) << " [label = \""
       << "while loop"
       << "\"];\n";
    declare_nodes(os, node.get_condition());
    declare_nodes(os, node.get_body());
}

void print_edges(ostream& os, while_loop<bool_expr, command> const& a) {
    os << "    "; // Print space before line
    os << get_id(a) << " -> " << get_id(a.get_condition()) << "\n";
    os << "    "; // Print space before line
    os << get_id(a) << " -> " << get_id(a.get_body()) << "\n";
    print_edges(os, a.get_condition());
    print_edges(os, a.get_body());
}

void declare_nodes(ostream& os, std::vector<command> const& victor) {
    os << "    "; // Print space before line
    os << get_id(victor) << " [label = \"[list of commands]\"];\n";
    for (auto& item : victor) {
        declare_nodes(os, item);
    }
}

template <class... T>
void declare_nodes(ostream& os, rva::variant<T...> const& node) {
    auto visitor = [&](auto const& v) { declare_nodes(os, v); };
    rva::visit(visitor, node);
}

void print_edges(ostream& os, binary_expr<arith_expr> const& a) {
    os << "    "; // Print space before line
    os << get_id(a) << " -> " << get_id(a.get_left()) << '\n';
    os << "    "; // Print space before line
    os << get_id(a) << " -> " << get_id(a.get_right()) << '\n';
    print_edges(os, a.get_left());
    print_edges(os, a.get_right());
}

void print_edges(ostream& os, assignment<arith_expr> const& a) {
    os << "    "; // Print space before line
    os << get_id(a) << " -> " << get_id(a.dest) << '\n';
    os << "    "; // Print space before line
    os << get_id(a) << " -> " << get_id(a.value) << '\n';
    print_edges(os, a.value);
}

void print_edges(ostream& os, std::vector<command> const& victor) {
    for (auto& item : victor) {
        os << "    "; // Print space before line
        os << get_id(victor) << " -> " << get_id(item) << "\n";
        print_edges(os, item);
    }
}

template <class... T>
void print_edges(ostream& os, rva::variant<T...> const& expr) {
    auto visitor = [&](auto const& v) { print_edges(os, v); };
    rva::visit(visitor, expr);
}

void print_graph(ostream& os, command const& ast) {
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
        fontname="Hack, Fira Code, monospace"]
    edge [
        color="#BB9AF7"
        arrowsize=0.5]
)";
    declare_nodes(os, ast);
    print_edges(os, ast);
    os << "}\n";
}
// ast_to_dotfile
void ast_to_dotfile(string fname, command const& ast) {
    // Open the file
    std::ofstream dotfile(fname);

    // Print the graph to the dotfile
    print_graph(dotfile, ast);

    // dotfile closed automatically when it falls out of scope
}
} // namespace imp
