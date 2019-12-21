#include <iostream>
#include <cassert>
#include <array>
#include <vector>
#include <set>

#include "../input_parsing.cpp"
#include "../computer.cpp"
#include "robot.cpp"

int count_visited_points(Robot &robot) {
    std::set<std::array<int, 2>> visited_points;

    while (robot.brain.flag != HLT) {
        visited_points.insert({robot.pos.x, robot.pos.y});
        robot.run();
    }

    return visited_points.size();
}

int main() {
    const std::vector<int64_t> program = parse_program<int64_t>("input.txt");
        
    Robot robot(program, grid_size/2, grid_size/2);

    std::cout << "Part 1: " << count_visited_points(robot) << std::endl;

    std::cout << "Part 2: \n" << std::endl;
    Robot robot2(program, grid_size/2, grid_size/2);
    robot2.grid[grid_size/2][grid_size/2] = 1;

    while (robot2.brain.flag != HLT)
        robot2.run();

    robot2.print();

    return 0;
}