#include <iostream>
#include <array>
#include <cassert>
#include <vector>
#include <algorithm>
#include <set>
#include <functional>

#include "../input_parsing.cpp"
#include "../computer.cpp"

const int grid_size = 50;

enum { WALL, BLANK, OXYGEN, START };
enum { NORT=1, SOUT, WEST, EAST };

struct Point {
    int x;
    int y;

    int flag = BLANK;
    bool visited = false;

    int dist = 0;
    std::vector<int> route;

    Point(): x(0), y(0) {};
    Point(const int x, const int y): x(x), y(y) {};
};

class Robot {
    private:
    IntcodeComputer<int64_t> brain;
    std::vector<Point> route;
    
    public:
    Point pos;
    std::array<std::array<Point, grid_size>, grid_size> grid;

    Robot(const std::vector<int64_t> &program) {
        pos = Point(grid_size / 2, grid_size / 2),
        brain = IntcodeComputer<int64_t>(program);

        for (int y = 0; y < grid_size; y++) {
            for (int x = 0; x < grid_size; x++) {
                grid[y][x] = Point(x, y);
            }
        }
    };

    public:
    int catch_flag(Point start, const int end) {
        static const int opdir[] = {SOUT, NORT, EAST, WEST};

        std::vector<Point> queue;

        start.visited = true;
        queue.push_back(start);

        while (queue.size() > 0) {
            pos = queue[0];
            queue.erase(queue.begin());

            // Move to new position
            for (const int dir : pos.route) {
                brain.run(INP, dir);
                assert(brain.run(OUT, NON) != WALL);
            }

            if (pos.flag == end)
                break;

            auto neighbors = get_neighbors(pos);

            for (int dir = NORT; dir <= EAST; dir++) {
                Point &neighbor = neighbors[dir-1].get();

                if (!neighbor.visited) {
                    brain.run(INP, dir);
                    neighbor.flag = brain.run(OUT, NON);
                    
                    if (neighbor.flag != WALL) {
                        neighbor.dist = pos.dist + 1;
                        neighbor.visited = true;

                        neighbor.route = pos.route;
                        neighbor.route.push_back(dir);
                        
                        queue.push_back(neighbor);

                        brain.run(INP, opdir[dir-1]);
                        assert(brain.run(OUT, NON) != WALL);
                    }
                }
            }

            // Return to initial position
            for (int i = pos.route.size() - 1; i >= 0; i--) {
                const int dir = pos.route[i];
                brain.run(INP, opdir[dir-1]);
                assert(brain.run(OUT, NON));
            }
        }

        return pos.dist;
    }

    void print() const {
        for (const auto &row : grid) {
            for (const auto sqr : row) {
                static const char objs[] = {'|', ' ', '+', '*'};

                std::cout << objs[sqr.flag];
            }

            std::cout << std::endl;
        }
    }

    private:
    std::vector<std::reference_wrapper<Point>> get_neighbors(const Point p) {
        std::vector<std::reference_wrapper<Point>> points;

        points.push_back(std::ref(grid[p.y-1][p.x]));
        points.push_back(std::ref(grid[p.y+1][p.x]));
        points.push_back(std::ref(grid[p.y][p.x-1]));
        points.push_back(std::ref(grid[p.y][p.x+1]));

        return points;
    }
};

int main() {
    const std::vector<int64_t> program = parse_program<int64_t>("input.txt");

    const Point start = Point(grid_size/2, grid_size/2);
    Robot robot(program);

    const int dist = robot.catch_flag(start, OXYGEN);

    std::cout << "Distance to oxygen: " << dist << std::endl;

    Point oxygen = robot.pos;
    oxygen.dist = 0;

    Robot robot2(program);

    const int max_dist = robot2.catch_flag(oxygen, -1);

    std::cout << "Time to fill with oxygen: " << max_dist << std::endl;

    robot2.grid[start.y][start.x].flag = START;
    robot2.print();

    return 0;
}