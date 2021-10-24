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
    constexpr node_id(T const& node) noexcept
      : type_size(sizeof(T))
      , node_location((size_t)&node) {}
};

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
} // namespace imp

template <>
struct fmt::formatter<imp::node_id> {
    constexpr auto parse(format_parse_context& ctx) -> decltype(ctx.begin()) {
        return ctx.begin();
    }

    template <typename FormatContext>
    constexpr auto format(imp::node_id n, FormatContext& ctx) {
        return fmt::format_to(
            ctx.out(),
            "s{:02x}x{:x}",
            n.type_size,
            n.node_location);
    }
};
namespace imp {

// By default, there is no styling
std::string_view style_node(std::vector<command> const&) {
    return R"(label="[list of commands]")";
}
template <class Expr>
std::string_view style_node(assignment<Expr> const&) {
    return R"(fontname="Hack italic, monospace italic"; label="assignment")";
}
auto style_node(bool_const const& node) {
    return fmt::format(R"(fontcolor="#9ECE6A"; label="{}")", node.value);
}
auto style_node(constant const& node) {
    return fmt::format(R"(fontcolor="#9ECE6A"; label="{}")", node.value);
}
std::string_view style_node(while_loop<bool_expr, command> const&) {
    return R"(fontcolor="#BB9AF7"; label="while loop")";
}
std::string_view style_node(if_command<bool_expr, command> const&) {
    return R"(fontcolor="#BB9AF7"; label="conditional")";
}
std::string_view style_node(skip_command const&) {
    return R"(fontcolor="#BB9AF7"; label="skip")";
}
auto style_node(variable const& node) {
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
    os << fmt::format("    {} [{}];\n", get_id(node), style_node(node));
    for_each(node, [&](auto const& child) {
        declare_nodes(os, child);
    });
}

void print_edges(ostream& os, auto const& node) {
    for_each(node, [&](auto const& child) {
        os <<  fmt::format("    {} -> {};\n", get_id(node), get_id(child));
        print_edges(os, child);
    });
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
void ast_to_dotfile(string fname, command const& ast) {
    // Open the file
    std::ofstream dotfile(fname);

    // Print the graph to the dotfile
    print_graph(dotfile, ast);

    // dotfile closed automatically when it falls out of scope
}
} // namespace imp
