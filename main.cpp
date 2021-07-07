#include <iostream>
#include <concepts>
#include <cmath>
#include <vector>

template <std::integral T>
bool is_power_of_2(T i) {
    return i > 0 && (i & (i - 1)) == 0;
}

template <std::floating_point T>
bool is_power_of_2(T x) {
    int exponent;
    const T mantissa = std::frexp(x, &exponent);
    return mantissa == T(0.5);
}

int main() {
    std::cout << is_power_of_2(8) << std::endl;
    std::cout << is_power_of_2(32.0f) << std::endl;

    std::vector v = { 7, 3, 2, 4, 9 };

    std::erase_if(v, [](int i){ return i % 2 == 1; });

    for (auto i : v) {
        std::cout << i << std::endl;
    }

    return 0;
}
