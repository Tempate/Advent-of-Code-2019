#include <iostream>
#include <algorithm>
#include <vector>

#include "../input_parsing.cpp"
#include "../computer.cpp"

std::vector<std::vector<int>> gen_permutations(std::vector<int> p) {
    std::vector<std::vector<int>> permutations;

    if (p.size() == 1) {
        permutations.push_back(p);
        return permutations;
    }

    for (int i = 0; i < p.size(); i++) {
        std::vector<int> q = p;
        q.erase(q.begin() + i);
        
        const auto sub_permutations = gen_permutations(q);

        for (const auto &sub_permutation : sub_permutations) {
            std::vector<int> permutation = {p[i]};
            permutation.insert(permutation.end(), sub_permutation.begin(), sub_permutation.end());
            permutations.push_back(permutation);
        }
    }

    return permutations;
}

int main() {
    std::vector<int64_t> program = parse_program<int64_t>("input.txt");

    // Part 1
    const auto phases1 = gen_permutations({0,1,2,3,4});
    int part1 = std::numeric_limits<int>::min();

    for (const auto &phase : phases1) {
        int signal = 0;

        for (const int digit : phase) {
            IntcodeComputer<int64_t> machine(program);
            machine.run(INP, digit);
            machine.run(INP, signal);
            signal = machine.run(OUT, NON);
        }

        if (signal > part1)
            part1 = signal;
    }

    std::cout << "Part 1: " << part1 << std::endl;

    // Part 2
    const auto phases2 = gen_permutations({5,6,7,8,9});
    int part2 = std::numeric_limits<int>::min();

    for (const auto &phase : phases2) {
        std::array<IntcodeComputer<int64_t>, 5> machines;

        // Initialize machines
        for (int i = 0; i < 5; i++)
            machines[i] = IntcodeComputer<int64_t>(program);

        int signal = 0;

        // Set phases
        for (int i = 0; i < 5; i++)
            machines[i].run(INP, phase[i]);

        // Execute the program until the last machine halts
        while (machines[4].flag != HLT) {
            for (auto &machine : machines) {
                machine.run(INP, signal);
                signal = machine.run(OUT, NON);
            }
        }

        if (signal > part2)
            part2 = signal;
    }

    std::cout << "Part 2: " << part2 << std::endl;

    return 0;
}