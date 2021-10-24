# Imp Code Generator

A simple compiler for the IMP programming language

## Building

Build via:

```bash
./build.sh
```

This will invoke CMake and download any dependcies that aren't found on the
system.

## Example

Output of running `build/ast_printing_test`, which prints the syntax tree when
piped to graphviz:

![](docs/ast-example-output.png)

Corresponding syntax tree:

![](docs/ast-example.png)

## Summary of project

_Taken from design document_

> Translating programs in a very simple canonical imperative language, known in
> the literature as WHILE, or sometimes IMP, into RISC-V assembly code. The
> resulting assembly code will be linked with a main program, written in C and
> compiled with gcc, and then run on either the SiFive board
> (risc-machine.cs.unm.edu) or a RISC-V emulator.
>
> - Task 1: Scan and parse a source language program to yield an abstract syntax
>   tree (AST). You can use whatever tools are appropriate to your
>   implementation language.
> - Task 2: Display the AST. For the purposes of the project report, generate a
>   PDF of the AST. (Hint: you can use an external graph-drawing tool, such as
>   graphviz, to render an AST.)
> - Task 3: Convert the AST into RISC-V assembly code, and then produce a
>   standalone Fedora Linux RISC-V executable by linking with a custom main
>   program in C, which you will also generate.
> - Task 4: Extensively test your compiler. Write test programs to demonstrate
>   complete coverage of language features.
