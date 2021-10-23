#include <fmt/core.h>
#include <fstream>
#include <imp/ast_printing.hpp>
#include <imp/syntax_types.hpp>
#include <iostream>

using namespace imp;

int main() {

    // command test_command = assignment<arith_expr> { variable{"x"},
    // constant{10} };
    command test_command = command {std::vector<command> {
        assignment<arith_expr> {variable {"x"}, constant {10}},
        assignment<arith_expr> {variable {"z"}, constant {20}},
        command {std::vector<command> {
            assignment<arith_expr> {variable {"x"}, constant {10}},
            assignment<arith_expr> {variable {"z"}, constant {20}}}}}};

    ast_to_dotfile("testdot.dot", test_command);
}
