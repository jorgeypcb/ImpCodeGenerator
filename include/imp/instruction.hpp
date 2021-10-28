#pragma once
#include <fmt/format.h>
#include <string_view>

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

enum class OpCategory { BinaryOp, UnaryOp, LoadConstantOp, JumpOp, LabelOp };

struct instruction {
    Op op {};  // Stores the operand
    int i1 {}; // Used for input1 and the condition of a jump
    int i2 {}; // Used for input2 and the label
    int output;

    OpCategory getCategory() const {
        switch (op) {
            case Op::Plus:
            case Op::Minus:
            case Op::Times:
            case Op::Greater:
            case Op::GreaterEq:
            case Op::Equal:
            case Op::Or:
            case Op::And: return OpCategory::BinaryOp;
            case Op::Not: return OpCategory::UnaryOp;
            case Op::LoadConstant: return OpCategory::LoadConstantOp;
            case Op::JumpIfZero:
            case Op::JumpIfNonzero: return OpCategory::JumpOp;
            case Op::Label: return OpCategory::LabelOp;
        }
    }
};
#define ENUM_TO_STR_CASE(EnumName, EnumValue)                                  \
    case EnumName::EnumValue: return #EnumValue;

constexpr static std::string_view to_string(Op o) {
    switch (o) {
        ENUM_TO_STR_CASE(Op, Plus);
        ENUM_TO_STR_CASE(Op, Minus);
        ENUM_TO_STR_CASE(Op, Times);
        ENUM_TO_STR_CASE(Op, Greater);
        ENUM_TO_STR_CASE(Op, GreaterEq);
        ENUM_TO_STR_CASE(Op, Equal);
        ENUM_TO_STR_CASE(Op, Or);
        ENUM_TO_STR_CASE(Op, And);
        ENUM_TO_STR_CASE(Op, Not);
        ENUM_TO_STR_CASE(Op, LoadConstant);
        ENUM_TO_STR_CASE(Op, JumpIfZero);
        ENUM_TO_STR_CASE(Op, JumpIfNonzero);
        ENUM_TO_STR_CASE(Op, Label);
    }
}
} // namespace imp


template <>
struct fmt::formatter<imp::instruction> {
    constexpr auto parse(format_parse_context& ctx) -> decltype(ctx.begin()) {
        return ctx.begin();
    }

    template <class Context>
    constexpr auto format(imp::instruction const& ins, Context& ctx)
        -> decltype(ctx.out()) {
        using imp::Op;
        auto [op, i1, i2, out] = ins;
        auto op_name = imp::to_string(op);
        switch (op) {
            case Op::Plus:
            case Op::Minus:
            case Op::Times:
            case Op::Greater:
            case Op::GreaterEq:
            case Op::Equal:
            case Op::Or:
            case Op::And:
                return fmt::format_to(
                    ctx.out(),
                    "{} {} {} {}",
                    op_name,
                    i1,
                    i2,
                    out);
            case Op::Not:
                return fmt::format_to(ctx.out(), "{} {} {}", op_name, i1, out);
            case Op::LoadConstant:
                return fmt::format_to(ctx.out(), "{} {} {}", op_name, i1, out);
            case Op::JumpIfZero:
            case Op::JumpIfNonzero:
                return fmt::format_to(
                    ctx.out(),
                    "{} {} .LBB_{}",
                    op_name,
                    i1,
                    out);
            case Op::Label:
                return fmt::format_to(ctx.out(), "{} .LBB_{}", op_name, i1);
        }
    }
};
