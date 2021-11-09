#pragma once
#include <imp/syntax_types.hpp>
#include <imp/util/overload_set.hpp>
#include <set>

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
inline std::set<variable> get_variables(command const& program) {
    std::set<variable> variables;
    traverse(program, overload_set {
        [](auto const&) {},
        [&](variable const& var) { variables.insert(var); } 
    });
    return variables;
}


} // namespace imp
