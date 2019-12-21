#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>

#include "../input_parsing.cpp"

std::map<std::string, std::string> orbits;

// Count the number of planets a planet is orbiting.
int count_orbits(const std::string planet) {
    if (planet.compare("COM") == 0)
        return 0;

    return count_orbits(orbits[planet]) + 1;
}

// Returns an ordered list with all the planets a given planet is orbiting.
std::vector<std::string> orbiting_planets(const std::string planet) {
    std::vector<std::string> planets;
    
    if (planet.compare("COM") == 0)
        return planets;

    const auto indirect_planets = orbiting_planets(orbits[planet]);

    planets.assign(1, orbits[planet]);
    planets.insert(planets.end(), indirect_planets.begin(), indirect_planets.end());

    return planets;
}

int main() {
    for (const auto &line : file_to_vector("input.txt"))
        orbits[line.substr(4, 3)] = line.substr(0, 3);        

    // Part 1
    int orbit_count = 0;

    for (const auto& [key, val] : orbits)
        orbit_count += count_orbits(key);

    std::cout << "Number of orbits: " << orbit_count << std::endl;

    // Part 2
    int transfers = 0;

    const auto orbits_SAN = orbiting_planets("SAN");
    const auto orbits_YOU = orbiting_planets("YOU");

    for (int i = 0; i < orbits_SAN.size() && !transfers; i++) {
        for (int j = 0; j < orbits_YOU.size() && !transfers; j++) {
            
            if (orbits_SAN[i].compare(orbits_YOU[j]) == 0)
                transfers = i + j;
        }
    }

    std::cout << "Orbit transfers: " << transfers << std::endl;

    return 0;
}