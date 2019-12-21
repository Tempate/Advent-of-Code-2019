enum {NORTH, EAST, SOUTH, WEST};

static const int grid_size = 200;

struct Point {
    int x;
    int y;

    Point() : x(0), y(0) {};
    Point(const int x, const int y) : x(x), y(y) {};

    bool operator==(const Point p) const {
        return x == p.x && y == p.y;
    }
};

class Robot {
    public:
    std::array<std::array<int, grid_size>, grid_size> grid = {};

    IntcodeComputer<int64_t> brain;
    Point pos;
    
    private:
    int dir;

    public:
    Robot(const std::vector<int64_t> program, const int x, const int y) {
        brain = IntcodeComputer(program);
        pos = Point{x, y};
        dir = NORTH;
    };

    void run() {
        brain.run(INP, grid[pos.y][pos.x]);
        
        paint(brain.run(OUT, NON));
        change_dir(brain.run(OUT, NON));
        move();
    }

    private:
    void paint(const int color) {
        assert(color == 0 || color == 1);
        grid[pos.y][pos.x] = color;
    }

    void change_dir(const int rot) {
        switch (rot) {
        case 0:
            // Turn left 90 degrees
            dir = (dir > 0) ? dir - 1 : WEST;
            break;
        case 1:
            // Turn right 90 degrees
            dir = (dir + 1) % 4;
            break;
        default:
            std::cout << "There's been an error" << std::endl;
            exit(1);
        }
    }

    void move() {
        switch (dir) {
        case NORTH:
            pos.y--;
            break;
        case EAST:
            pos.x++;
            break;
        case SOUTH:
            pos.y++;
            break;
        case WEST:
            pos.x--;
            break;
        }

        assert(pos.x >= 0 && pos.x < grid_size);
        assert(pos.y >= 0 && pos.y < grid_size);
    }

    public:
    void print() const {
        for (const auto &row : grid) {
            bool blank = true;

            for (const auto sqr : row) {
                switch (sqr) {
                case 0:
                    if (!blank)
                        std::cout << " ";
                    break;
                case 1:
                    blank = false;
                    std::cout << "*";
                    break;
                default:
                    break;
                }
            }

            if (!blank)
                std::cout << std::endl;
        }
    }
};