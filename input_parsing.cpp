#include <iostream>
#include <fstream>
#include <sstream>
#include <cassert>
#include <string>
#include <vector>
#include <type_traits>

std::vector<std::string> file_to_vector(const std::string &filename) {
    std::ifstream myfile(filename);

    if (!myfile.is_open()) {
        std::cout << "Unable to open file" << std::endl;
        exit(1);
    }

    std::vector<std::string> v;
    std::string line;

    while (getline(myfile, line))
        v.push_back(line);

    assert(v.size() > 0);

    myfile.close();

    return v;
}

std::vector<std::string> split_by_char(const std::string &s, const char separator) {
    std::vector<std::string> v;
    std::stringstream s_stream(s);

    while (s_stream.good()) {
        std::string word;
        getline(s_stream, word, separator);
        v.push_back(word);
    }

    return v;
}

template <typename T>
std::vector<T> parse_strings(const std::vector<std::string> &s) {
    std::vector<T> v;
    
    for (const auto &r : s) {
        if (std::is_same<T, int>::value)
            v.push_back(std::stoi(r));
        else if (std::is_same<T, int64_t>::value) {
            v.push_back(std::stoll(r));
        }
    }

    return v;
}

template <typename T>
std::vector<T> parse_program(const std::string &filename) {
    const auto input = file_to_vector(filename)[0];
    const auto strs = split_by_char(input, ',');

    return parse_strings<T>(strs);
}

