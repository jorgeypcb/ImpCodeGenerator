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
    Move,

    // LoadConstantOp: 1 constant, 1 output
    LoadConstant,

    // JumpOp 1 Input, 1 Label, No Output
    JumpIfZero,
    JumpIfNonzero,

    // LabelOp: 1 Label
    Label,
    // JumpOp: 1 Label (unconditional jump)
    Jump
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
    long long i1 {};  // Used for input1 and the condition of a jump
    long long i2 {};  // Used for input2 and the location of a jump
    int output {}; // Used as the destination of an instruction
    int expr_label {};

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
            case Op::Move: return OpCategory::UnaryOp;
            case Op::LoadConstant: return OpCategory::LoadConstantOp;
            case Op::JumpIfZero:
            case Op::JumpIfNonzero: return OpCategory::JumpOp;
            case Op::Label: 
            case Op::Jump: return OpCategory::LabelOp;
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
        IMP_ENUM_TO_STR_CASE(Op, Move);
        IMP_ENUM_TO_STR_CASE(Op, LoadConstant);
        IMP_ENUM_TO_STR_CASE(Op, JumpIfZero);
        IMP_ENUM_TO_STR_CASE(Op, JumpIfNonzero);
        IMP_ENUM_TO_STR_CASE(Op, Label);
        IMP_ENUM_TO_STR_CASE(Op, Jump);
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
        return format_with_labels(ins, ctx);
    }

    template <class Context>
    constexpr auto format_with_labels(imp::instruction const& ins, Context& ctx)
        -> decltype(ctx.out()) {
        using imp::Op;
        auto [op, i1, i2, out, expr_label] = ins;
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
                    "{} {} {} {} {}",
                    op_name,
                    i1,
                    i2,
                    out,
                    expr_label);
            case Op::Not:
                return fmt::format_to(
                    ctx.out(),
                    "{} {} null {} {}",
                    op_name,
                    i1,
                    out,
                    expr_label);
            case Op::Move:
                return fmt::format_to(
                    ctx.out(),
                    "{} {} null {} {}",
                    op_name,
                    i1,
                    out,
                    expr_label);
            case Op::LoadConstant:
                return fmt::format_to(
                    ctx.out(),
                    "{} {} null {} {}",
                    op_name,
                    i1,
                    out,
                    expr_label);
            case Op::JumpIfZero:
            case Op::JumpIfNonzero:
                return fmt::format_to(
                    ctx.out(),
                    "{} {} .LBB_{} null {}",
                    op_name,
                    i1,
                    i2,
                    expr_label);
            case Op::Label:
            case Op::Jump:
                return fmt::format_to(
                    ctx.out(),
                    "{} .LBB_{} null null {}",
                    op_name,
                    i1,
                    expr_label);

            // Used if given an unknown opcode
            default:
                return fmt::format_to(
                    ctx.out(),
                    "Op({}) {} {} {} {}",
                    (int)op,
                    i1,
                    i2,
                    out,
                    expr_label);
        }
    }
    // Format requested by group
    template <class Context>
    constexpr auto format_python(imp::instruction const& ins, Context& ctx)
        -> decltype(ctx.out()) {
        using imp::Op;
        auto [op, i1, i2, out, expr_label] = ins;
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
            case Op::Move:
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
            case Op::Jump:
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
        auto [op, i1, i2, out, expr_label] = ins;
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
            case Op::Move:
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
            case Op::Jump:
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
