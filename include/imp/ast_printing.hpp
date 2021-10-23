#pragma once

#include <fmt/core.h>
#include <fstream>
#include <imp/syntax_types.hpp>
#include <iostream>
#include <string>

namespace imp {
using std::ostream;
using std::string;

template <class T>
size_t get_id(const T& anything) {
    std::cout << "Calling " << __PRETTY_FUNCTION__ << "\n\twith "
              << (size_t)&anything << " and " << sizeof(T) * 11 << '\n';
    return (size_t)&anything + sizeof(T) * 11;
}
template <class... T>
size_t get_id(const rva::variant<T...>& anything) {
    auto visitor = [](auto const& x) -> size_t { return get_id(x); };
    return rva::visit(visitor, anything);
}

// declare_nodes forward declarations


template <class... T>
void declare_nodes(ostream& os, rva::variant<T...> const& node);
// void declare_nodes(ostream& os, arith_expr const& node);

// void declare_nodes(ostream& os, binary_expr<arith_expr> const& node);

// void declare_nodes(ostream& os, bool_expr const& node);

// void declare_nodes(ostream& os, command const& node);

// declare_node implementations
void declare_nodes(ostream& os, constant const& node) {
    os << get_id(node) << " [label = \"" << node.value << "\"];\n";
}
void declare_nodes(ostream& os, variable const& node) {
    os << get_id(node) << " [label = \"" << node.get_name() << "\"];\n";
}
void declare_nodes(ostream& os, assignment<arith_expr> const& node) {
    os << get_id(node) << " [label = \"assignment\"];\n";
    declare_nodes(os, node.dest);
    declare_nodes(os, node.value);
}

void declare_nodes(ostream& os, binary_expr<arith_expr> const& node) {
    os << get_id(node) << " [label = \"" << node.get_op() << "\"];\n";
    declare_nodes(os, node.get_left());
    declare_nodes(os, node.get_right());
}

// os << get_id(node) << " [label = \"" <<  << "\"];\n";
void declare_nodes(ostream& os, bool_const const& node){
    os << get_id(node) << " [label = \"" << node.value << "\"];\n";
}

void declare_nodes(ostream& os, unary_expr<bool_expr> const& node){
    os << get_id(node) << " [label = \"" << node.get_op() << "\"];\n";
    declare_nodes(os, node.get_input());
}

void declare_nodes(ostream& os, binary_expr<bool_expr> const& node){
    os << get_id(node) << " [label = \"" << node.get_op() << "\"];\n";
    declare_nodes(os, node.get_left());
    declare_nodes(os, node.get_right());
}

void declare_nodes(ostream& os, const imp::skip_command& node) {
    os << get_id(node) << " [label = \"" << "skip" << "\"];\n";
}

void declare_nodes(ostream& os, if_command<bool_expr, command> const& node){
    os << get_id(node) << " [label = \"" << "if_condition" << "\"];\n";
    declare_nodes(os, node.get_condition());
    declare_nodes(os, node.when_false());
    declare_nodes(os, node.when_true());
} 

void declare_nodes(ostream& os, while_loop<bool_expr, command> const& node){
    os << get_id(node) << " [label = \"" << "while_condition" << "\"];\n";
    declare_nodes(os, node.get_condition());
    declare_nodes(os, node.get_body());
}

void declare_nodes(ostream& os, std::vector<command> const& victor) {
    os << get_id(victor) << " [label = \"list of commands\"];\n";
    for (auto& item : victor) {
        declare_nodes(os, item);
    }
}

template <class... T>
void declare_nodes(ostream& os, rva::variant<T...> const& node) {
    std::cout << "declare nodes\n";
    auto visitor = [&](auto const& v) { declare_nodes(os, v); };
    rva::visit(visitor, node);
}
// void declare_nodes(ostream& os, arith_expr const& node) {
//     std::cout << "declare nodes\n";
//     auto visitor = [&](auto const& v) { declare_nodes(os, v); };
//     rva::visit(visitor, node);
// }

// void declare_nodes(ostream& os, command const& node) {
//     std::cout << "declare nodes\n";
//     auto visitor = [&](auto const& v) { declare_nodes(os, v); };
//     rva::visit(visitor, node);
// }

// operator<< forward declarations

ostream& operator<<(ostream& os, command const& expr);

// operator<< implementations

ostream& operator<<(ostream& os, assignment<arith_expr> const& a) {
    os << get_id(a) << "->" << get_id(a.dest) << '\n';
    os << get_id(a) << "->" << get_id(a.value) << '\n';
    return os;
}

ostream& operator<<(ostream& os, binary_expr<arith_expr> const& a) {
    os << get_id(a) << "->" << get_id(a.get_left()) << '\n';
    os << get_id(a) << "->" << get_id(a.get_right()) << '\n';
    return os;
}

ostream& operator<<(ostream& os, std::vector<command> const& victor) {
    for (auto& item : victor) {
        os << get_id(victor) << "->" << get_id(item) << "\n";
        os << item;
    }
    return os;
}

ostream& operator<<(ostream& os, command const& expr) {
    std::cout << "calling visitor\n";
    auto visitor = [&](auto const& v) { os << v; };
    rva::visit(visitor, expr);
    return os;
}


// ast_to_dotfile
void ast_to_dotfile(string fname, command const& ast) {
    std::ofstream dotfile;
    int new_node = 0;
    dotfile.open(fname);
    dotfile << "digraph g { \n";
    declare_nodes(dotfile, ast);
    dotfile << ast;
    dotfile << "\n}";
    dotfile.close();
}

} // namespace imp
