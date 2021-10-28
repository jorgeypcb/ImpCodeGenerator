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

// Defines the category of an operation (see above)
enum class OpCategory {
    BinaryOp,
    UnaryOp,
    LoadConstantOp,
    JumpOp,
    LabelOp,
    Unknown
};

struct instruction {
    Op op {};   // Stores the operand
    int i1 {};  // Used for input1 and the condition of a jump
    int i2 {};  // Used for input2 and the location of a jump
    int output; // Used as the destination of an instruction

    // Gets the category of an operation based on the operation
    constexpr OpCategory getCategory() const noexcept {
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

        // Used if given an unknown opcode
        return OpCategory::Unknown;
    }
};
#define IMP_ENUM_TO_STR_CASE(EnumName, EnumValue)                                  \
    case EnumName::EnumValue: return #EnumValue;

constexpr static std::string_view to_string(Op o) {
    switch (o) {
        IMP_ENUM_TO_STR_CASE(Op, Plus);
        IMP_ENUM_TO_STR_CASE(Op, Minus);
        IMP_ENUM_TO_STR_CASE(Op, Times);
        IMP_ENUM_TO_STR_CASE(Op, Greater);
        IMP_ENUM_TO_STR_CASE(Op, GreaterEq);
        IMP_ENUM_TO_STR_CASE(Op, Equal);
        IMP_ENUM_TO_STR_CASE(Op, Or);
        IMP_ENUM_TO_STR_CASE(Op, And);
        IMP_ENUM_TO_STR_CASE(Op, Not);
        IMP_ENUM_TO_STR_CASE(Op, LoadConstant);
        IMP_ENUM_TO_STR_CASE(Op, JumpIfZero);
        IMP_ENUM_TO_STR_CASE(Op, JumpIfNonzero);
        IMP_ENUM_TO_STR_CASE(Op, Label);
    }

    // Used if given an unknown opcode
    return "<unknown opcode>";
}
} // namespace imp


template <>
struct fmt::formatter<imp::instruction> {
    constexpr auto parse(format_parse_context& ctx) -> decltype(ctx.begin()) {
        // Doesn't parse formatting context for now, just use empty {}
        return ctx.begin();
    }

    template <class Context>
    constexpr auto format(imp::instruction const& ins, Context& ctx) {
        return format_python(ins, ctx);
    }

    // Format requested by group
    template <class Context>
    constexpr auto format_python(imp::instruction const& ins, Context& ctx)
        -> decltype(ctx.out()) {
        using imp::Op;
        auto [op, i1, i2, out] = ins;
        auto op_name = imp::to_string(op);

        // Operations in the same category get formatted in the same way
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
                return fmt::format_to(ctx.out(), "{} {} null {}", op_name, i1, out);
            case Op::LoadConstant:
                return fmt::format_to(ctx.out(), "{} {} null {}", op_name, i1, out);
            case Op::JumpIfZero:
            case Op::JumpIfNonzero:
                return fmt::format_to(
                    ctx.out(),
                    "{} {} .LBB_{} null",
                    op_name,
                    i1,
                    i2);
            case Op::Label:
                return fmt::format_to(ctx.out(), "{} .LBB_{} null null", op_name, i1);

            // Used if given an unknown opcode
            default:
                return fmt::format_to(
                    ctx.out(),
                    "Op({}) {} {} {}",
                    (int)op,
                    i1,
                    i2,
                    out);
        }
    }

    template <class Context>
    constexpr auto format_alt(imp::instruction const& ins, Context& ctx)
        -> decltype(ctx.out()) {
        using imp::Op;
        auto [op, i1, i2, out] = ins;
        auto op_name = imp::to_string(op);

        // Operations in the same category get formatted in the same way
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
                    i2);
            case Op::Label:
                return fmt::format_to(ctx.out(), "{} .LBB_{}", op_name, i1);

            // Used if given an unknown opcode
            default:
                return fmt::format_to(
                    ctx.out(),
                    "Op({}) {} {} {}",
                    (int)op,
                    i1,
                    i2,
                    out);
        }
    }
};
