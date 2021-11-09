#pragma once
#include <imp/ast_printing.hpp>
#include <imp/syntax_types.hpp>
#include <noam/combinators.hpp>
#include <noam/intrinsics.hpp>
#include <noam/result_types.hpp>
#include <noam/type_traits.hpp>
#include <rva/variant.hpp>

namespace imp {
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
constexpr auto parse_cons_or_var = noam::either<arith_expr>(
    parse_constant,
    parse_variable);
}
