#include <iostream>
#include <fstream>
#include <vector>

#include "../input_parsing.cpp"

std::vector<int> g(const std::vector<int> &signal) {
    std::vector<int> new_signal;
    new_signal.resize(signal.size());

    for (int i = 0; i < signal.size(); i++) {
        int sum = i;
        int sub = 2*(i+1) + sum;

        while (sum < signal.size()) {
            for (int k = 0; k < i+1; k++) {
                if (sum + k < signal.size())
                    new_signal[i] += signal[sum + k];

                if (sub + k < signal.size())
                    new_signal[i] -= signal[sub + k];
            }

            sum += 4*(i+1);
            sub += 4*(i+1);
        }

        new_signal[i] = abs(new_signal[i] % 10);
    }

    return new_signal;
}

std::vector<std::vector<int>> next_comb(std::vector<std::vector<int>> comb) {
    const int N = comb.size();
    
    std::vector<std::vector<int>> next(N);

    for (int i = 0; i < N; i++) {
        std::vector<int> factors(N, 0);

        int sum = i;
        int sub = 2*(i+1) + sum;

        while (sum < 10000 * N) {
            for (int k = 0; k < i+1 && sum + k < 10000 * N; k++) {
                for (int j = 0; j < N; j++) {
                    factors[j] += comb[(sum+k)%comb.size()][j] % 10;

                    if (sub + k < N)
                        factors[j] -= comb[(sub+k)%comb.size()][j] % 10;
                }
            }

            const int jump = 4*(i+1);
            sum += jump;
            sub += jump;
        }

        next[i] = factors;
    }

    return next;
}

int main() {
    std::vector<int> signal;
    
    for (const auto &line : file_to_vector("input.txt")) {
        for (char c : line)
            signal.push_back(c - '0');
    }

    // Part 1

    std::vector<int> s = signal;

    for (int i = 0; i < 100; i++)
        s = g(s);

    std::cout << "Part 1: ";

    for (int i = 0; i < 8; i++)
        std::cout << s[i];

    std::cout << std::endl;

    // Part 2

    // std::cout << signal2.size() << std::endl;

    int offset = 0;

    for (int i = 0; i < 7; i++)
        offset = offset * 10 + signal[i];

    std::vector<std::vector<int>> canonic(signal.size());

    for (int i = 0; i < signal.size(); i++) {
        std::vector<int> v(signal.size(), 0);
        v[i] = signal[i];

        canonic[i] = v;
    }

    auto factors = canonic;

    for (int i = 0; i < 100; i++)
        factors = next_comb(factors);

    /*
    for (const auto row : factors) {
        for (const auto v : row)
            std::cout << v << " ";

        std::cout << std::endl;
    }
    */

    for (int i = 0; i < 8; i++) {
        const auto facts = factors[(offset + i) % factors.size()];
        int d = 0;

        for (int j = 0; j < facts.size(); j++)
            d += facts[j]; //abs(facts[j] % 10);

        std::cout << abs(d % 10);
    }

    std::cout << std::endl;

    return 0;
}