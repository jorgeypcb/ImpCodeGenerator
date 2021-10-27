#pragma once

namespace imp {
enum class Op {
    // 2 inputs, 1 Output
    Plus,
    Minus,
    Times,
    Greater,
    GreaterEq,
    Equal,
    Or,
    And,

    // 1 Input, 1 Output
    Not,
    LoadConstant,

    // 1 Input, 1 Label, No Output
    JumpIfZero,
    JumpIfNonzero,
    // 1 Label
    Label
};

struct instruction {
    Op op {};  // Stores the operand
    int i1 {}; // Used for input1 and the condition of a jump
    int i2 {}; // Used for input2 and the label
    int output;
};
} // namespace imp