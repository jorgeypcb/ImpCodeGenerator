#pragma once

#include <imp/syntax_types.hpp>
#include <fmt/core.h>
#include <iostream>
#include <fstream>
#include <string>

namespace imp{
using std::string;
using std::ostream;

void print_command(command const& cmd, std::ofstream out);

void ast_to_file(string fname, command const& ast) {
    std::ofstream dotfile;
    int new_node = 0;
    dotfile.open (fname + ".dot");

    //// PASS OPEN DOTFILE TO OVERLOADED PRINT_COMMAND WITH NESTED CALLS TO PRINT THE REST OF THE AST
    dotfile << ast;

    dotfile.close();
}

ostream& operator<<(ostream& os, command const& ast) {return os; };

ostream& operator<<(ostream& os, assignment<arith_expr> const& a) {
    os << a.dest << "->" << a.value;
}

void print_command(assignment<arith_expr> const& a, std::ofstream out) {
    out << a.dest << "->" << a.value; ///NEED TO FORMAT THIS TO GRAPHVIZ
}

} //namespace imp

