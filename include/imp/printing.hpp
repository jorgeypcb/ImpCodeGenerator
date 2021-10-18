#pragma once

#include <imp/syntax_types.hpp>
#include <fmt/core.h>

namespace imp {
// I copied the definitions here so that we could reference them while implementing
// printing
using arith_expr = rva::variant<
    constant,                  // Integral constant
    variable,                  // Variable
    binary_expr<rva::self_t>>; // Binary operation on to arithmatic expressions

using bool_expr = rva::variant<
    bool_const,               // Boolean constant
    unary_expr<rva::self_t>,  // Unary operation on boolean expression
    binary_expr<rva::self_t>, // Binary operation on boolean expression
    binary_expr<arith_expr>>; // Comparison on arithmetic expressions

using command = rva::variant<
    assignment<arith_expr>,             // Assignment command
    skip_command,                       // Skip command
    if_command<bool_expr, rva::self_t>, // If statement
    while_loop<bool_expr, rva::self_t>, // while loop
    std::vector<rva::self_t>>;          // List of commands separated by ;

// Declare it ahead of time so we can use it in other functions
void print_expr(arith_expr const& expr);

void print_expr(constant const& c) {
    fmt::print("{}", c.value);
}
void print_expr(variable const& v) {
    fmt::print("{}", v.get_name());
}
void print_expr(binary_expr<arith_expr> const& expr) {
    print_expr(expr.get_left());
    fmt::print("{}", expr.get_op());
    print_expr(expr.get_right());
}

// Now we actually declare it
void print_expr(arith_expr const& expr) {
    // This lambda is templated so it can accept any type that has a print_expr
    // function. It calls the correct version of print_expr by finding the right
    // overload
    auto visitor = [](auto const& v) { print_expr(v); };

    // We use rva::visit since we're using rva::variant
    rva::visit(visitor, expr);
}

}
