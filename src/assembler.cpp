#include <fmt/core.h>
#include <imp/ast_printing.hpp>
#include <imp/parsers/command.hpp>

int main() {
    std::string_view program = R"(
n := input;
steps := 0;
while n > 1 do
  rem := n;
  quot := 0;
  while rem > 1 do
    rem := rem - 2;
    quot := quot + 1
  od;
  if rem = 0 then
    n := quot
  else
    n := 3*n+1
  fi;
  steps := steps + 1
od;
output := steps
)";
    auto test = imp::parse_program.parse(program);
    if (test) {
        auto val = test.get_value();

        imp::print_graph(std::cout, val);
    } else {
        fmt::print("failed to parse thing\n");
    }
}
