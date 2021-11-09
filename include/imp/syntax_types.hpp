#pragma once

#include <memory>
#include <noam/combinators.hpp>
#include <rva/variant.hpp>
#include <string_view>
#include <utility>

#include <imp/copyable_ptr.hpp>

namespace imp {
// Memory location in the intermediate representation
using address_t = int;

struct constant {
    long long value;
    // Address in the intermediate representation
    address_t address {};

    constexpr void for_each(auto&& func) const noexcept {}
};
struct bool_const {
    bool value;
    // Address in the intermediate representation
    address_t address {};

    constexpr void for_each(auto&& func) const noexcept {}
};
struct variable {
    auto operator<=>(variable const&) const = default;
    bool operator==(variable const&) const = default;

    std::string_view name;
    // Address in the intermediate representation
    address_t address {};
    std::string_view get_name() const { return name; }

    constexpr void for_each(auto&& func) const noexcept {}
};
struct comment : std::string_view {

    constexpr void for_each(auto&& func) const noexcept {}
};

template <class T>
struct unary_expr {
    copyable_ptr<T> data_ptr;
    char op;
    // Address in the intermediate representation
    address_t address {};
    unary_expr() = default;
    unary_expr(unary_expr const&) = default;
    unary_expr(unary_expr&&) = default;
    unary_expr(T const& value, char op)
      : data_ptr(value)
      , op(op) {}
    unary_expr(T&& value, char op)
      : data_ptr(std::move(value))
      , op(op) {}
    T& get_input() { return *data_ptr; }
    T const& get_input() const { return *data_ptr; }
    char get_op() const { return op; }

    unary_expr& operator=(unary_expr const&) = default;
    unary_expr& operator=(unary_expr&&) = default;
    constexpr void for_each(auto&& func) const noexcept { func(get_input()); }
};
template <class T>
struct binary_expr {
    struct data {
        T left;
        T right;
    };
    copyable_ptr<data> data_ptr;
    // Address in the intermediate representation
    address_t address {};
    char op;
    binary_expr() = default;
    binary_expr(binary_expr const&) = default;
    binary_expr(binary_expr&&) = default;
    binary_expr(T&& left, T&& right, char op)
      : data_ptr(data {std::move(left), std::move(right)})
      , op(op) {}
    binary_expr& operator=(binary_expr const&) = default;
    binary_expr& operator=(binary_expr&&) = default;

    T& get_left() { return data_ptr->left; }
    T& get_right() { return data_ptr->right; }
    T const& get_left() const { return data_ptr->left; }
    T const& get_right() const { return data_ptr->right; }
    char get_op() const { return op; }

    constexpr void for_each(auto&& func) const noexcept {
        func(get_left());
        func(get_right());
    }
};
struct skip_command {
    constexpr void for_each(auto&& func) const noexcept {}
};

template <class BExpr, class Cmd>
struct if_command {
    struct data {
        BExpr condition;
        Cmd when_true;
        Cmd when_false;
    };
    copyable_ptr<data> data_ptr;

    if_command() = default;
    if_command(if_command const&) = default;
    if_command(if_command&&) = default;
    if_command(BExpr&& cond, Cmd&& when_true, Cmd&& when_false)
      : data_ptr(
          data {std::move(cond), std::move(when_true), std::move(when_false)}) {
    }

    if_command& operator=(if_command const&) = default;
    if_command& operator=(if_command&&) = default;

    BExpr& get_condition() { return data_ptr->condition; }
    Cmd& when_true() { return data_ptr->when_true; }
    Cmd& when_false() { return data_ptr->when_false; }
    BExpr const& get_condition() const { return data_ptr->condition; }
    Cmd const& when_true() const { return data_ptr->when_true; }
    Cmd const& when_false() const { return data_ptr->when_false; }

    constexpr void for_each(auto&& func) const noexcept {
        func(get_condition());
        func(when_true());
        func(when_false());
    }
};
template <class BExpr, class Cmd>
struct while_loop {
    struct data {
        BExpr condition;
        Cmd body;
    };
    copyable_ptr<data> data_ptr;

    while_loop() = default;
    while_loop(while_loop const&) = default;
    while_loop(while_loop&&) = default;
    while_loop(BExpr&& cond, Cmd&& when_true)
      : data_ptr(data {std::move(cond), std::move(when_true)}) {}

    while_loop& operator=(while_loop const&) = default;
    while_loop& operator=(while_loop&&) = default;

    BExpr& get_condition() { return data_ptr->condition; }
    Cmd& get_body() { return data_ptr->body; }
    BExpr const& get_condition() const { return data_ptr->condition; }
    Cmd const& get_body() const { return data_ptr->body; }

    constexpr void for_each(auto&& func) const noexcept {
        func(get_condition());
        func(get_body());
    }
};
template <class Expr>
struct assignment {
    variable dest;
    Expr value;

    constexpr void for_each(auto&& func) const noexcept {
        func(dest);
        func(value);
    }
};

template <class T, class F>
void for_each(T const& item, F&& func) {
    item.for_each(func);
}

template <class T, class F>
void for_each(std::vector<T> const& vect, F&& func) {
    for (auto& item : vect) {
        func(item);
    }
}

template <class... T, class F>
void for_each(rva::variant<T...> const& v, F&& func) {
    rva::visit([&](auto const& node) { for_each(node, func); }, v);
}

// Traverses an AST
template <class T, class F>
void traverse(T const& tree, F&& func) {
    func(tree);
    // Apply self recursively to all of the branches
    for_each(tree, [&](auto const& branch) { traverse(branch, func); });
}
// Traverses an AST depth-first: branches are traversed before the node itself
template <class T, class F>
void traverse_depth_first(T const& tree, F&& func) {
    for_each(tree, [&](auto const& branch) { traverse(branch, func); });
    func(tree);
}

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

// Get the address of an arithmetic expression and a boolean expression
// respectively
inline address_t get_address(arith_expr const& ex) {
    return rva::visit(
        [](auto&& thing) -> address_t { return thing.address; },
        ex);
}
inline address_t get_address(bool_expr const& ex) {
    return rva::visit(
        [](auto&& thing) -> address_t { return thing.address; },
        ex);
}
} // namespace imp

// Variables are hashed according to their name
template <>
struct std::hash<imp::variable> {
    size_t operator()(imp::variable const& v) const noexcept {
        return std::hash<std::string_view> {}(v.get_name());
    }
};
