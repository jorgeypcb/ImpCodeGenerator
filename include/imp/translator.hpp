#pragma once
#include <fmt/core.h>
#include <imp/instruction.hpp>
#include <imp/syntax_types.hpp>
#include <imp/util/overload_set.hpp>
#include <set>
#include <unordered_map>

namespace imp {
using arith_expr = rva::variant<
    constant,                  // Integral constant
    variable,                  // Variable
    binary_expr<rva::self_t>>; // Binary operation on to arithmatic expressions

using bool_expr = rva::variant<
    bool_const,               // Boolean constant
    unary_expr<rva::self_t>,  // Unary operation on boolean expression
    binary_expr<rva::self_t>, // Binary operation on boolean expression
    binary_expr<arith_expr>>; // Comparison on arithmetic expressions

using command = rva::variant<
    assignment<arith_expr>,             // Assignment command
    skip_command,                       // Skip command
    if_command<bool_expr, rva::self_t>, // If statement
    while_loop<bool_expr, rva::self_t>, // while loop
    std::vector<rva::self_t>>;          // List of commands separated by ;

// Step 1: Get a list of all the variables in alphabetical order
// Step 2: assign addresses to variables and temporaries

// Step 1 - function to get all the variables
inline std::set<std::string_view> get_variables(command const& program) {
    std::set<std::string_view> variables;
    traverse(
        program,
        overload_set {
            [](auto const&) {},
            [&](variable const& var) { variables.insert(var.get_name()); }});
    return variables;
}

// Step 2 - class to assign addresses to everything
// Assigns addresses to variables, temporaries, and constants
struct address_assigner {
    std::set<std::string_view> variables;
    std::unordered_map<std::string_view, address_t> var_addresses;
    void assign_address(variable& var, address_t) {
        var.address = var_addresses[var.get_name()];
    }
    void assign_address(constant& c, address_t addr) { c.address = addr; }
    void assign_address(bool_const& c, address_t addr) { c.address = addr; }

    template <class Expr>
    void assign_address(unary_expr<Expr>& expr, address_t addr) {
        expr.address = addr;
        assign_address(expr.get_input(), addr + 1);
    }
    template <class... T>
    void assign_address(rva::variant<T...>& thing, address_t addr) {
        rva::visit(
            [addr, this](auto& thing) { assign_address(thing, addr); },
            thing);
    }
    template <class Expr>
    void assign_address(binary_expr<Expr>& expr, address_t addr) {
        expr.address = addr;
        assign_address(expr.get_left(), addr + 1);
        assign_address(expr.get_right(), addr + 2);
    }

    void assign_address(assignment<arith_expr>& ass, address_t addr) {
        assign_address(ass.dest, addr);
        assign_address(ass.value, addr);
    }
    void assign_address(skip_command&, address_t) {}
    void assign_address(if_command<bool_expr, command>& if_, address_t addr) {
        assign_address(if_.get_condition(), addr);
        assign_address(if_.when_true(), addr);
        assign_address(if_.when_false(), addr);
    }
    void assign_address(
        while_loop<bool_expr, command>& while_,
        address_t addr) {
        assign_address(while_.get_condition(), addr);
        assign_address(while_.get_body(), addr);
    }
    void assign_address(std::vector<command>& commands, address_t addr) {
        for (command& cmd : commands) {
            assign_address(cmd, addr);
        }
    }
    void assign_address(command& program) {
        variables = get_variables(program);
        var_addresses.reserve(variables.size());

        address_t addr = 0;
        for (std::string_view var_name : variables) {
            var_addresses[var_name] = addr;
            addr++;
        }
        assign_address(program, addr);
    }

    void print_variables(FILE* file) {
        address_t addr = 0;
        for (std::string_view var_name : variables) {
            fmt::print(file, "{} {}\n", addr, var_name);
            addr++;
        }
    }
};

struct ir_compiler {
    // The position to jump to when skip occurs. Set to -1 initially, because
    // it can only be used inside a loop body
    int skip_position = -1;
    // The value of the next lael
    int next_label = 0;
    int expr_label = 1;
    // Gets an unused unique label number
    int get_next_label() { return next_label++; }
    std::vector<instruction> ins;

