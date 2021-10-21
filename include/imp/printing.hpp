#pragma once

#include <imp/syntax_types.hpp>
#include <fmt/core.h>

namespace imp {

// Declare it ahead of time so we can use it in other functions
// The function itself could just go here b/c templates
void print_expr(auto const& expr);

void print_expr(constant const& c) {
    fmt::print("{}", c.value);
}
void print_expr(variable const& v) {
    fmt::print("{}", v.get_name());
}
void print_expr(binary_expr<arith_expr> const& expr) {
    print_expr(expr.get_left());
    fmt::print(" {} ", expr.get_op());
    print_expr(expr.get_right());
}

////// BOOL EXPR //////////

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
    fmt::print(" {} ", be.get_op());
    print_expr(be.get_right());
}

//print_expr(binary_expr<arith_expr>) already exists -
//need another print_expr for the fourth case in bool_expr (binary_expr<arith_expr>) ??

//////// COMMAND //////////

void print_expr(assignment<arith_expr> const& a) {
    print_expr(a.dest);
    fmt::print(" = ");
    print_expr(a.value);
    fmt::print(";");
}

void print_expr(skip_command const& s) {
    fmt::print("skip;");
}

void print_expr(if_command<bool_expr, command> const& ic) {
    fmt::print("if ");
    print_expr(ic.get_condition());
    fmt::print(" then\n");
    print_expr(ic.when_true());
    fmt::print("\nelse\n");
    print_expr(ic.when_false());
    fmt::print("fi\n");
}

void print_expr(while_loop<bool_expr, command> const& wl) {
    fmt::print("while ");
    print_expr(wl.get_condition());
    fmt::print(" do \n");
    print_expr(wl.get_body());
    fmt::print("\ndone\n");
}
void print_expr(std::vector<command> const& commands) {
    for(auto const& cmd : commands) {
        print_expr(cmd);
        fmt::print("\n");
    }
}

// Now we actually declare it
void print_expr(auto const& expr) {
    // This lambda is templated so it can accept any type that has a print_expr
    // function. It calls the correct version of print_expr by finding the right
    // overload
    auto visitor = [](auto const& v) { print_expr(v); };

    // We use rva::visit since we're using rva::variant
    rva::visit(visitor, expr);
}

} //namespace imp
