#pragma once
#include <imp/parsers/arith_expr.hpp>

namespace imp {
using bool_expr = rva::variant<
    bool_const,               // Boolean constant
    unary_expr<rva::self_t>,  // Unary operation on boolean expression
    binary_expr<rva::self_t>, // Binary operation on boolean expression
    binary_expr<arith_expr>>; // Comparison on arithmetic expressions


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
}
