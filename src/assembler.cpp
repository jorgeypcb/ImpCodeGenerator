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
        char ch = st[0];
        switch (ch) {
            case '+': return {st.substr(1), ch};
            case '-': return {st.substr(1), ch};
            case '*': return {st.substr(1), ch};
        }
        return {};
    }};
constexpr auto parse_bool_op = noam::parser {
    [](noam::state_t st) -> noam::result<char> {
        if (st.empty()) {
            return {};
        }
        if (st.starts_with("and")) {
            return {st.substr(3), '&'};
        }
        if (st.starts_with("or")) {
            return {st.substr(2), '|'};
        }
        return {};
    }};
constexpr auto parse_comparison = noam::parser {
    [](noam::state_t st) -> noam::result<char> {
        if (st.empty()) {
            return {};
        }
        if (st.starts_with("=")) {
            return {st.substr(1), '='};
        }
        if (st.starts_with("<=")) {
            // L represents less than
            return {st.substr(2), 'L'};
        }
        if (st.starts_with(">=")) {
            // G represents greater than
            return {st.substr(2), 'G'};
        }
        if (st.starts_with('<')) {
            return {st.substr(1), '<'};
        }
        if (st.starts_with('>')) {
            return {st.substr(1), '>'};
        }
        return {};
    }};
constexpr auto parse_constant = noam::make<constant>(noam::parse_long_long);
constexpr auto parse_bool_const = noam::make<bool_const>(noam::parse_bool);

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
            if (noam::result<arith_expr> lx = parse_cons_or_var.parse(st)) {
                st = lx.get_state();
                // Read all whitespace
                st = noam::whitespace.parse(st).get_state();
                if (auto op = parse_op.parse(st)) {
                    st = op.get_state();
                    st = noam::whitespace.parse(st).get_state();

                    if (noam::result<arith_expr> rx = parse_arith_expr.parse(
                            st)) {
                        st = rx.get_state();
                        return noam::result {
                            st,
                            arith_expr {binary_expr<arith_expr> {
                                std::move(lx).get_value(),
                                std::move(rx).get_value(),
                                op.get_value()}}};
                    }
                } else {
                    return lx;
                }
            }
            return {};
        }};
    });
constexpr auto parse_bool_expr = noam::recurse<
    bool_expr>([](auto parse_bool_expr) {
    return noam::parser {[=](noam::state_t st) -> noam::result<bool_expr> {
        st = noam::whitespace.parse(st).get_state();
        if (st.starts_with("not")) {
            st.remove_prefix(3);
            st = noam::whitespace.parse(st).get_state();
            if (noam::result<bool_expr> expr = parse_bool_expr.parse(st)) {
                return noam::result<bool_expr> {
                    expr.get_state(),
                    unary_expr<bool_expr> {expr.get_value(), '!'}};
            } else {
                return {};
            }
        }
        if (noam::result<bool_const> lx = parse_bool_const.parse(st)) {
            st = lx.get_state();
            // Read all whitespace
            st = noam::whitespace.parse(st).get_state();
            if (auto op = parse_bool_op.parse(st)) {
                st = op.get_state();
                st = noam::whitespace.parse(st).get_state();

                if (noam::result<bool_expr> rx = parse_bool_expr.parse(st)) {
                    st = rx.get_state();
                    return noam::result {
                        st,
                        bool_expr {binary_expr<bool_expr> {
                            std::move(lx).get_value(),
                            std::move(rx).get_value(),
                            op.get_value()}}};
                }
            } else {
                return noam::result<bool_expr> {lx.get_state(), lx.get_value()};
            }
        }
        if (noam::result<arith_expr> lx = parse_arith_expr.parse(st)) {
            st = lx.get_state();
            st = noam::whitespace.parse(st).get_state();
            if (auto op = parse_comparison.parse(st)) {
                st = op.get_state();
                st = noam::whitespace.parse(st).get_state();

                if (noam::result<arith_expr> rx = parse_arith_expr.parse(st)) {
                    st = rx.get_state();
                    // Update lx, then check for a right hand side
                    auto lx2 = bool_expr {binary_expr<arith_expr> {
                        std::move(lx).get_value(),
                        std::move(rx).get_value(),
                        op.get_value()}};
                    st = noam::whitespace.parse(st).get_state();
                    if (auto op = parse_bool_op.parse(st)) {
                        st = op.get_state();
                        st = noam::whitespace.parse(st).get_state();

                        if (noam::result<bool_expr> rx = parse_bool_expr.parse(
                                st)) {
                            st = rx.get_state();
                            return noam::result {
                                st,
                                bool_expr {binary_expr<bool_expr> {
                                    std::move(lx2),
                                    std::move(rx).get_value(),
                                    op.get_value()}}};
                        }
                    } else {
                        return noam::result<bool_expr> {st, lx2};
                    }
                }
            }
        }
        return {};
    }};
});
} // namespace imp
int main() {
    auto test = imp::parse_bool_expr.parse("not 3 + 5 * 7 > 10 or 3 > x");
    if (test) {
        auto val = test.get_value();

        imp::print_graph(std::cout, val);
    } else {
        fmt::print("failed to parse thing\n");
    }
}
