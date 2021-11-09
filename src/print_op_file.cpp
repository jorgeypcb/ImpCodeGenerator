#include <imp/instruction.hpp>

int main() {
    using imp::Op;
    for(int i = 0; i <= int(Op::Jump); i++) {
        auto ins = imp::instruction{Op{i}, 1, 2, 3};
        fmt::print("{}\n", ins);
    }
}