    // No-op - does nothing
    void compile(variable const& v) {}
    void compile(constant const& c) {
        ins.push_back(
            instruction {Op::LoadConstant, c.value, 0, c.address, expr_label});
    }
    void compile(bool_const const& c) {
        ins.push_back(
            instruction {Op::LoadConstant, c.value, 0, c.address, expr_label});
    }
    void compile(binary_expr<arith_expr> const& b) {
        compile(b.get_left());
        compile(b.get_right());
        instruction i;
        switch (b.get_op()) {
            case '+':
                i = {
                    Op::Plus,
                    get_address(b.get_left()),
                    get_address(b.get_right()),
                    b.address,
                    expr_label};
                break;
            case '-':
                i = {
                    Op::Minus,
                    get_address(b.get_left()),
                    get_address(b.get_right()),
                    b.address,
                    expr_label};
                break;
            case '*':
                i = {
                    Op::Times,
                    get_address(b.get_left()),
                    get_address(b.get_right()),
                    b.address,
                    expr_label};
                break;
            case '=':
                i = {
                    Op::Equal,
                    get_address(b.get_left()),
                    get_address(b.get_right()),
                    b.address,
                    expr_label};
                break;
            case '<':
                i = {
                    Op::Greater,
                    // Flip operands: right first
                    get_address(b.get_right()),
                    get_address(b.get_left()),
                    b.address,
                    expr_label};
                break;
            case '>':
                i = {
                    Op::Greater,
                    get_address(b.get_left()),
                    get_address(b.get_right()),
                    b.address,
                    expr_label};
                break;
            case 'L':
                i = {
                    Op::GreaterEq,
                    // flip operands: right first
                    get_address(b.get_right()),
                    get_address(b.get_left()),
                    b.address,
                    expr_label};
                break;
            case 'G':
                i = {
                    Op::GreaterEq,
                    get_address(b.get_left()),
                    get_address(b.get_right()),
                    b.address,
                    expr_label};
        }
        ins.push_back(i);
    }
    void compile(unary_expr<bool_expr> const& b) {
        compile(b.get_input());
        // The only one right now is not, so I didn't bother writing a switch
        ins.push_back(instruction {
            Op::Not,
            get_address(b.get_input()),
            0,
            b.address,
            expr_label});
    }
    void compile(binary_expr<bool_expr> const& b) {
        compile(b.get_left());
        compile(b.get_right());
        Op op;
        switch (b.get_op()) {
            case '&': op = Op::And; break;
            case '|': op = Op::Or; break;
        }
        ins.push_back(instruction {
            op,
            get_address(b.get_left()),
            get_address(b.get_right()),
            b.address,
            expr_label});
    }
    void compile(assignment<arith_expr> const& ass) {
        compile(ass.value);
        // Emit an instruction to move the output of the expression into the
        // destination
        ins.push_back(instruction {
            Op::Move,
            get_address(ass.value),
            0,
            ass.dest.address,
            expr_label});
        expr_label++; // Get a new label after the command
    }
    void compile(skip_command const& cmd) {
        // Jump to the location set for a skip
        // This should be the condition of a loop, because it skips everything
        // else in the body
        ins.push_back(instruction {Op::Jump, skip_position, 0, 0, expr_label});
        expr_label++; // Get a new label after the command
    }
    void compile(while_loop<bool_expr, command> const& loop) {
        // Add the label for the start of the condition
        int loop_condition = expr_label;

        // Replace the skip position with the current loop condition. Save it to
        // 'previous_skip_position'. It'll be restored at the end of the
        // function
        int previous_skip_position = std::exchange(
            skip_position,
            loop_condition);
        ins.push_back(
            instruction {Op::Label, loop_condition, 0, 0, expr_label});

        compile(loop.get_condition());
        // Jump to the end of the loop if the condition is false

        size_t loop_end_jump = ins.size();
        ins.push_back(instruction {
            Op::JumpIfZero,
            get_address(loop.get_condition()),
            0, // instruction::i2: This location will be filled in when the
               // expression label for the end of the loop is known
            0,
            expr_label});

        expr_label++; // Get a new label after the condition

        // Compile the ody of the loop
        // First: Add a label indicating the start of the body
        ins.push_back(instruction {Op::Label, expr_label, 0, 0, expr_label});
        // Then compile the body of the loop itself
        compile(loop.get_body());

        // Jump back to the start of the loop. This should be part of the
        // previous expression, hence, expr_label - 1
        ins.push_back(
            instruction {Op::Jump, loop_condition, 0, 0, expr_label - 1});

        // Add a label for the instruction after the end of the loop
        ins.push_back(instruction {Op::Label, expr_label, 0, 0, expr_label});
        // Update the loop end jump instruction to point to this label
        ins[loop_end_jump].i2 = expr_label;

        // Restore the skip position
        skip_position = previous_skip_position;
    }
    void compile(if_command<bool_expr, command> const& if_) {
        address_t cond_addr = get_address(if_.get_condition());

        compile(if_.get_condition());
        // Jump to the else block if the condition was false
        size_t else_label_id = ins.size();
        ins.push_back(
            instruction {Op::JumpIfZero, cond_addr, 0, 0, expr_label});
        expr_label++; // Get a new label after the condition

        compile(if_.when_true());
        // Jump to the end of the if statement after finishing the 'then' block
        size_t end_of_if_label_id = ins.size();
        // The else block and it's corresponding jump should be part of the
        // previous expression
        ins.push_back(instruction {Op::Jump, 0, 0, 0, expr_label - 1});
        // Add the label for the start of the else block, then compile the else
        // block
        ins.push_back(instruction {Op::Label, expr_label, 0, 0, expr_label});
        // Update the label id now that the else expression is known
        ins[else_label_id].i2 = expr_label;
        compile(if_.when_false());

        // Add a label for the instruction after the end of the if statement
        ins.push_back(instruction {Op::Label, expr_label, 0, 0, expr_label});
        // Update the label id for the jump block at the end of the if
        ins[end_of_if_label_id].i1 = expr_label;
    }
    void compile(std::vector<command> const& commands) {
        for (command const& c : commands) {
            compile(c);
        }
    }
    template <class... T>
    void compile(rva::variant<T...> const& v) {
        rva::visit([this](auto const& expr) { compile(expr); }, v);
    }
    // Step 3: Translate to Intermediate Representation
    void print(
        command cmd,
        FILE* instruction_file = stdout,
        FILE* variable_file = stderr) {
        address_assigner scope;
        // Give everything an address
        scope.assign_address(cmd);

        // Print all the variables to the variable file
        scope.print_variables(variable_file);

        // Compile the command into the instruction vector
        compile(cmd);
        for (instruction const& i : ins) {
            fmt::print(instruction_file, "{}\n", i);
        }
    }
};
} // namespace imp
