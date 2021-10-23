#pragma once

#include <imp/syntax_types.hpp>
#include <fmt/core.h>
#include <iostream>
#include <fstream>
#include <string>

namespace imp{
using std::string;
using std::ostream;


ostream& operator<<(ostream& os, command const& expr);

void ast_to_dotfile(string fname, command const& ast) {
    std::ofstream dotfile;
    int new_node = 0;
    dotfile.open (fname);

    dotfile << ast;

    dotfile.close();
}

ostream& operator<<(ostream& os, assignment<arith_expr> const& a) {
    os << "asdf";
    return os;
}

ostream& operator<<(ostream& os, command const& expr) {
    std::cout << "calling visitor\n";
    auto visitor = [&](auto const& v) { 
        os << v; 
    };
    rva::visit(visitor, expr);
    return os;
}

} //namespace imp

