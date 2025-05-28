
#include <gtest/gtest.h>
#include <iostream>
#include <lockfree/queue.hpp>

TEST(QueueBasics, InitialSizeIsZero) {
    dero::lockfree::queue<int, 10> q;
    EXPECT_EQ(q.size(), 0);
}

TEST(QueueBasics, PushAndPopSuccess) {
    dero::lockfree::queue<int, 10> q;
    EXPECT_TRUE(q.push(1));
    EXPECT_TRUE(q.push(2));
    EXPECT_TRUE(q.push(3));
    EXPECT_TRUE(q.push(4));

    ASSERT_EQ(q.size(), 4);
    EXPECT_EQ(q.pop().value(), 1);
    EXPECT_EQ(q.pop().value(), 2);

    EXPECT_TRUE(q.push(5));
    EXPECT_TRUE(q.push(6));

    ASSERT_EQ(q.size(), 4);
    EXPECT_EQ(q.pop().value(), 3);
    EXPECT_EQ(q.pop().value(), 4);
    EXPECT_EQ(q.pop().value(), 5);
    EXPECT_EQ(q.pop().value(), 6);

    EXPECT_EQ(q.size(), 0);
}

TEST(QueueBasics, PushAndPopFailure) {
    dero::lockfree::queue<int, 5> q;
    EXPECT_EQ(q.pop(), std::nullopt);

    EXPECT_TRUE(q.push(1));
    EXPECT_TRUE(q.push(2));
    EXPECT_TRUE(q.push(3));
    EXPECT_TRUE(q.push(4));
    EXPECT_TRUE(q.push(5));

    EXPECT_FALSE(q.push(6));
    EXPECT_FALSE(q.push(7));

    ASSERT_EQ(q.size(), 5);
    EXPECT_EQ(q.pop().value(), 1);
    EXPECT_EQ(q.pop().value(), 2);

    EXPECT_TRUE(q.push(6));
    EXPECT_TRUE(q.push(7));
    EXPECT_FALSE(q.push(8));
}
