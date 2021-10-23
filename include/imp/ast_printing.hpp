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
    os << "s" << n.type_size << "x" << n.node_location;
    return os;
}

template <class T>
node_id get_id(const T& anything) {
    std::cout << "Calling " << __PRETTY_FUNCTION__ << "\n\twith "
              << (size_t)&anything << " and " << sizeof(T) * 11 << '\n';
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

// declare_nodes forward declarations


template <class... T>
void declare_nodes(ostream& os, rva::variant<T...> const& node);
template <class... T>
ostream& operator<<(ostream& os, rva::variant<T...> const& expr);
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
void declare_nodes(ostream& os, bool_const const& node) {
    os << get_id(node) << " [label = \"" << node.value << "\"];\n";
}

ostream& operator<<(ostream& os, bool_const const& a) { return os; }

void declare_nodes(ostream& os, unary_expr<bool_expr> const& node) {
    os << get_id(node) << " [label = \"" << node.get_op() << "\"];\n";
    declare_nodes(os, node.get_input());
}

ostream& operator<<(ostream& os, unary_expr<bool_expr> const& a) {
    os << get_id(a) << "->" << get_id(a.get_input()) << "\n";
    os << a.get_input();
    return os;
}

void declare_nodes(ostream& os, binary_expr<bool_expr> const& node) {
    os << get_id(node) << " [label = \"" << node.get_op() << "\"];\n";
    declare_nodes(os, node.get_left());
    declare_nodes(os, node.get_right());
}

ostream& operator<<(ostream& os, binary_expr<bool_expr> const& a) {
    os << get_id(a) << "->" << get_id(a.get_left()) << "\n";
    os << get_id(a) << "->" << get_id(a.get_right()) << "\n";
    os << a.get_left();
    os << a.get_right();
    return os;
}

void declare_nodes(ostream& os, const imp::skip_command& node) {
    os << get_id(node) << " [label = \""
       << "skip"
       << "\"];\n";
}

ostream& operator<<(ostream& os, skip_command const& a) { return os; }

ostream& operator<<(ostream& os, variable const& a) { return os; }

ostream& operator<<(ostream& os, constant const& a) { return os; }

void declare_nodes(ostream& os, if_command<bool_expr, command> const& node) {
    os << get_id(node) << " [label = \""
       << "if_condition"
       << "\"];\n";
    declare_nodes(os, node.get_condition());
    declare_nodes(os, node.when_false());
    declare_nodes(os, node.when_true());
}

ostream& operator<<(ostream& os, if_command<bool_expr, command> const& a) {
    os << get_id(a) << "->" << get_id(a.get_condition()) << "\n";
    os << get_id(a) << "->" << get_id(a.when_false()) << "\n";
    os << get_id(a) << "->" << get_id(a.when_true()) << "\n";
    os << a.get_condition();
    os << a.when_false();
    os << a.when_true();
    return os;
}

void declare_nodes(ostream& os, while_loop<bool_expr, command> const& node) {
    os << get_id(node) << " [label = \""
       << "while_condition"
       << "\"];\n";
    declare_nodes(os, node.get_condition());
    declare_nodes(os, node.get_body());
}

ostream& operator<<(ostream& os, while_loop<bool_expr, command> const& a) {
    os << get_id(a) << "->" << get_id(a.get_condition()) << "\n";
    os << get_id(a) << "->" << get_id(a.get_body()) << "\n";
    os << a.get_condition();
    os << a.get_body();
    return os;
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

// operator<< implementations



ostream& operator<<(ostream& os, binary_expr<arith_expr> const& a) {
    os << get_id(a) << "->" << get_id(a.get_left()) << '\n';
    os << get_id(a) << "->" << get_id(a.get_right()) << '\n';
    os << a.get_left();
    os << a.get_right();
    return os;
}

ostream& operator<<(ostream& os, assignment<arith_expr> const& a) {
    os << get_id(a) << "->" << get_id(a.dest) << '\n';
    os << get_id(a) << "->" << get_id(a.value) << '\n';
    os << a.value;
    return os;
}

ostream& operator<<(ostream& os, std::vector<command> const& victor) {
    for (auto& item : victor) {
        os << get_id(victor) << "->" << get_id(item) << "\n";
        os << item;
    }
    return os;
}

template <class... T>
ostream& operator<<(ostream& os, rva::variant<T...> const& expr) {
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
    dotfile << "}";
    dotfile.close();
}

} // namespace imp
