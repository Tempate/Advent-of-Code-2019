#include <iostream>
#include <cassert>
#include <array>
#include <set>

#include "../input_parsing.cpp"
#include "arcade.cpp"

int main() {
    std::vector<int> intcode = parse_program<int>("input.txt");
    
    ArcadeMachine m1(intcode);
    
    m1.load();
    m1.print();

    std::cout << "Number of blocks: " << m1.count(BLOCK) << "\n" << std::endl;

    intcode[0] = 2;
    ArcadeMachine m2(intcode);

    m2.load();
    m2.print();
    m2.play();
    m2.print();

    return 0;
}