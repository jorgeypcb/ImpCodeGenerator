#include <fmt/core.h>
#include <fstream>
#include <imp/ast_printing.hpp>
#include <imp/syntax_types.hpp>
#include <iostream>

using namespace imp;

int main(int argc, char const* argv[]) {

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
                        variable {"x"}, // <br>
                        variable {"y"}, // <br>
                        '+'             // <br>
                    }                   // <br>
                }                       // <br>
            },                          // <br>
            while_loop<bool_expr, command> {
                binary_expr<arith_expr> {
                    variable {"x"}, // <br>
                    variable {"y"}, // <br>
                    '>'             // <br>
                },                  // <br>
                assignment<arith_expr> {
                    variable {"x"}, // <br>
                    arith_expr {
                        binary_expr<arith_expr> {
                            variable {"x"}, // <br>
                            constant {1},   // <br>
                            '+'             // <br>
                        }                   // <br>
                    }                       // <br>
                }                           // <br>
            }                               // <br>
        }                                   // <br>
    };


    // If no file is specified, print to standard output
    if (argc == 1) {
        print_graph(std::cout, test_command);
    } else {
        ast_to_dotfile(
            argv[1], // The filename is stored in argument 1
            test_command);
    }
}
