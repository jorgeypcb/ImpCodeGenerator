#include <fmt/core.h>
#include <fstream>
#include <imp/instruction.hpp>
#include <iostream>
#include <string>

namespace imp {
void print_instruction(imp::instruction ins) {
    OpCategory category = ins.getCategory();
    switch (category) {
        case OpCategory::BinaryOp: break;

        case OpCategory::UnaryOp: break;
        case OpCategory::LoadConstantOp: break;
        case OpCategory::JumpOp: break;
        case OpCategory::LabelOp: break;
    }
}

void binaryOps(instruction i) {
    std::string_view name;
    switch (i.op) {
        case Op::Plus: name = "add"; break;
        case Op::Minus: name = "";

        case Op::Times: name = "mul";

        case Op::Greater: name = "add";

        case Op::GreaterEq:

        case Op::Equal:

        case Op::Or:

        case Op::And:

        default: break;
    }
    fmt::print("lw {} a0 \n", i.i1);
    fmt::print("lw {} a1 \n", i.i2);
    fmt::print("{} {} {} {}\n", name, i.i1, i.i2, i.output);
    fmt::print("sw {} a1 \n", i.output);
}
} // namespace imp