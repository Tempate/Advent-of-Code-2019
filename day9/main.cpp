#include <iostream>

#include "../input_parsing.cpp"
#include "../computer.cpp"

int main() {
    const std::vector<int64_t> program = parse_program<int64_t>("input.txt");

    // Part 1
    IntcodeComputer<int64_t> m1(program);
    m1.run(INP, 1);

    std::cout << "Part 1: ";
    std::cout << m1.run(OUT, NON) << std::endl;

    // Part 2
    IntcodeComputer<int64_t> m2(program);
    m2.run(INP, 2);

    std::cout << "Part 2: ";
    std::cout << m2.run(OUT, NON) << std::endl;

    return 0;
}