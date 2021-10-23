#include <fmt/core.h>
#include <fstream>
#include <imp/ast_printing.hpp>
#include <imp/syntax_types.hpp>
#include <iostream>

using namespace imp;

int main() {

    // command test_command = assignment<arith_expr> { variable{"x"},
    // constant{10} };

    // command test_command = command {std::vector<command> {
    //     assignment<arith_expr> {variable {"x"}, constant {10}},
    //     assignment<arith_expr> {variable {"z"}, constant {20}},
    //     command {std::vector<command> {
    //         assignment<arith_expr> {variable {"x"}, constant {10}},
    //         assignment<arith_expr> {variable {"z"}, constant {20}}}}}};

    command test_command = command {
        std::vector<command> {
            assignment<arith_expr> {
                variable {"x"}, // <br>
                constant {10}   // <br>
            },                  // <br>
            assignment<arith_expr> {
                variable {"y"}, // <br>
                constant {20}   // <br>
            },                  // <br>
            assignment<arith_expr> {
                variable {"z"}, // <br>
                arith_expr {
                    binary_expr<arith_expr> {
                        binary_expr<arith_expr>::data {
                            variable {"x"}, // <br>
                            variable {"y"}, // <br>
                            '+'             // <br>
                        }                   // <br>
                    }                       // <br>
                }                           // <br>
            },                              // <br>
            while_loop<bool_expr, command> {
                while_loop<bool_expr, command>::data {
                    binary_expr<arith_expr> {
                        binary_expr<arith_expr>::data {
                            variable {"x"}, // <br>
                            variable {"y"}, // <br>
                            '>'             // <br>
                        }                   // <br>
                    },                      // <br>
                    assignment<arith_expr> {
                        variable {"x"}, // <br>
                        arith_expr {
                            binary_expr<arith_expr> {
                                binary_expr<arith_expr>::data {
                                    variable {"x"}, // <br>
                                    constant {1},   // <br>
                                    '+'             // <br>
                                }                   // <br>
                            }                       // <br>
                        }                           // <br>
                    }                               // <br>
                }                                   // <br>
            }                                       // <br>
        }                                           // <br>
    };


    ast_to_dotfile("testdot.dot", test_command);
}
