#include <gtest/gtest.h>
#include <memory>

#define __MYHTTPD_UNIT_TEST__

#include "util/linked_buffer.hpp"

TEST(get_available_block, when_zero_capacity) {
    myhttpd::linked_buffer<4096> buf;
    auto b = buf.get_available_block();
    EXPECT_EQ(b.size, 4096);
    EXPECT_EQ(buf._capacity, 4096);
}

TEST(get_available_block, when_there_are_partial_block) {
    myhttpd::linked_buffer<100> buf;
    buf._blocks.push_back(myhttpd::linked_buffer<100>::BlockType());
    buf._size = 50;
    buf._capacity = 50;
    EXPECT_EQ(buf.get_available_block().size, 50);
}

TEST(get_available_block, only_returns_the_last_block) {
    myhttpd::linked_buffer<100> buf;
    buf._blocks.push_back(myhttpd::linked_buffer<100>::BlockType());
    buf._size = 50;
    buf._capacity = 50;
    auto b1 = buf.get_available_block();
    auto b2 = buf.get_available_block();
    EXPECT_EQ(b1.base, b2.base);
    EXPECT_EQ(b1.size, b2.size);
}

TEST(size, test) {
    myhttpd::linked_buffer<4096> buf;
    buf._size = 500;
    EXPECT_EQ(buf.size(), 500);
}

TEST(get_data, test) {
    myhttpd::linked_buffer<4096> buf;
    buf._blocks.push_back(myhttpd::linked_buffer<4096>::BlockType());
    buf._blocks.push_back(myhttpd::linked_buffer<4096>::BlockType());
    buf._offset = 2048;
    buf._size = 4096;
    EXPECT_EQ(buf.get_data(4096).size(), 2);
    EXPECT_EQ(buf.get_data(2048).size(), 1);
}

TEST(get_data, check_each_block_case1) {
    myhttpd::linked_buffer<100> buf;
    buf._blocks.push_back(myhttpd::linked_buffer<100>::BlockType());
    buf._blocks.push_back(myhttpd::linked_buffer<100>::BlockType());
    buf._offset = 50;
    buf._size = 100;
    buf._capacity = 50;
    auto bs = buf.get_data(100);
    auto it = bs.begin();
    EXPECT_EQ(it->size, 50);
    it++;
    EXPECT_EQ(it->size, 50);
}

TEST(get_data, check_each_block_case2) {
    myhttpd::linked_buffer<100> buf;
    buf._blocks.push_back(myhttpd::linked_buffer<100>::BlockType());
    buf._blocks.push_back(myhttpd::linked_buffer<100>::BlockType());
    buf._blocks.push_back(myhttpd::linked_buffer<100>::BlockType());
    buf._offset = 50;
    buf._size = 200;
    buf._capacity = 50;
    auto bs = buf.get_data(200);
    auto it = bs.begin();
    EXPECT_EQ(it->size, 50);
    it++;
    EXPECT_EQ(it->size, 100);
    it++;
    EXPECT_EQ(it->size, 50);
}

TEST(commit, test) {
    myhttpd::linked_buffer<100> buf;
    buf._blocks.push_back(myhttpd::linked_buffer<100>::BlockType());
    buf._capacity = 100;
    buf.commit(20);
    EXPECT_EQ(buf._size, 20);
    EXPECT_EQ(buf._capacity, 80);
}

TEST(consume, drop_block) {
    myhttpd::linked_buffer<100> buf;
    buf._blocks.push_back(myhttpd::linked_buffer<100>::BlockType());
    buf._blocks.push_back(myhttpd::linked_buffer<100>::BlockType());
    buf._blocks.push_back(myhttpd::linked_buffer<100>::BlockType());
    buf._size = 40;
    buf._offset = 80;
    buf._capacity = 180;
    buf.consume(40);
    EXPECT_EQ(buf._blocks.size(), 2);
    EXPECT_EQ(buf._capacity, 180);
    EXPECT_EQ(buf._offset, 20);
    EXPECT_EQ(buf._size, 0);
}