#pragma once

#include <memory>
#include <noam/combinators.hpp>
#include <rva/variant.hpp>
#include <string_view>

#include <imp/copyable_ptr.hpp>

namespace imp {
struct constant {
    long long value;
};
struct bool_const {
    bool value;
};
struct variable {
    std::string_view name;
    std::string_view get_name() const {
        return name;
    }
};
struct comment : std::string_view {};

template <class T>
struct unary_expr {
    struct expr_data {
        T value;
        char op;
    };
    copyable_ptr<expr_data> data_ptr;
    T const& get_input() const {
        return data_ptr->value;
    }
    char get_op() const {
        return data_ptr->op;
    }
};
template <class T>
struct binary_expr {
    struct expr_data {
        T left;
        T right;
        char op;
    };
    copyable_ptr<expr_data> data_ptr;

    T const& get_left() const {
        return data_ptr->left;
    }
    T const& get_right() const {
        return data_ptr->right;
    }
    char get_op() const {
        return data_ptr->op;
    }
};
struct skip_command {};

template <class BExpr, class Cmd>
struct if_command {
    struct data {
        BExpr condition;
        Cmd when_true;
        Cmd when_false;
    };
    copyable_ptr<data> data_ptr;
    BExpr const& get_condition() const {
        return data_ptr->condition;
    }
    Cmd const& when_true() const {
        return data_ptr->when_true;
    }
    Cmd const& when_false() const {
        return data_ptr->when_false;
    }
};
template <class BExpr, class Cmd>
struct while_loop {
    struct data {
        BExpr condition;
        Cmd body;
    };
    copyable_ptr<data> data_ptr;
    BExpr const& get_condition() const {
        return data_ptr->condition;
    }
    Cmd const& get_body() const {
        return data_ptr->body;
    }
};
template <class Expr>
struct assignment {
    variable dest;
    Expr value;
};


// clang-format off
/**
 * Arithmetic expressions a ::= x | n | a1 o_a a2
 * Boolean expressions b ::= true | false | not b1 | b1 o_b b2 | a1 o_r a2
 * Commands c ::= x := a | skip | c1 ; c2 | if b then c1 else c2 fi | while b do c1 od
 *
 * where x ranges over (scalar) variable names; n ranges over integer literals;
 * o_a ranges over binary arithmetic operators, namely +, −, ×;
 * true and false are boolean literals; not is a unary
 * boolean operator; o_b ranges over binary boolean operators, namely ∧, ∨;
 * o_r ranges over binary relational operators, namely =, <, ≤, >, ≥.
 */


// clang-format on
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

static_assert(std::is_copy_constructible_v<arith_expr>);
static_assert(std::is_copy_constructible_v<bool_expr>);
static_assert(std::is_copy_constructible_v<command>);
static_assert(std::is_move_constructible_v<arith_expr>);
static_assert(std::is_move_constructible_v<bool_expr>);
static_assert(std::is_move_constructible_v<command>);
static_assert(std::is_copy_assignable_v<arith_expr>);
static_assert(std::is_copy_assignable_v<bool_expr>);
static_assert(std::is_copy_assignable_v<command>);
static_assert(std::is_move_assignable_v<arith_expr>);
static_assert(std::is_move_assignable_v<bool_expr>);
static_assert(std::is_move_assignable_v<command>);
} // namespace imp