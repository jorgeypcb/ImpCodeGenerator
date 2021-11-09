#pragma once
#include <imp/syntax_types.hpp>
#include <imp/util/overload_set.hpp>
#include <set>
#include <unordered_map>

namespace imp {
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

// Step 1: Get a list of all the variables in alphabetical order
// Step 2: assign addresses to variables and temporaries

// Step 1 - function to get all the variables
inline std::set<variable> get_variables(command const& program) {
    std::set<variable> variables;
    traverse(
        program,
        overload_set {
            [](auto const&) {},
            [&](variable const& var) { variables.insert(var); }});
    return variables;
}

// Step 2 - class to assign addresses to everything
// Assigns addresses to variables, temporaries, and constants
struct address_assigner {
    std::set<variable> variables;
    std::unordered_map<variable, address_t> var_addresses;
    void assign_address(variable& var, address_t) {
        var.address = var_addresses[var];
    }
    void assign_address(constant& c, address_t addr) { c.address = addr; }
    void assign_address(bool_const& c, address_t addr) { c.address = addr; }

    template <class Expr>
    void assign_address(unary_expr<Expr>& expr, address_t addr) {
        expr.address = addr;
        assign_address(expr.get_input(), addr + 1);
    }
    template <class... T>
    void assign_address(rva::variant<T...>& thing, address_t addr) {
        rva::visit(
            [addr, this](auto& thing) { assign_address(thing, addr); },
            thing);
    }
    template <class Expr>
    void assign_address(binary_expr<Expr>& expr, address_t addr) {
        expr.address = addr;
        assign_address(expr.get_left(), addr + 1);
        assign_address(expr.get_right(), addr + 2);
    }

    void assign_address(assignment<arith_expr>& ass, address_t addr) {
        assign_address(ass.dest, addr);
        assign_address(ass.value, addr);
    }
    void assign_address(skip_command&, address_t) {}
    void assign_address(if_command<bool_expr, command>& if_, address_t addr) {
        assign_address(if_.get_condition(), addr);
        assign_address(if_.when_true(), addr);
        assign_address(if_.when_true(), addr);
    }
    void assign_address(
        while_loop<bool_expr, command>& while_,
        address_t addr) {
        assign_address(while_.get_condition(), addr);
        assign_address(while_.get_body(), addr);
    }
    void assign_address(std::vector<command>& commands, address_t addr) {
        for (command& cmd : commands) {
            assign_address(cmd, addr);
        }
    }
    void assign_address(command& program) {
        variables = get_variables(program);
        var_addresses.reserve(variables.size());

        address_t addr = 0;
        for (auto const& var : variables) {
            var_addresses[var] = addr;
            addr++;
        }
        assign_address(program, addr);
    }
};
} // namespace imp
