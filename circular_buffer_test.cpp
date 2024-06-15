#include <gtest/gtest.h>
#include "circular_buffer.hpp"

const size_t queue_size = 10;
circular_buffer<uint32_t, queue_size> cq;
    

TEST(FullTest, BasicAssertions) {

    for (size_t i = 0; i < queue_size; i++) {
        cq.try_push_entity(i);
    }
    EXPECT_EQ(cq.full, true);
}

TEST(EmptyTest, BasicAssertions) {

    for (size_t i = 0; i < queue_size; i++) {
        uint32_t x = 0;
        cq.try_pop_entity(x);
    }

    EXPECT_EQ(cq.empty(), true);
}