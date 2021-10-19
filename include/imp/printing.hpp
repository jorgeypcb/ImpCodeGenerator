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

////// BOOL EXPR //////////
void print_expr(bool_expr const& expr);

void print_expr(bool_const const& bc) {
    fmt::print("{}", bc.value);
}

void print_expr(unary_expr<bool_expr> const& ue) {
    print_expr(ue.get_input());
    fmt::print("{}", ue.get_op());
}

//same as print_expr(binary_expr<arith_expr>)
void print_expr(binary_expr<bool_expr> const& be) {
    print_expr(be.get_left());
    fmt::print("{}", be.get_op());
    print_expr(be.get_right());
}

//print_expr(binary_expr<arith_expr>) already exists -
//need another print_expr for the fourth case in bool_expr (binary_expr<arith_expr>) ??

void print_expr(bool_expr const& expr) {
    auto visitor = [](auto const& v) { print_expr(v); };
    rva::visit(visitor,expr);
}

//////// COMMAND //////////

void print_expr(command const& expr);

void print_expr(assignment<arith_expr> const& a) {
    print_expr(a.dest);
    print_expr(a.value);
}

void print_expr(skip_command const& s) {
    fmt::print("skip_command");
}

void print_expr(if_command<bool_expr, command> const& ic) {
    print_expr(ic.when_true());
    print_expr(ic.when_false());
    print_expr(ic.get_condition());
}

void print_expr(while_loop<bool_expr, command> const& wl) {
    print_expr(wl.get_condition());
    print_expr(wl.get_body());
}

void print_expr(command const& expr) {
    auto visitor = [](auto const& v) { print_expr(v); };
    rva::visit(visitor,expr);
}

}
