#include <gtest/gtest.h>
#include "slice.h"


TEST(SliceTest, Create) {
    leveldb::Slice s("hello", 5);
    EXPECT_EQ(s.size(), 5);
    EXPECT_EQ(s.ToString(), "hello");
}

TEST(SliceTest, DefaultConstruction) {
    leveldb::Slice s;
    EXPECT_EQ(s.size(), 0);
    EXPECT_TRUE(s.empty());
}

TEST(SliceTest, StringConstruction) {
    std::string str = "test string";
    leveldb::Slice s(str);
    EXPECT_EQ(s.size(), str.size());
    EXPECT_EQ(s.ToString(), str);
}
