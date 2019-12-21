#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <map>
#include <vector>
#include <cassert>
#include <cmath>

#include "../input_parsing.cpp"

struct Chemical {
    std::string name;
    uint64_t n;

    Chemical(const std::string name, const uint64_t n) :
        name(name), n(n) {};
};

std::map<std::string, std::vector<Chemical>> recepies;

void make_element(std::map<std::string, uint64_t> &stash, Chemical chem) {
    
    if (chem.name == "ORE") {
        stash[chem.name] += chem.n;
    } else if (stash[chem.name] >= chem.n) {
        stash[chem.name] -= chem.n;
    } else {
            
        if (stash[chem.name] > 0) {
            chem.n -= stash[chem.name];
            stash[chem.name] = 0;
        }

        assert(recepies[chem.name].size() >= 2);

        int fact = chem.n / recepies[chem.name][0].n;

        if (fact * recepies[chem.name][0].n != chem.n)
            fact++;

        for (int i = 1; i < recepies[chem.name].size(); i++) {
            Chemical c = recepies[chem.name][i];
            c.n *= fact;

            make_element(stash, c);
        }

        stash[chem.name] += recepies[chem.name][0].n * fact - chem.n;
    }
}

int main() {
    for (const auto &line : file_to_vector("input.txt")) {
        const std::string last = line.substr(line.find_last_of(' ') + 1);
        const std::string aux = line.substr(line.find_first_of('=') + 3);

        const int n = std::stoi(aux.substr(0, aux.find_first_of(' ')));

        std::vector<Chemical> chemicals;
        chemicals.emplace_back(last, n);

        for (std::string &chem : split_by_char(line, ',')) {
            if (chem[0] == ' ')
                chem.erase(0, 1);

            auto v = split_by_char(chem, ' ');

            const std::string name = v[1];
            const int n = std::stoi(v[0]);

            chemicals.emplace_back(name, n);
        }

        recepies[last] = chemicals;
    }

    std::map<std::string, uint64_t> stash;
    
    make_element(stash, Chemical("FUEL", 1));

    std::cout << "Amount of ORE required for 1 FUEL: " << stash["ORE"] << std::endl;

    const uint64_t one_trillion = 1000000000000;

    const uint64_t lower_bound = one_trillion / stash["ORE"];
    const uint64_t upper_bound = 2 * lower_bound;

    stash.clear();

    make_element(stash, Chemical("FUEL", lower_bound));

    const int jump = 10;
    uint64_t count;

    std::map<std::string, uint64_t> aux_stash = stash;

    for (count = lower_bound; count < upper_bound && aux_stash["ORE"] < one_trillion; count += 10)
        make_element(aux_stash, Chemical("FUEL", jump));

    count -= jump;
    make_element(stash, Chemical("FUEL", count - lower_bound));

    while (stash["ORE"] < one_trillion) {
        make_element(stash, Chemical("FUEL", 1));
        count++;
    }

    std::cout << "Amount of FUEL for one trillion ORE: " << count - 1 << std::endl;

    return 0;
}