#include <iostream>
#include <filesystem>
#include <gtest/gtest.h>
#include <forward_list>

class BaseA {
public:
    virtual ~BaseA() = default;

    virtual void x() = 0;
};

class ChildA : public BaseA {
public:
    ~ChildA() override {
        std::cout << "ChildA dtor" << std::endl;
    }

    void x() override { }
};

class GrandChildA : public ChildA {
public:
    GrandChildA(int _v = 0) : v { _v } {}

    ~GrandChildA() override {
        std::cout << "GrandChildA " << v << " dtor" << std::endl;
    }

private:
    int v;
};

TEST(TourTest, DefaultBaseVirtualDtor) {
    BaseA* a = new GrandChildA();
    auto b = std::make_unique<GrandChildA>(2);

    std::cout << std::boolalpha; // Output bools as true/false rather than 1/0.
    std::cout << std::is_trivial<BaseA>::value << std::endl;
    std::cout << std::is_trivial<ChildA>::value << std::endl;

    delete a;
}

// Go to Build tab under Messages to see warnings.

class X {
public:
    X(int) { std::cout << "1" << std::endl; }
    X() { std::cout << "2" << std::endl; }
    X(const X&) { std::cout << "3" << std::endl; }
    X(X&&) { std::cout << "4" << std::endl; }

    X& operator=(const X&) { std::cout << "5" << std::endl; throw std::runtime_error("unsupported"); }
    X& operator=(X&&) { std::cout << "6" << std::endl; throw std::runtime_error("unsupported"); }
};

TEST(TourTest, ElideCopy) {
    X make(int);
    std::cout << "A" << std::endl;
    X x = make(3);
    std::cout << "C" << std::endl;
}

X make(int x) {
    std::cout << "B" << std::endl;
    return X { x };
}

TEST(TourTest, TypeDeduction) {
    // Type deduction seems pretty poor when it comes to these examples.

    auto x = std::make_unique<std::vector<int>>(std::initializer_list<int> { 1, 2, 3 });
    auto y = std::make_unique<std::vector<int>>(std::vector { 1, 2, 3 });

    std::unique_ptr<std::vector<int>> z { new std::vector { 1, 2, 3 } };
}

template<typename Transport, typename ...TransportArgs>
class InputChannel {
public:
    InputChannel(TransportArgs&&... transportArgs) : _transport(std::forward<TransportArgs>(transportArgs)...) {}
private:
    Transport _transport;
};

// Clang libc++ - https://libcxx.llvm.org/
// Older pages refer to _LIBCPP_DEBUG2 but it was renamed to _LIBCPP_DEBUG:
// https://github.com/llvm/llvm-project/commit/145afa17caedb3ff6d58dfd75dd72047810190d1
// Older pages have _LIBCPP_DEBUG2 set to 2 but modern _LIBCPP_DEBUG supports just 0 or 1:
// https://github.com/llvm/llvm-project/blob/llvmorg-12.0.1/libcxx/include/__config#L873
// https://libcxx.llvm.org//DesignDocs/DebugMode.html
// Undefined means no debug mode, 1 enables it and 2 enables it also for iterators.
// So add this to CMakeLists.txt:
//   add_compile_definitions("_LIBCPP_DEBUG=1")
//
// However, it turns out that even when using Clang, it links against GNU's libstdc++
// GNU libstdc++ - https://gcc.gnu.org/onlinedocs/libstdc++/
// For it you need to define _GLIBCXX_DEBUG:
// https://gcc.gnu.org/onlinedocs/libstdc++/manual/debug_mode_using.html
// So add this to CMakeLists.txt:
//   add_compile_definitions("_GLIBCXX_DEBUG")
// Unlike _LIBCPP_DEBUG, it doesn't take a value, it's either defined or not.
//
TEST(MiscTest, AtVsSubscriptDebugMode) {
    std::vector<int> v;

    // Without _GLIBCXX_DEBUG this is undefined behavior, with _GLIBCXX_DEBUG it results in an error message
    // and the program is aborted. I.e. it doesn't result in an std::out_of_range as you'd set with `at`.
    std::cout << v[0] << std::endl;
}

TEST(TourTest, BooleanIstream) {
    // You can convert cin to a bool with `bool operator basic_ios::bool()`.
    if (std::cin) {

    }

    // However, you can't assign it to a bool as the `bool()` operator is marked `explicit`.
    //bool x = std::cin; // Calls

    // However, this is completely OK - brilliant.
    bool x = !!std::cin;
}

TEST(TourTest, Path) {
    std::filesystem::path x = "/";

    // Note that `name()` isn't guaranteed to be anything human readable.
    std::cout << typeid(int).name() << std::endl;
    std::cout << typeid(std::filesystem::path::value_type).name() << std::endl;
}

