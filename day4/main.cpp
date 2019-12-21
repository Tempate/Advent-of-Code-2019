#include <iostream>
#include <cassert>

template <bool simple>
bool valid_password(int pass) {
    bool repetition = false;
    int failed_int = -1;
    int repeat_int = -1;

    bool decreasing = true;
    int last_digit = std::numeric_limits<int>::max();

    while (pass > 0 && decreasing) {
        const int new_last_digit = pass % 10;
        
        decreasing = last_digit >= new_last_digit;

        if constexpr (simple) {
            if (last_digit == new_last_digit)
                repetition = true;
        } else {
            if (last_digit == new_last_digit) {
                if (repetition == true && repeat_int == new_last_digit) {
                    repetition = false;
                    failed_int = new_last_digit;
                } 
                
                else if (repetition == false && failed_int != new_last_digit) {
                    repetition = true;
                    repeat_int = new_last_digit;
                }
            }
        }

        last_digit = new_last_digit;
        pass /= 10;
    }

    return decreasing && repetition;
}

template <bool simple>
int count_valid_passwords(const int min, const int max) {
    int n = 0;

    for (int pass = min; pass < max; pass++) {
        if (valid_password<simple>(pass))
            n++;
    }

    return n;
}

int main() {
    const int min = 265275;
    const int max = 781584;

    std::cout << "Part 1: " << count_valid_passwords<true>(min, max) << std::endl;
    std::cout << "Part 2: " << count_valid_passwords<false>(min, max) << std::endl;

    return 0;
}