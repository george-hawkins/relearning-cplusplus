#include <gtest/gtest.h>
#include <deque>
#include <queue>

int factorial(int n) {
    return (n == 1 || n == 0) ? 1 : n * factorial(n - 1);
}

// From https://google.github.io/googletest/quickstart-cmake.html
// Demonstrate some basic assertions.
TEST(HelloTest, BasicAssertions) {
    // Expect two strings not to be equal.
    EXPECT_STRNE("hello", "world");
    // Expect equality.
    EXPECT_EQ(7 * 6, 42);
}

// You can introduce names like "FactorialTest" as and where you like - they just group tests into test suites.
TEST(FactorialTest, HandlesZeroInput) {
    EXPECT_EQ(factorial(0), 1);
}

// Tests factorial of positive numbers.
TEST(FactorialTest, HandlesPositiveInput) {
    EXPECT_EQ(factorial(1), 1);
    EXPECT_EQ(factorial(2), 2);
    EXPECT_EQ(factorial(3), 6);
    EXPECT_EQ(factorial(8), 40320);
}

// ---------------------------------------------------------------------

template <typename E>  // E is the element type.
class Queue {
public:
    void enqueue(const E& element) {
        elem.push(element);
    }
    std::optional<E> dequeue() {
        std::optional<E> r = elem.empty() ? std::nullopt : std::optional(elem.front());
        elem.pop();
        return r;
    }
    size_t size() const {
        return elem.size();
    }

private:
    std::queue<E> elem;
};

class QueueTest : public ::testing::Test {
protected:
    void SetUp() override {
        q1_.enqueue(1);
        q2_.enqueue(2);
        q2_.enqueue(3);
    }

    // void TearDown() override {}

    Queue<int> q0_;
    Queue<int> q1_;
    Queue<int> q2_;
};

// `TEST` expects an arbitrary suite name as its first argument, `TEST_F` expects a class name.
TEST_F(QueueTest, IsEmptyInitially) {
    EXPECT_EQ(q0_.size(), 0);
}

TEST_F(QueueTest, DequeueWorks) {
    auto n = q0_.dequeue();
    EXPECT_EQ(n, std::nullopt);

    n = q1_.dequeue();
    ASSERT_NE(n, std::nullopt);
    EXPECT_EQ(*n, 1);
    EXPECT_EQ(q1_.size(), 0);

    n = q2_.dequeue();
    ASSERT_NE(n, std::nullopt);
    EXPECT_EQ(*n, 2);
    EXPECT_EQ(q2_.size(), 1);
}