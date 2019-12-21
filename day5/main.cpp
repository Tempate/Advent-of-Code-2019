#include <iostream>
#include <array>
#include <cassert>

#include "../input_parsing.cpp"

std::vector<int> program = parse_program<int>("input.txt");

enum {
    SUM=1, MULT, INPUT, OUTPUT, JUMP_IF_TRUE, JUMP_IF_FALSE, LESS_THAN, EQUALS, HALT=99
};

struct Opcode {
    int instruction;
    std::array<int, 3> modes;

    Opcode(const int opc) {
        instruction = opc % 100;
        set_modes(opc / 100);
    }

    void set_modes(int m) {
        for (int i = 0; i < 3; i++) {
            modes[i] = m % 10;
            m /= 10;
        }
    }
};

int get_index(const int i, const int mode) {
    int index = 0;
    
    switch (mode) {
    case 0:
        index = program[i];
        break;
    case 1:
        index = i;
        break;
    default:
        std::cout << "Wrong mode: " << mode << std::endl;
    }

    assert(index >= 0 && index < program.size());

    return index;
}

void exec_program() {
    int i = 0;
    bool exit = false;

    while (!exit) {
        Opcode opc(program[i]);

        switch (opc.instruction) {
            case SUM:
                program[get_index(i+3, opc.modes[2])] = program[get_index(i+1, opc.modes[0])] + program[get_index(i+2, opc.modes[1])];
                i += 4;
                break;
            case MULT:
                program[get_index(i+3, opc.modes[2])] = program[get_index(i+1, opc.modes[0])] * program[get_index(i+2, opc.modes[1])];
                i += 4;
                break;
            case INPUT:
                std::cin >> program[get_index(i+1, opc.modes[0])];
                i += 2;
                break;
            case OUTPUT:
                std::cout << program[get_index(i+1, opc.modes[0])] << std::endl;
                i += 2;
                break;
            case JUMP_IF_TRUE:
                if (program[get_index(i+1, opc.modes[0])] != 0)
                    i = program[get_index(i+2, opc.modes[1])];
                else
                    i += 3;

                break;
            case JUMP_IF_FALSE:
                if (program[get_index(i+1, opc.modes[0])] == 0)
                    i = program[get_index(i+2, opc.modes[1])];
                else
                    i += 3;
                
                break;
            case LESS_THAN:
                program[get_index(i+3, opc.modes[2])] = 
                    program[get_index(i+1, opc.modes[0])] < 
                    program[get_index(i+2, opc.modes[1])] ? 1 : 0;

                i += 4;
                break;
            case EQUALS:
                program[get_index(i+3, opc.modes[2])] = 
                    program[get_index(i+1, opc.modes[0])] == 
                    program[get_index(i+2, opc.modes[1])] ? 1 : 0;

                i += 4;
                break;
            case HALT:
                exit = true;
                break;
            default:
                std::cout << "Wrong opcode: " << i << std::endl;
        }
    }
}

int main() {
    std::cout << "Part 1: (input 1)" << std::endl;
    std::cout << "Part 2: (input 5)" << std::endl;
    exec_program();
    
    return 0;
}