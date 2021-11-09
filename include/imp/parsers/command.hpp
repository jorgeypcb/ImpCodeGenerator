#pragma once
#include <imp/parsers/basic_parsers.hpp>
#include <imp/parsers/arith_expr.hpp>
#include <imp/parsers/bool_expr.hpp>

namespace imp {
using command = rva::variant<
    assignment<arith_expr>,             // Assignment command
    skip_command,                       // Skip command
    if_command<bool_expr, rva::self_t>, // If statement
    while_loop<bool_expr, rva::self_t>, // while loop
    std::vector<rva::self_t>>;          // List of commands separated by ;

constexpr auto parse_one_command = [](auto parse_command) {
    return noam::parser {
        [parse_sub_cmd = noam::whitespace_enclose(parse_command)](
            noam::state_t st) -> noam::result<command> {
            constexpr auto parse_condition = noam::whitespace_enclose(
                parse_bool_expr);
            const auto empty_command = command {std::vector<command> {}};

            // Parse a command
            if (auto res = noam::literal<"skip">.read(st)) {
                return {st, skip_command {}};
            }
            if (noam::literal<"if">.read(st)) {
                noam::result<bool_expr> cond = parse_condition.read(st);
                if (!cond)
                    return {};
                if (!noam::literal<"then">.read(st))
                    return {};
                noam::result<command> when_true = parse_sub_cmd.read(st);
                if (!when_true)
                    return {};
                if (!noam::literal<"else">.read(st))
                    return {};
                noam::result<command> when_false = parse_sub_cmd.read(st);
                if (!when_false)
                    return {};
                if (!noam::literal<"fi">.read(st))
                    return {};

                return noam::result<command> {
                    st,
                    if_command<bool_expr, command>(
                        std::move(cond).get_value(),
                        std::move(when_true).get_value(),
                        std::move(when_false).get_value())};
            }
            if (noam::literal<"while">.read(st)) {
                noam::result<bool_expr> cond = parse_condition.read(st);
                if (!cond)
                    return {};
                if (!noam::literal<"do">.read(st))
                    return {};
                noam::result<command> when_true = parse_sub_cmd.read(st);
                if (!when_true)
                    return {};
                if (!noam::literal<"od">.read(st))
                    return {};
                return noam::result<command> {
                    st,
                    while_loop {
                        std::move(cond).get_value(),
                        std::move(when_true).get_value()}};
            }
            if (auto var = parse_variable.read(st)) {
                if (!noam::whitespace_enclose(noam::literal<":=">).read(st))
                    return {};
                auto value = parse_arith_expr.read(st);
                if (!value)
                    return {};
                return noam::result<command> {
                    st,
                    assignment<arith_expr> {
                        std::move(var).get_value(),
                        std::move(value).get_value()}};
            }
            return {};
        }};
};
constexpr auto parse_command = noam::recurse<command>([](auto self) {
    return noam::parser {
        [parse_one = parse_one_command(self)](
            noam::state_t st) -> noam::result<command> {
            auto first_command = parse_one.read(st);
            // If there's no first command, return an empty vector
            if (!first_command)
                return {st, command {std::vector<command> {}}};

            constexpr auto sep = noam::whitespace_enclose(noam::literal<';'>);

            std::vector<command> all;
            all.push_back(std::move(first_command.get_value()));

            auto next_command = noam::join(sep, parse_one);

            while (auto next = next_command.read(st)) {
                all.push_back(next.get_value());
            }

            // Read the separator if it was at the end with no command present
            sep.read(st);
            if (all.size() > 1) {
                return {st, command {all}};
            } else {
                return {st, std::move(all[0])};
            }
        }};
});
constexpr auto parse_program = noam::whitespace_enclose(parse_command);
} // namespace imp
