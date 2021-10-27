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
        switch(op) {
            case Op::Plus:
            case Op::Minus:
            case Op::Times:
            case Op::Greater:
            case Op::GreaterEq:
            case Op::Equal:
            case Op::Or:
            case Op::And:
                return OpCategory::BinaryOp;
            case Op::Not:
                return OpCategory::UnaryOp;
            case Op::LoadConstant:
                return OpCategory::LoadConstantOp;
            case Op::JumpIfZero:
            case Op::JumpIfNonzero:
                return OpCategory::JumpOp;
            case Op::Label:
                return OpCategory::LabelOp;
        }
    }
};
} // namespace imp
