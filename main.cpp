#include <iostream>
#include <concepts>
#include <cmath>
#include <vector>
#include <complex>
#include <memory>

#include "Foo.h"

// Modules are only supported in g++-11 and _some_ module related features require -fmodules-ts
// See https://gcc.gnu.org/projects/cxx-status.html#cxx20

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

// `using x = y` is essentially a typedef that can then be used in subsequent templates.
template <typename T>
using complex_array_ref_t = T(&)[2];

template<typename T>
complex_array_ref_t<T> cartesian (std::complex<T>& z)
{
    return reinterpret_cast<complex_array_ref_t<T>>(z);
}

// Chromium have a nice page on smart pointers - https://www.chromium.org/developers/smart-pointer-guidelines
// In particular they "What about passing or returning a smart pointer by reference? Don't do this."
static std::unique_ptr<Bar> alpha() {
    return std::make_unique<Bar>();
}

static void beta() {
    auto x = alpha();

    std::cout << "Expecting Bar to be dtored now..." << std::endl;
}

std::vector<int> return_local() {
    // C++ can infer the type arguments for `vector` etc.
    std::vector x { 1, 2, 3, 4 };

    return x;
}

int main() {
    // Templates with concepts.
    std::cout << is_power_of_2(8) << std::endl;
    std::cout << is_power_of_2(32.0f) << std::endl;

    // -----------------------------------------------------------------

    std::vector v = { 7, 3, 2, 4, 9 };

    // `erase` with `begin` and `end`.
    std::erase_if(v, [](int i){ return i % 2 == 1; });

    for (auto i : v) {
        std::cout << i << std::endl;
    }

    // -----------------------------------------------------------------

    // Debug when dtors get called.
    foo();

    // -----------------------------------------------------------------

    // A Tour of C++ (2nd ed. p45) claims you can use structural binding directly on `complex` but you can't.
    // But you can with a bit of trickery - see https://www.codeproject.com/Articles/1249535/Hacking-on-Structured-Bindings
    std::complex y { 1, 2 };
    auto z = y + 2;
    auto [re, im] = cartesian(z); // `cartesian` expects a ref, so `z` works but `z + 2` doesn't.

    std::cout << re << " + " << im << 'i' << std::endl;

    // -----------------------------------------------------------------

    // Debug `unique_ptr` behavior.
    beta();

    //throw 42;
    //throw std::runtime_error("foo bar");

    return 0;
}
