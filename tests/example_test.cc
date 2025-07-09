#include <gtest/gtest.h>
#include "leveldb/slice.h"

TEST(SliceTest, Create) {
    leveldb::Slice s("hello", 5);
    ASSERT_TRUE(true); // 占位
}
