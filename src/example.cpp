#include <imp/printing.hpp>

using namespace imp;
int main() {
    command comm = command {
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
                        binary_expr<arith_expr>::expr_data {
                            variable {"x"}, // <br>
                            variable {"y"}, // <br>
                            '+'             // <br>
                        }                   // <br>
                    }                       // <br>
                }                           // <br>
            }                               // <br>
        }                                   // <br>
    };

    print_expr(comm);
}
