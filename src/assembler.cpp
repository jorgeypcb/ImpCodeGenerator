#include <fmt/core.h>
#include <imp/ast_printing.hpp>
#include <imp/syntax_types.hpp>
#include <noam/co_await.hpp>
#include <noam/combinators.hpp>
#include <noam/intrinsics.hpp>
#include <noam/result_types.hpp>
#include <noam/type_traits.hpp>
#include <rva/variant.hpp>

namespace imp {
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

constexpr auto parse_op = noam::parser {
    [](noam::state_t st) -> noam::result<char> {
        if (st.empty()) {
            return {};
        }
        fmt::print("Parsing '{}'\n", st);
        char ch = st[0];
        fmt::print("ch: {}\n", ch);
        switch (ch) {
            case '+': return {st.substr(1), ch};
            case '-': return {st.substr(1), ch};
            case '*': return {st.substr(1), ch};
        }
        fmt::print("{} wasn't an operator\n", ch);
        return {};
    }};
constexpr auto parse_constant = noam::make<constant>(noam::parse_long_long);
constexpr auto parse_variable = noam::parser {
    [](noam::state_t st) -> noam::result<variable> {
        std::string_view sv = st;

        size_t len = 0;
        while (len < sv.length() && isalnum(sv[len])) {
            len++;
        }
        if (len > 0) {
            return {
                st.substr(len),
                variable {std::string_view {sv.substr(0, len)}}};
        } else {
            return {};
        }
    }};

constexpr auto parse_cons_or_var = noam::either<arith_expr>(
    parse_constant,
    parse_variable);
constexpr auto parse_arith_expr = noam::recurse<arith_expr>(
    [](auto parse_arith_expr) {
        return noam::parser {[=](noam::state_t st) -> noam::result<arith_expr> {
            st = noam::whitespace.parse(st).get_state();
            if (noam::result<arith_expr> value = parse_cons_or_var.parse(st)) {
                st = value.get_state();
                // Read all whitespace
                st = noam::whitespace.parse(st).get_state();

                fmt::print("Parsed value. St: '{}'\n", st);
                if (auto op = parse_op.parse(st)) {

                    fmt::print("Parsed op\n");
                    st = op.get_state();
                    st = noam::whitespace.parse(st).get_state();

                    if (noam::result<arith_expr> left_expr = parse_arith_expr
                                                                 .parse(st)) {
                        st = left_expr.get_state();
                        fmt::print("Obtained left expr");
                        return noam::result {
                            st,
                            arith_expr {binary_expr<arith_expr> {
                                std::move(value).get_value(),
                                std::move(left_expr).get_value(),
                                op.get_value()}}};
                    }
                } else {
                    rva::visit(
                        [](auto& v) {
                            if constexpr (std::is_same_v<
                                              std::decay_t<decltype(v)>,
                                              constant>) {
                                fmt::print(
                                    "Returning simple value: {}\n",
                                    v.value);
                            }
                        },
                        value.get_value());
                    return value;
                }
            }
            return {};
        }};
    });
} // namespace imp
int main() {
    auto test = imp::parse_arith_expr.parse("3 + 5 * 7");
    if (test) {
        auto val = test.get_value();

        imp::print_graph(std::cout, val);
    } else {
        fmt::print("failed to parse thing\n");
    }
}
