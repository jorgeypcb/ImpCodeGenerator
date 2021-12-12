# Imp Code Generator

A simple compiler for the IMP programming language

## Team

- Alecto Perez
- Jorge Loredo
- David Arredondo

## Languages

- C++
- Python

## How to build the project (including dependencies):

1. Make sure gcc-11 is installed
   https://www.linuxfromscratch.org/blfs/view/svn/general/gcc.html
2. Make sure python 3.9 is installed
3. Make sure CMake version 3.21 or above is installed (you need a version of
   CMake that supports CMake Presets)

Run:

```bash
cmake --preset=build && cmake --build build
```

This will attempt to use system version of libraries. If the build fails, you
can alternatively have CMake download and build with the newest version of these
libraries by passing `-DALWAYS_FETCH=ON`:

```bash
cmake -DALWAYS_FETCH=ON --preset=build && cmake --build build
```

Once the assembler is built, you can build all the programs in the `programs/`
directory by first compiling them to the intermediate representation, then
running the riscv compiler to convert them to riscv:

```bash
for file in programs/*.imp; do
    build/assembler $file     \
        1> ${file%.imp}.ins   \
        2> ${file%.imp}.vars
    riscv/print_risc.py ${file} \
        --constant_folding      \
        --dead_code_elim        \
        --register_allocation > ${file%.imp}.asm
done
```

This will produce a RISC-V assembly file for every program in the programs
folder. Optimizations can be enabled or disabled by adding or removing the above
flags. To see all flags, run `riscv/print_risc.py`.
