#include <iostream>
#include <string>
#include <vector>
#include <array>

#include "../input_parsing.cpp"

const int LAYER_SIZE = 25*6;

int validate_image(const std::string image) {
    std::vector<std::array<int, 2>> data;

    for (int i = 0; i < image.size(); i += LAYER_SIZE) {
        std::array<int, 2> layer = {0, 0};

        int ones = 0;
        int twos = 0;

        for (char c : image.substr(i, LAYER_SIZE)) {
            switch (c) {
            case '0':
                layer[0]++;
                break;
            case '1':
                ones++;
                break;
            case '2':
                twos++;
                break;
            }
        }

        layer[1] = ones * twos;
        data.push_back(layer);
    }

    std::array<int, 2> record = {std::numeric_limits<int>::max(), 0};

    for (const auto &layer : data) {
        if (layer[0] < record[0])
            record = layer;
    }

    return record[1];
}

std::array<int, LAYER_SIZE> decode_image(const std::string image) {
    std::array<int, LAYER_SIZE> visible_layer;

    for (int i = 0; i < 150; i++)
        visible_layer[i] = 2;

    for (int i = 0; i < image.size(); i += LAYER_SIZE) {
        const std::string layer = image.substr(i, LAYER_SIZE);

        for (int i = 0; i < visible_layer.size(); i++) {
            if (visible_layer[i] == 2)
                visible_layer[i] = layer[i] - '0';
        }
    }

    return visible_layer;
}

int main() {
    std::string image = file_to_vector("input.txt")[0];

    std::cout << "Part 1: " << validate_image(image) << std::endl;

    const auto visible_layer = decode_image(image);

    std::cout << "Part 2: \n" << std::endl;

    for (int i = 0; i < visible_layer.size(); i += 25) {
        for (int j = 0; j < 25; j++) {
            switch (visible_layer[i+j]) {
            case 0:
                std::cout << "  ";
                break;
            case 1:
                std::cout << "* ";
                break;
            }
        }

        std::cout << std::endl;
    }

    return 0;
}