class FooElement {
private:
    int _x;

public:
    FooElement(int x) : _x(x) { }
};

class FooContainer {
public:
    void push_back(const FooElement& copy) {
        std::cout << "copy" << std::endl;
    }

    void push_back(FooElement&& move) {
        std::cout << "move" << std::endl;
    }
};

FooElement create_foo() {
    return { 3 };
}

TEST(TourTest, MoveOrCopy) {
    FooContainer container;
    FooElement alpha { 1 };

    container.push_back(alpha);
    container.push_back(std::move(alpha)); // `move` doesn't move - it just turns an lvalue into an rvalue.
    container.push_back({ 2 });
    container.push_back(create_foo());
}

TEST(TourTest, VectorAndPolymorhism) {
    std::vector<std::unique_ptr<BaseA>> v;

    // This works fine.
    v.push_back(std::make_unique<ChildA>());

    auto childA = std::make_unique<ChildA>();

    // Here we have to use `move` as you can't copy a `unique_ptr`.
    v.push_back(std::move(childA));

    // As you've moved `childA` doing anything with this point is UB.

    std::cout << "children will be destroyed when v goes out of scope" << std::endl;
}

TEST(TourTest, ForwardList) {
    std::forward_list<int> l;
    auto i = 0;

    for (const auto& x : l) {
        i++;
    }

    std::cout << i << std::endl;
}

TEST(TourTest, Map) {
    std::map<std::string, int> m;


    // Wow - trying to retrieve a key that isn't present results in it being inserted with a default value.
    std::cout << m.contains("a") << std::endl;
    std::cout << m["a"] << std::endl;
    std::cout << m.contains("a") << std::endl;
}

std::vector<std::string::const_iterator> find_all(const std::string& s, char c) {
    std::vector<std::string::const_iterator> res;

    for (std::string::const_iterator p = s.begin(); p != s.end(); ++p) {
        if (*p == c) res.push_back(p);
    }
    return  res;
}

std::vector<std::string::const_iterator> find_all2(const std::string& s, char c) {
    std::vector<std::string::const_iterator> res;
    auto find = [&](auto it) { return std::find(it, s.end(), c); };
    auto curr = find(s.begin());

    while (curr != s.end()) {
        res.push_back(curr);
        curr = find(++curr);
    }
    return  res;
}

using namespace std::string_literals;

TEST(TourTest, Find) {
    auto m = "Mary had a little lamb"s;
    auto c = 'a';
    auto count = 0;
    for (auto p : find_all2(m, c)) {
        if (*p != c) std::cerr << "a bug!" << std::endl;
        else count++;
    }
    std::cout << count << std::endl;
}

#include <algorithm>

bool find_fn(int i) {
    return i > 4;
}

