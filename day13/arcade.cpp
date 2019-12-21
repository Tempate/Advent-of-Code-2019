#include <iostream>
#include <vector>
#include <array>

#include "../computer.cpp"

enum {BLANK, WALL, BLOCK, PADDLE, BALL};

static const int grid_size = 100;

struct Point {
    int x;
    int y;

    Point() : x(0), y(0) {};
    Point(const int x, const int y) : x(x), y(y) {};

    bool operator==(const Point p) const {
        return x == p.x && y == p.y;
    }
};

class ArcadeMachine {
    private:
    Point ball;
    Point paddle;
    int score = 0;

    public:
    IntcodeComputer<int> brain;
    std::array<std::array<int, grid_size>, grid_size> grid = {};

    ArcadeMachine(const std::vector<int> intcode):
        brain(IntcodeComputer(intcode)) {};

    public:
    int count(const int object) {
        int n = 0;

        for (const auto &row : grid) {
            for (const int sqr : row) {
                if (sqr == object)
                    n++;
            }
        }

        return n;
    }

    void load() {
        while (brain.flag != INP && brain.flag != HLT) {
            const int x = brain.run(OUT, NON);
            const int y = brain.run(OUT, NON);
            const int block = brain.run(OUT, NON);

            if (x == -1 && y == 0)
                score = block;
            else
                grid[y][x] = block;
        }
    }

    void play() {
        while (brain.flag != HLT) {
            ball = find(4);
            paddle = find(3);

            int dir = 0;

            if (ball.x > paddle.x)
                dir = 1;
            else if (ball.x < paddle.x)
                dir = -1;

            brain.run(INP, dir);

            load();
        }
    }

    void print() const {
        static const char icons[5] = {' ', '|', '*', '_', 'o'};

        for (const auto &row : grid) {
            bool blank = true;

            for (const int sqr : row) {
                if (blank)
                    blank = sqr == 0;

                if (!blank)
                    std::cout << icons[sqr];
            }

            if (!blank)
                std::cout << std::endl;
        }

        std::cout << "\nScore: " << score << std::endl;
    }

    private:
    Point find(const int object) const {
        Point obj{};
        bool found = false;

        for (int y = 0; y < grid.size() && !found; y++) {
            for (int x = 0; x < grid[y].size() && !found; x++) {
                if (grid[y][x] == object) {
                    obj.x = x;
                    obj.y = y;
                }
            }
        }

        return obj;
    }
};