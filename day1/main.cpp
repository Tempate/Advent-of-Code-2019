#include <iostream>
#include <string>

#include "../input_parsing.cpp"

int f (const int x) {
    return (x / 3) - 2;
}

int main() {
    int part1 = 0;
    int part2 = 0;

    for (const auto &line : file_to_vector("input.txt")) {
        int m = f(std::stoi(line));

        // Part 1
        part1 += m;

        // Part 2
        while (m > 0) {
            part2 += m;
            m = f(m);
        }
    }

    std::cout << "Part 1: " << part1 << std::endl;
    std::cout << "Part 2: " << part2 << std::endl;

    return 0;
}