TEST(TourTest, FindPredicate) {
    std::vector m { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

    auto a = std::ranges::find(m, 5);
    auto b = std::ranges::find_if(m, [](auto i) { return i > 4; });
    auto c = std::ranges::find_if(m, find_fn);

    if (a != m.end()) std::cout << *a << std::endl;
    if (b != m.end()) std::cout << *b << std::endl;
    if (c != m.end()) std::cout << *c << std::endl;
}

TEST(TourTest, OstreamIterator) {
    std::ostream_iterator<std::string> oo { std::cout };

    *oo = "Hello, ";
    ++oo; // Does absolutely nothing!
    *oo = "world!\n";
}

TEST(MiscTest, LibVersion) {
#ifdef __GLIBCPP__
    std::printf("GLIBCPP: %d\n",__GLIBCPP__);
#endif
#ifdef __GLIBCXX__
    std::printf("GLIBCXX: %d\n",__GLIBCXX__);
    std::printf("GLIBCXX_RELEASE: %d\n",_GLIBCXX_RELEASE);
#endif
}

#include <concepts>

#include <gsl/gsl>
#include <span>

TEST(TourTest, RangeSort) {
    std::vector m { 'x', 'y', 'z' };
    auto s = "xyz"s;

    std::ranges::sort(m);
    std::ranges::sort(s);

    auto i = std::ostream_iterator<char>(std::cout, " ");
    std::ranges::copy(m, i);

    std::cout << s << std::endl;
}

void fs(gsl::span<int> p) {
    auto count = 0;
    for (int& x : p) {
        x = 0;
        count++;
    }

    std::cout << count << std::endl;
}

void std_fs(std::span<int> p) {
    auto count = 0;
    for (int& x : p) {
        x = 0;
        count++;
    }

    std::cout << count << std::endl;
}

TEST(TourTest, Span) {
    int a[100];

    std_fs(a);
    // std_fs({ a + 10, 100 }); SEGV or unfortunate silences.
    // std_fs({a, 1000 }); SEGV or unfortunate silences.

    fs(a);
    // fs({ a + 10, 100 }); SEGV or unfortunate silences.
    // fs({a, 1000 }); SEGV or unfortunate silences.

    std::span<int> x { a };
    gsl::span<int> y { a };

    // std::cout << x[x.size()] << std::endl; // Actively checks and ABRT - good.
    // std::cout << y[y.size()] << std::endl; // Ditto.

    std::cout << *x.end() << std::endl; // No check - `end` returns a normal iterator with no checking on `*` operator.
    //std::cout << *y.end() << std::endl; // Actively checks and ABRT - good.
}

TEST(TourTest, DirectInitialization) {
    std::vector<int> p { 1000 }; // Direct list initialization.
    std::vector<int> q(1000); // Direct initialization.
    std::vector<int> r(1000, 42);
    std::vector<int> s { 1000, 1001 };

    std::cout << p.size() << ' ' << p[0] << std::endl;
    std::cout << q.size() << ' ' << q[0] << std::endl;
    std::cout << r.size() << ' ' << r[0] << std::endl;
    std::cout << s.size() << ' ' << s[0] << std::endl;
}

struct FooVec {
    std::vector<int> v { 1, 2, 3 };
};

struct FooArray {
    std::array<int, 3> a { 1, 2, 3 };
};

TEST(TourTest, VectorVsArray) {
    FooVec fv;
    FooArray fa;

    // uintptr_t is a numeric type guaranteed to be large enough to hold a pointer.
    auto a0 = reinterpret_cast<uintptr_t>(&fv);
    auto b0 = reinterpret_cast<uintptr_t>(&(fv.v));
    auto c0 = reinterpret_cast<uintptr_t>(&(fv.v[0])); // These are somewhere else.
    auto d0 = reinterpret_cast<uintptr_t>(&(fv.v[1]));
    auto e0 = reinterpret_cast<uintptr_t>(&(fv.v[2]));

    auto a1 = reinterpret_cast<uintptr_t>(&fa);
    auto b1 = reinterpret_cast<uintptr_t>(&(fa.a));
    auto c1 = reinterpret_cast<uintptr_t>(&(fa.a[0])); // These are inline (as it were).
    auto d1 = reinterpret_cast<uintptr_t>(&(fa.a[1]));
    auto e1 = reinterpret_cast<uintptr_t>(&(fa.a[2]));

    std::cout << a0 << std::endl;
    std::cout << b0 << std::endl;
    std::cout << c0 << std::endl;
    std::cout << d0 << std::endl;
    std::cout << e0 << std::endl;

    std::cout << a1 << std::endl;
    std::cout << b1 << std::endl;
    std::cout << c1 << std::endl;
    std::cout << d1 << std::endl;
    std::cout << e1 << std::endl;
}

#include <date/date.h>
#include <mutex>

using namespace std::chrono;
using namespace date; // In std::chrono namespace in libstdc++ 11.2.0.

TEST(TourTest, Chrono) {
    auto t0 = high_resolution_clock::now();
    auto t1 = t0 + 10s;
    auto diff = t1 - t0; // `diff` is a `duration`.
    auto count = diff.count(); // `count` is just a long.

    std::cout << duration_cast<microseconds>(diff).count() << std::endl;
    std::cout << duration_cast<seconds>(diff).count() << std::endl;
}

TEST(TourTest, Date) {
    auto x = year{2021}/7/29;

    std::cout << x << std::endl;
}

TEST(TourTest, Locks) {
    std::mutex mm;
    std::unique_lock lck { mm };

    lck.unlock();

    std::scoped_lock lck2 { mm }; // Can only unlock on going out of scope.
}

TEST(TourTest, CurrentException) {
    std::exception_ptr e41 = []() {
        try {
            throw 41;
        } catch (...) {
            return std::current_exception();
        }
    }();
    std::exception_ptr e42 = []() {
        try {
            throw 42;
        } catch (...) {
            return std::current_exception();
        }
    }();

    try {
        throw 42;
    } catch (const std::exception& e) {
        // std::exception is the root of all the standard exceptions like `runtime_exception`.
        std::cout << e.what() << std::endl;
    } catch (...) {
        std::exception_ptr e = std::current_exception();

        // Neither of these compare equal - seems exceptions are unique, irrespective of their value.
        std::cout << (e == e41) << std::endl;
        std::cout << (e == e42) << std::endl;

        // `e` is an `exception_ptr` - there's nothing you can do with other than store it and/or rethrow it.
        // In this case there's no way to work out that it's an `int` other than to rethrow it.
        // So don't think of it as an int but as something that the system knows to match to a `catch` and unwrap.
        try {
            std::rethrow_exception(e);
        } catch (const int e) {
            std::cout << e << std::endl;
        } catch (...) {
            std::cout << "unknown exception" << std::endl;
        }
    }
}