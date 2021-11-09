#include <fmt/core.h>
#include <imp/ast_printing.hpp>
#include <imp/comment.hpp>
#include <imp/parsers/command.hpp>
#include <imp/read_file.hpp>
#include <imp/translator.hpp>

int main(int argc, char** argv) {
    if (argc == 1) {
        fmt::print("Fatal Error: No input files.\n");
        fmt::print("Usage:\n\n\t{} <filename>", argv[0]);
        return 1;
    }

    std::string file = imp::read_file(argv[1]);
    auto program = imp::filter_comments(file);

    auto parse_result = imp::parse_program.parse(program);
    if (!parse_result.get_state().empty()) {
        fmt::print("Unable to finish parsing program.\n");
        fmt::print(
            "Error beginning here: \n\n{}",
            std::string_view(parse_result.get_state()));
        return 1;
    }
    if (parse_result) {
        auto program_ast = parse_result.get_value();

        imp::ir_compiler compiler {};
        compiler.print(program_ast);
    } else {
        fmt::print("failed to parse thing\n");
    }
    return 0;
}
