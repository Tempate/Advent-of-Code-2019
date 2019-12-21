#include <iostream>
#include <algorithm>
#include <array>

struct Moon {
    std::array<int, 3> pos;
    std::array<int, 3> velocity;

    Moon(const int x, const int y, const int z) {
        pos = {x,y,z};
        velocity = {0,0,0};
    }

    void apply_gravity(Moon moon) {
        for (int i = 0; i < 3; i++) {
            if (moon.pos[i] > pos[i])
                velocity[i]++;
            else if (moon.pos[i] < pos[i])
                velocity[i]--;
        }
    }

    void apply_velocity() {
        for (int i = 0; i < 3; i++)
            pos[i] += velocity[i];
    }

    int potential_energy() const {
        return abs(pos[0]) + abs(pos[1]) + abs(pos[2]);
    }

    int kinetic_energy() const {
        return abs(velocity[0]) + abs(velocity[1]) + abs(velocity[2]);
    }

    int total_eneregy() const {
        return potential_energy() * kinetic_energy();
    }
};

void run_universe(std::array<Moon, 4> &moons) {
    for (int i = 0; i < moons.size(); i++) {
        for (int j = i+1; j < moons.size(); j++) {
            moons[i].apply_gravity(moons[j]);
            moons[j].apply_gravity(moons[i]);
        }
    }

    for (Moon &moon : moons)
        moon.apply_velocity();
}

int energy_after_time(std::array<Moon, 4> moons, const int n) {
    for (int t = 0; t < n; t++)
        run_universe(moons);

    int total_energy = 0;

    for (const Moon &moon : moons)
        total_energy += moon.total_eneregy();

    return total_energy;
} 

uint64_t repetition_for_axis(std::array<Moon, 4> moons, const uint64_t n) {
    const auto m = moons;
    
    uint64_t count = 1;
    bool equal = false;

    while (!equal) {
        count++;

        run_universe(moons);

        equal = true;

        for (int i = 0; i < moons.size() && equal; i++)
            equal = moons[i].pos[n] == m[i].pos[n];
    }

    return count;
}

uint64_t gcd(const uint64_t n, const uint64_t m) {   
    uint64_t p = std::max(m, n);
    uint64_t q = std::min(m, n);
    uint64_t r = p % q; 

    while (r != 0) {
        p = q;
        q = r;
        r = p % q;
    }

    return q;
}

uint64_t lcm(const uint64_t n, const uint64_t m) {
    return n * m / gcd(n, m);
}

int main() {
    std::array<Moon, 4> moons = {
        Moon{-14, -4, -11},
        Moon{-9, 6, -7},
        Moon{4, 1, 4},
        Moon{2, -14, -9}
    };

    std::cout << "Total Energy: " << energy_after_time(moons, 1000) << std::endl;

    const uint64_t rx = repetition_for_axis(moons, 0);
    const uint64_t ry = repetition_for_axis(moons, 1);
    const uint64_t rz = repetition_for_axis(moons, 2);

    std::cout << "Cycles of length: " << lcm(lcm(rx, ry), rz) << std::endl;

    return 0;
}