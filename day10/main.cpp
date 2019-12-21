#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <array>
#include <cmath>

#include "../input_parsing.cpp"

std::vector<std::vector<bool>> asteroids;
int max_x = std::numeric_limits<int>::max();
int max_y = std::numeric_limits<int>::max();

std::vector<std::array<int, 2>> proportional_vectors(const std::array<int, 2> v) {
    std::vector<std::array<int, 2>> vectors;

    int n = 2;

    while ( (abs(v[0] / n) >= 1 || v[0] == 0) && 
            (abs(v[1] / n) >= 1 || v[1] == 0)) {
        
        if (v[0] % n == 0 && v[1] % n == 0) {
            const std::array<int, 2> w = {v[0] / n, v[1] / n};

            for (int i = 1; i < n; i++)
                vectors.push_back({w[0] * i, w[1] * i});
        }

        n++;
    }

    return vectors;
}

int watchable_asteroids(const int X, const int Y) {
    int n = 0;

    for (int y = 0; y < max_y; y++) {
        for (int x = 0; x < max_x; x++) {
            if ((x != X || y != Y) && asteroids[y][x]) {
                const std::array<int, 2> v = {x- X, y - Y};
                const auto vectors = proportional_vectors(v);

                bool blocked = false;

                for (const auto &w : vectors) {
                    if (asteroids[Y + w[1]][X + w[0]]) {
                        blocked = true;
                        break;
                    }
                }

                if (!blocked)
                    n++;
            }
        }
    }

    return n;
}

int calc_station_value() {
    std::array<int, 2> best_asteroid = {-1, -1};
    int watchable = std::numeric_limits<int>::min();

    // All lines of asteroids are assumed to have the same size

    for (int y = 0; y < asteroids.size(); y++) {
        for (int x = 0; x < asteroids[0].size(); x++) {
            if (asteroids[y][x]) {
                const int n = watchable_asteroids(x, y);

                if (n > watchable) {
                    watchable = n;
                    best_asteroid = {x, y};
                }
            }
        }
    }

    return watchable;
}

float calc_angle(const std::array<int, 2> v, const std::array<int, 2> w) {
    const int dot = v[0]*w[0] + v[1]*w[1];
    const int det = v[0]*w[1] - v[1]*w[0];

    return atan2(-det, -dot) + M_PI;
}

std::array<int, 2> next_asteroid(const std::array<int, 2> from, const std::array<int, 2> current) {
    std::array<int, 2> next;
    float best_angle = 2 * M_PI + 1;

    const std::array<int, 2> v = {current[0] - from[0], current[1] - from[1]};
    const float current_angle = acosf64(current[1]);

    for (int y = 0; y < asteroids.size(); y++) {
        for (int x = 0; x < asteroids[0].size(); x++) {
            if (asteroids[y][x]) {
                const std::array<int, 2> w = {x - from[0], y - from[1]};
                float angle = calc_angle(v, w);

                if ((x == current[0] && y == current[1]) ||
                    (x == from[0] && y == from[1]))
                    continue;

                if (angle < best_angle) {
                    best_angle = angle;
                    next = {x, y};
                }
            }
        }
    }

    return next;
}

std::array<int, 2> find_n_asteroid(const int n, const std::array<int, 2> laser) {
    std::array<int, 2> asteroid;

    std::array<int, 2> first = laser;
    bool found = false;

    while (!found) {
        first[1]--;

        if (asteroids[first[1]][first[0]]) {
            asteroid = first;
            found = true;
        }
    }

    for (int i = 0; i < n - 1; i++) { 
        const auto aux = next_asteroid(laser, asteroid);
        asteroids[asteroid[1]][asteroid[0]] = false;
        asteroid = aux;
    }

    return asteroid;
}

int main() {
    for (const auto &line : file_to_vector("input.txt")) {
        std::vector<bool> asteroid_row;

        for (const char c : line)
            asteroid_row.push_back(c == '#');

        asteroids.push_back(asteroid_row);
    }

    max_y = asteroids.size();
    max_x = asteroids[0].size();

    std::cout << "Part 1: " << calc_station_value() << std::endl;

    std::array<int, 2> asteroid = find_n_asteroid(200, {14, 17});
    std::cout << "Part 2: " << asteroid[0] * 100 + asteroid[1] << std::endl;

    return 0;
}

