#include <iostream>
#include <vector>
#include <array>
#include <string>
#include <cassert>
#include <limits>

#include "../input_parsing.cpp"

enum { X_AXIS, Y_AXIS };

struct Point {
    int x;
    int y;
    int steps;

    Point() : x(0), y(0), steps(0) {}
    Point(const int x, const int y, const int steps) : x(x), y(y), steps(steps) {}
};

void moveInDir(std::vector<Point> &points, Point &point, const int steps, const int axis, const int dir) {
    for (int i = 0; i < steps; i++) {
        switch (axis) {
        case X_AXIS:
            point.x += dir;
            break;
        case Y_AXIS:
            point.y += dir;
            break;
        }

        points.push_back(point);
    }
}

std::vector<Point> mapRouteToPoints(std::vector<std::string> route) {
    std::vector<Point> points;
    Point point;

    for (const std::string dir : route) {
        const int steps = std::stoi(dir.substr(1));

        switch (dir[0]) {
        case 'R':
            moveInDir(points, point, steps, X_AXIS, 1);
            break;
        case 'L':
            moveInDir(points, point, steps, X_AXIS, -1);
            break;
        case 'U':
            moveInDir(points, point, steps, Y_AXIS, 1);
            break;
        case 'D':
            moveInDir(points, point, steps, Y_AXIS, -1);
            break;
        }
    }

    return points;
}

std::vector<Point> find_collisions(const std::vector<Point> points1, const std::vector<Point> points2) {
    std::vector<Point> collisions;
    
    for (int i = 0; i < points1.size(); i++) {
        int j = 0;

        while (j < points2.size()) {
            const auto point1 = points1[i];
            const auto point2 = points2[j];

            const int dist = abs(point1.x - point2.x) + abs(point1.y - point2.y); 

            if (dist == 0) {
                collisions.emplace_back(point1.x, point1.y, i + j + 2);
                j++;
            } else {
                j += dist;
            }
        }
    }

    return collisions;
}

int main() {
    const std::vector<std::string> input = file_to_vector("input.txt");
    const std::vector<std::string> route1 = split_by_char(input[0], ',');
    const std::vector<std::string> route2 = split_by_char(input[1], ',');

    const auto points1 = mapRouteToPoints(route1);
    const auto points2 = mapRouteToPoints(route2);

    std::vector<Point> collisions = find_collisions(points1, points2);

    Point closest_collision(0, 0, std::numeric_limits<int>::max());
    int fewest_steps = closest_collision.steps;

    for (const auto collision : collisions) {
        // Part 1
        const int dist = abs(collision.x) + abs(collision.y);

        if (dist < closest_collision.steps) {
            closest_collision = collision;
            closest_collision.steps = dist;
        }
        
        // Part 2
        if (collision.steps < fewest_steps)
            fewest_steps = collision.steps;
    }

    std::cout << "Closest collision: " << closest_collision.steps << std::endl;
    std::cout << "Fewe steps: " << fewest_steps << std::endl;

    return 0;
}