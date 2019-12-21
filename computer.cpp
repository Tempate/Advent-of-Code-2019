#include <iostream>
#include <array>
#include <vector>
#include <cassert>

enum { NON, INP, OUT, HLT };

struct Opcode {
    int inst;
    std::array<int, 3> modes;

    Opcode(const int opc) {
        inst = opc % 100;
        set_modes(opc / 100);
    }

    void set_modes(int m) {
        for (int i = 0; i < 3; i++) {
            modes[i] = m % 10;
            m /= 10;
        }
    }
};

template <typename T>
class IntcodeComputer {
    private:
    int base = 0;   // Keeps track of the relative base
    int i = 0;      // Current memory address

    public:
    std::array<T, 10000> program = {0};
    int flag = NON;

    IntcodeComputer() {};

    IntcodeComputer(const std::vector<T> intcode) { 
        std::copy(intcode.begin(), intcode.end(), program.begin());
    };
    
    T run(const int flag_, const T input) {
        T output;
        
        while (flag == NON)
            exec_inst(Opcode(program[i]), input);

        if (flag == flag_)
            output = exec_inst(Opcode(program[i]), input);

        return output;
    }

    private:
    T exec_inst(Opcode opc, const T input) {
        enum {
            SUM=1, MULT, 
            INPUT, OUTPUT, 
            JUMP_IF_TRUE, JUMP_IF_FALSE, 
            LESS_THAN, EQUALS, 
            REL_BASE, HALT=99
        };

        T output;

        assert(flag != HLT);

        switch (opc.inst) {
            case SUM:
                program[get_index(i+3, opc.modes[2])] = program[get_index(i+1, opc.modes[0])] + program[get_index(i+2, opc.modes[1])];
                i += 4;
                break;
            case MULT:
                program[get_index(i+3, opc.modes[2])] = program[get_index(i+1, opc.modes[0])] * program[get_index(i+2, opc.modes[1])];
                i += 4;
                break;
            case INPUT:
                if (flag == INP) {
                    program[get_index(i+1, opc.modes[0])] = input;
                    flag = NON;
                
                    i += 2;
                } else {
                    flag = INP;
                }

                break;
            case OUTPUT:
                if (flag == OUT) {
                    output = program[get_index(i+1, opc.modes[0])];
                    flag = NON;

                    i += 2;
                } else {
                    flag = OUT;
                }
                
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
            case REL_BASE:
                base += program[get_index(i+1, opc.modes[0])];

                i += 2;
                break;
            case HALT:
                flag = HLT;
                break;
            default:
                std::cout << "Wrong opcode: " << i << std::endl;
                exit(1);
        }

        return output;
    }

    private: T get_index(const int i, const int mode) {
        T index = 0;
        
        switch (mode) {
        case 0:
            index = program[i];
            break;
        case 1:
            index = i;
            break;
        case 2:
            index = base + program[i];
            break;
        default:
            std::cout << "Wrong mode: " << mode << std::endl;
            exit(1);
        }

        assert(index >= 0 && index < program.size());

        return index;
    }
};
