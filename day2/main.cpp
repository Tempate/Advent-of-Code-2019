#include <iostream>
#include <array>

#include "../input_parsing.cpp"

enum {SUM=1, MULT, HALT=99};

int computer(std::vector<int> program, const int a, const int b) {    
    program[1] = a;
    program[2] = b;

    int i = 0;
    bool halt = false;

    while (!halt) {
        const int i1 = program[i+1];
        const int i2 = program[i+2];
        const int i3 = program[i+3];
        
        switch (program[i]) {
            case SUM:
                program[i3] = program[i1] + program[i2];
                break;
            case MULT:
                program[i3] = program[i1] * program[i2];
                break;
            case HALT:
                halt = true;
                break;
            default:
                std::cout << "Wrong opcode: " << i << std::endl;
                exit(1);
        }

        i += 4;
    }

    return program[0];
}

int main() {
    const std::vector<int> program = parse_program<int>("input.txt");

    std::cout << "Part 1: " << computer(program, 12, 3) << std::endl;

    int part2 = 0;

    for (int a = 0; a < 100 && !part2; a++) {
        for (int b = 0; b < 100 && !part2; b++) {
            if (computer(program, a, b) == 19690720)
                part2 = a * 100 + b;
        }
    }

    std::cout << "Part 2: " << part2 << std::endl;

    return 0;
}