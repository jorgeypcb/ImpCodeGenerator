# Project1-SimpleCodeGeneration

## Building

Build via:

```bash
./build.sh
```

This will invoke CMake and download any dependcies that aren't found on the
system.

## Example

Output of running `build/example`, which prints the syntax tree:

```
x = 10;
y = 20;
z = x + y;
while x > y do
x = x + 1;
done
```

Corresponding syntax tree:

```cpp
command comm = command {
    std::vector<command> {
        assignment<arith_expr> {
            variable {"x"},
            constant {10}
        },
        assignment<arith_expr> {
            variable {"y"},
            constant {20}
        },
        assignment<arith_expr> {
            variable {"z"},
            arith_expr {
                binary_expr<arith_expr> {
                    binary_expr<arith_expr>::expr_data {
                        variable {"x"},
                        variable {"y"},
                        '+'
                    }
                }
            }
        },
        while_loop<bool_expr, command> {
            while_loop<bool_expr, command>::data {
                binary_expr<arith_expr> {
                    binary_expr<arith_expr>::expr_data {
                        variable {"x"},
                        variable {"y"},
                        '>'
                    }
                },
                assignment<arith_expr> {
                    variable {"x"},
                    arith_expr {
                        binary_expr<arith_expr> {
                            binary_expr<arith_expr>::expr_data {
                                variable {"x"},
                                constant {1},
                                '+'
                            }
                        }
                    }
                },
            }
        }
    }
};
```

## Summary of project

Translating programs in a very simple canonical imperative language, known in
the literature as WHILE, or sometimes IMP, into RISC-V assembly code. The
resulting assembly code will be linked with a main program, written in C and
compiled with gcc, and then run on either the SiFive board
(risc-machine.cs.unm.edu) or a RISC-V emulator.

- Task 1: Scan and parse a source language program to yield an abstract syntax
  tree (AST). You can use whatever tools are appropriate to your implementation
  language.
- Task 2: Display the AST. For the purposes of the project report, generate a
  PDF of the AST. (Hint: you can use an external graph-drawing tool, such as
  graphviz, to render an AST.)
- Task 3: Convert the AST into RISC-V assembly code, and then produce a
  standalone Fedora Linux RISC-V executable by linking with a custom main
  program in C, which you will also generate.
- Task 4: Extensively test your compiler. Write test programs to demonstrate
  complete coverage of language features.
