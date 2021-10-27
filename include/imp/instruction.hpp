#pragma once

namespace imp {
enum class Op {
    // Binrary op: 2 inputs, 1 Output
    Plus,
    Minus,
    Times,
    Greater,
    GreaterEq,
    Equal,
    Or,
    And,

    // Unary op: 1 Input, 1 Output
    Not,

    // LoadConstantOp: 1 constant, 1 output
    LoadConstant,

    // JumpOp 1 Input, 1 Label, No Output
    JumpIfZero,
    JumpIfNonzero,

    // LabelOp: 1 Label
    Label
};

enum class OpCategory {
    BinaryOp,
    UnaryOp,
    LoadConstantOp,
    JumpOp,
    LabelOp
};

struct instruction {
    Op op {};  // Stores the operand
    int i1 {}; // Used for input1 and the condition of a jump
    int i2 {}; // Used for input2 and the label
    int output;

    OpCategory getCategory() const {

    }
};
} // namespace imp
